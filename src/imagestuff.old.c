/**************************************************************************
 *									  *
 * 		 Copyright (C) 1986, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/


/*
 *	iopen -
 *
 *				Paul Haeberli - 1984
 *
 */

#if defined(LINUX) || defined(NT) || defined(MACX)
#define _IORW  256
#define _IOWRT   2
#define _IOREAD  1
#define _IOERR  32
#endif

#include	<stdio.h> 
#include	<stdlib.h>
#include        <string.h>
#ifdef MACX
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif
#include	"image.h"

#define INTSIZE 4
int intsize = INTSIZE;

unsigned int img_optseek(IMAGE *image, unsigned int offset);
unsigned int img_seek(IMAGE *image, unsigned int y, unsigned int z);
void img_setrowsize(IMAGE *image, int cnt, int y, int z);
void isetname(IMAGE *image, char *name);
void cvtcrayshort(short *buffer, int nitems);

IMAGE *imgopen(long, char *, char *,unsigned int, unsigned int,
		unsigned int, unsigned int, unsigned int);

IMAGE *iopen(char *file, char *mode, unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
    return(imgopen(0, file, mode, type, dim, xsize, ysize, zsize));
}

IMAGE *fiopen(long f, char *mode, unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
    return(imgopen(f, 0, mode, type, dim, xsize, ysize, zsize));
}

IMAGE *imgopen(long f, char *file, char *mode,
		unsigned int type, unsigned int dim,
		unsigned int xsize, unsigned int ysize, unsigned int zsize)
{
	register IMAGE 	*image;
	register rw;
	int tablesize, acttablesize;
	register int i, max;

	image = (IMAGE*)calloc(1,sizeof(IMAGE));
	if(!image ) {
	    i_errhdlr("iopen: error on image struct alloc\n");
	    return NULL;
	}
	rw = mode[1] == '+';
	if(rw) {
	    i_errhdlr("iopen: read/write mode not supported\n");
		return NULL;
	}
	if (*mode=='w') {
		if (file) {
		    f = creat(file, 0666);
		    if (rw && f>=0) {
			    close(f);
			    f = open(file, 2);
		    }
		}
		if (f < 0) {
		    i_errhdlr("iopen: can't open output file %s\n",file);
		    return NULL;
		}
		image->imagic = IMAGIC;
		image->type = type;
		image->xsize = xsize;
		image->ysize = 1;
		image->zsize = 1;
		if (dim>1)
		    image->ysize = ysize;
		if (dim>2)
		    image->zsize = zsize;
		if(image->zsize == 1) {
		    image->dim = 2;
		    if(image->ysize == 1)
			image->dim = 1;
		} else {
		    image->dim = 3;
		}
		image->min = 10000000;
		image->max = 0;
		isetname(image,"no name"); 
		image->wastebytes = 0;
		image->dorev = 0;

#if !defined(CRAY)

		if (write(f,image,sizeof(IMAGE)) != sizeof(IMAGE)) {
		    i_errhdlr("iopen: error on write of image header\n");
		    return NULL;
		}

#endif

	} else {
		if (file)
		    f = open(file, rw? 2: 0);
		if (f < 0)
		    return(NULL);
		if (read(f,image,sizeof(IMAGE)) != sizeof(IMAGE)) {
		    i_errhdlr("iopen: error on read of image header\n");
		    return NULL;
		}
		if( ((image->imagic>>8) | ((image->imagic&0xff)<<8)) 
							     == IMAGIC ) {
		    image->dorev = 1;
		    cvtimage(image);
		} else
		    image->dorev = 0;
		if (image->imagic != IMAGIC) {
			i_errhdlr("iopen: bad magic in image file %x\n",image->imagic);
		    return NULL;
		}
	}
	if (rw)
	    image->flags = _IORW;
	else if (*mode != 'r')
	    image->flags = _IOWRT;
	else
	    image->flags = _IOREAD;
	if(ISRLE(image->type)) {
	    tablesize = image->ysize*image->zsize*sizeof(int);
	    acttablesize = image->ysize*image->zsize*intsize;
	    image->rowstart = (unsigned int *)malloc(tablesize);
	    image->rowsize = (int *)malloc(tablesize);
	    if( image->rowstart == 0 || image->rowsize == 0 ) {
		i_errhdlr("iopen: error on table alloc\n");
		return NULL;
	    }
	    image->rleend = 512L+2*acttablesize;
	    if (*mode=='w') {
		max = image->ysize*image->zsize;
		for(i=0; i<max; i++) {
		    image->rowstart[i] = 0;
		    image->rowsize[i] = -1;
		}
	    } else {
		tablesize = image->ysize*image->zsize*sizeof(int);
		lseek(f, 512L, 0);
		if (read(f,image->rowstart,tablesize) != tablesize) {
		    i_errhdlr("iopen: error on read of rowstart\n");
		    return NULL;
		}
		if(image->dorev)
		    cvtints(image->rowstart,tablesize);
		if (read(f,image->rowsize,tablesize) != tablesize) {
		    i_errhdlr("iopen: error on read of rowsize\n");
		    return NULL;
		}
		if(image->dorev)
		    cvtints(image->rowsize,tablesize);
	    }
	}
	image->cnt = 0;
	image->ptr = 0;
	image->base = 0;
	if( (image->tmpbuf = ibufalloc(image)) == 0 ) {	
	    i_errhdlr("iopen: error on tmpbuf alloc %d\n",image->xsize);
	    return NULL;
	}
	image->x = image->y = image->z = 0;
	image->file = f;
	image->offset = 512L;			/* set up for img_optseek */
	lseek(image->file, 512L, 0);
	return(image);
}

unsigned short *ibufalloc(IMAGE *image)
{
    return (unsigned short *)malloc(IBUFSIZE(image->xsize));
}

reverse(lwrd) 
register unsigned int lwrd;
{
    return ((lwrd>>24) 		| 
	   (lwrd>>8 & 0xff00) 	| 
	   (lwrd<<8 & 0xff0000) | 
	   (lwrd<<24) 		);
}

cvtshorts( buffer, n)
register unsigned short buffer[];
register int n;
{
    register short i;
    register int nshorts = n>>1;
    register unsigned short swrd;

    for(i=0; i<nshorts; i++) {
	swrd = *buffer;
	*buffer++ = (swrd>>8) | (swrd<<8);
    }
}

cvtints( buffer, n)
register int buffer[];
register int n;
{
    register short i;
    register int nints = n>>2;
    register unsigned int lwrd;

    for(i=0; i<nints; i++) {
	lwrd = buffer[i];
	buffer[i] =     ((lwrd>>24) 		| 
	   		(lwrd>>8 & 0xff00) 	| 
	   		(lwrd<<8 & 0xff0000) 	| 
	   		(lwrd<<24) 		);
    }
}

cvtimage( buffer )
register int buffer[];
{

#if defined(CRAY)

    return;

#else

    cvtshorts(buffer,12);
    cvtints(buffer+3,12);
    cvtints(buffer+26,4);

#endif

}

static void (*i_errfunc)();

/*	error handler for the image library.  If the iseterror() routine
	has been called, sprintf's the args into a string and calls the
	error function.  Otherwise calls fprintf with the args and then
	exit.  This allows 'old' programs to assume that no errors
	ever need be worried about, while programs that know how and
	want to can handle the errors themselves.  Olson, 11/88
*/
i_errhdlr(fmt, a1, a2, a3, a4)	/* most args currently used is 2 */
char *fmt;
{
	if(i_errfunc) {
		char ebuf[2048];	/* be generous; if an error includes a
			pathname, the maxlen is 1024, so we shouldn't ever 
			overflow this! */
		sprintf(ebuf, fmt, a1, a2, a3, a4);
		(*i_errfunc)(ebuf);
		return;
	}
	fprintf(stderr, fmt, a1, a2, a3, a4);
	/* gmvexit(); */
}

/* this function sets the error handler for i_errhdlr */
i_seterror(func)
void (*func)();
{
	i_errfunc = func;
}


/*
 *	iclose and iflush -
 *
 *				Paul Haeberli - 1984
 *
 */

int iclose(IMAGE *image)
{
    int tablesize, acttablesize;

    iflush(image);
    img_optseek(image, 0);

#if defined(LINUX) || defined(DEC) || defined(NT)

    image->dorev = 1;

#endif

    if (image->flags&_IOWRT) {
	if(image->dorev)
	    cvtimage(image);

#if defined(CRAY)

    retval =  crayimgwrite(image->file,image,sizeof(IMAGE));
    if(retval == sizeof(IMAGE))
        image->offset += 152;
    else {
        image->offset = -1;
	i_errhdlr("iclose: error on write of image header\n");
	return EOF;
    }

#else

	if (img_write(image,(char *)image,sizeof(IMAGE)) != sizeof(IMAGE)) {
	    i_errhdlr("iclose: error on write of image header\n");
	    return EOF;
	}

#endif

	if(image->dorev)
	    cvtimage(image);
	if(ISRLE(image->type)) {
	    img_optseek(image, 512L);
	    tablesize = image->ysize*image->zsize*intsize;
	    acttablesize = image->ysize*image->zsize*sizeof(int);
	    if(image->dorev)
		cvtints(image->rowstart,tablesize);
#ifdef CRAY
	    if (cimg_write(image,(char *)(image->rowstart),acttablesize) 
               != tablesize) {
#else
	    if (img_write(image,(char *)(image->rowstart),tablesize) 
               != tablesize) {
#endif
		i_errhdlr("iclose: error on write of rowstart\n");
		return EOF;
	    }
	    if(image->dorev)
		cvtints(image->rowsize,tablesize);
#ifdef CRAY
	    if (cimg_write(image,(char *)(image->rowsize),acttablesize) 
               != tablesize) {
#else
	    if (img_write(image,(char *)(image->rowsize),tablesize) 
               != tablesize) {
#endif
		i_errhdlr("iclose: error on write of rowsize\n");
		return EOF;
	    }
	}
    }
    if(image->base) {
	free(image->base);
	image->base = 0;
    }
    if(image->tmpbuf) {
	free(image->tmpbuf);
	image->tmpbuf = 0;
    }
    if(ISRLE(image->type)) {
	free(image->rowstart);
	image->rowstart = 0;
	free(image->rowsize);
	image->rowsize = 0;
    }
    return(close(image->file));
}

int iflush(IMAGE *image)
{
    unsigned short *base;

    if ( (image->flags&_IOWRT)
     && (base=image->base)!=NULL && (image->ptr-base)>0) {
	    if (putrow(image, base, image->y,image->z)!=image->xsize) {
		    image->flags |= _IOERR;
		    return(EOF);
	    }
    }
    return(0);
}


/*
 *	putrow, getrow -
 *
 *				Paul Haeberli - 1984
 *
 */

int putrow(IMAGE *image, unsigned short *buffer,
		unsigned int y, unsigned int z) 
{
    register unsigned short 	*sptr;
    register unsigned char      *cptr;
    register unsigned int x;
    register unsigned int min, max;
    register int cnt;

    if( !(image->flags & (_IORW|_IOWRT)) )
	return -1;
    if(image->dim<3)
	z = 0;
    if(image->dim<2)
	y = 0;
    if(ISVERBATIM(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		cptr = (unsigned char *)image->tmpbuf;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    *cptr = *sptr++;
		    if (*cptr > max) max = *cptr;
		    if (*cptr < min) min = *cptr;
		    cptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize;
		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) 
		    return -1;
		else
		    return cnt;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		img_seek(image,y,z);
		cnt = image->xsize<<1;
		if(image->dorev)	
		    cvtshorts(buffer,cnt);
		if (img_write(image,(char *)(buffer),cnt) != cnt) {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return -1;
		} else {
		    if(image->dorev)	
			cvtshorts(buffer,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		i_errhdlr("putrow: weird bpp\n");
	}
    } else if(ISRLE(image->type)) {
	switch(BPP(image->type)) {
	    case 1: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
                    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,1,image->xsize);
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);

		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) 
		    return -1;
		else
		    return image->xsize;
		/* NOTREACHED */

	    case 2: 
		min = image->min;
		max = image->max;
		sptr = buffer;
		for(x=image->xsize; x--;) { 
		    if (*sptr > max) max = *sptr;
		    if (*sptr < min) min = *sptr;
		    sptr++;
		}
		image->min = min;
		image->max = max;
		cnt = img_rle_compact(buffer,2,image->tmpbuf,2,image->xsize);
		cnt <<= 1;
		img_setrowsize(image,cnt,y,z);
		img_seek(image,y,z);
		if(image->dorev)
		    cvtshorts(image->tmpbuf,cnt);
		if (img_write(image,(char *)(image->tmpbuf),cnt) != cnt) {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return -1;
		} else {
		    if(image->dorev)
			cvtshorts(image->tmpbuf,cnt);
		    return image->xsize;
		}
		/* NOTREACHED */

	    default:
		i_errhdlr("putrow: weird bpp\n");
	}
    } else 
	i_errhdlr("putrow: weird image type\n");
    return(-1);
}


/*
 *	img_seek, img_write, img_read, img_optseek -
 *
 *				Paul Haeberli - 1984
 *
 */

unsigned int img_seek(IMAGE *image, unsigned int y, unsigned int z)
{
    if(img_badrow(image,y,z)) {
	i_errhdlr("img_seek: row number out of range\n");
	return EOF;
    }
    image->x = 0;
    image->y = y;
    image->z = z;
    if(ISVERBATIM(image->type)) {
	switch(image->dim) {
	    case 1:
		return img_optseek(image, 512L);
	    case 2: 
		return img_optseek(image,512L+(y*image->xsize)*BPP(image->type));
	    case 3: 
		return img_optseek(image,
		    512L+(y*image->xsize+z*image->xsize*image->ysize)*
							BPP(image->type));
	    default:
		i_errhdlr("img_seek: weird dim\n");
		break;
	}
    } else if(ISRLE(image->type)) {
	switch(image->dim) {
	    case 1:
		return img_optseek(image, image->rowstart[0]);
	    case 2: 
		return img_optseek(image, image->rowstart[y]);
	    case 3: 
		return img_optseek(image, image->rowstart[y+z*image->ysize]);
	    default:
		i_errhdlr("img_seek: weird dim\n");
		break;
	}
    } else 
	i_errhdlr("img_seek: weird image type\n");
    return((unsigned int)-1);
}

int img_badrow(IMAGE *image, unsigned int y, unsigned int z)
{
    if(y>=image->ysize || z>=image->zsize)
	return 1;
    else
        return 0;
}

int img_write(IMAGE *image, char *buffer,int count)
{
    int retval;

    retval =  write(image->file,buffer,count);
    if(retval == count) 
	image->offset += count;
    else
	image->offset = -1;
    return retval;
}

int img_read(IMAGE *image, char *buffer, int count)
{
    int retval;

    retval =  read(image->file,buffer,count);
    if(retval == count) 
	image->offset += count;
    else
	image->offset = -1;
    return retval;
}

unsigned int img_optseek(IMAGE *image, unsigned int offset)
{
    if(image->offset != offset) {
       image->offset = offset;
       return ((unsigned int) lseek(image->file,offset,0));
   }
   return offset;
}

/*
 *	img_getrowsize, img_setrowsize, img_rle_compact, img_rle_expand -
 *
 *				Paul Haeberli - 1984
 *
 */

int img_getrowsize(IMAGE *image)
{
    switch(image->dim) {
	case 1:
	    return image->rowsize[0];
	case 2:
	    return image->rowsize[image->y];
	case 3:
	    return image->rowsize[image->y+image->z*image->ysize];
    }
}

void img_setrowsize(IMAGE *image, int cnt, int y, int z)
{
    int *sizeptr;

    if(img_badrow(image,y,z)) 
	return;
    switch(image->dim) {
	case 1:
	    sizeptr = &image->rowsize[0];
	    image->rowstart[0] = image->rleend;
	    break;
	case 2:
	    sizeptr = &image->rowsize[y];
	    image->rowstart[y] = image->rleend;
	    break;
	case 3:
	    sizeptr = &image->rowsize[y+z*image->ysize];
	    image->rowstart[y+z*image->ysize] = image->rleend;
    }	
    if(*sizeptr != -1) 
	image->wastebytes += *sizeptr;
    *sizeptr = cnt;
    image->rleend += cnt;
}

#define docompact 							\
	while(iptr<ibufend) {						\
	    sptr = iptr;						\
	    iptr += 2;							\
	    while((iptr<ibufend)&&((iptr[-2]!=iptr[-1])||(iptr[-1]!=iptr[0])))\
		iptr++;							\
	    iptr -= 2;							\
	    count = iptr-sptr;						\
	    while(count) {						\
		todo = count>126 ? 126:count; 				\
		count -= todo;						\
		*optr++ = 0x80|todo;					\
		while(todo--)						\
		    *optr++ = *sptr++;					\
	    }								\
	    sptr = iptr;						\
	    cc = *iptr++;						\
	    while( (iptr<ibufend) && (*iptr == cc) )			\
		iptr++;							\
	    count = iptr-sptr;						\
	    while(count) {						\
		todo = count>126 ? 126:count; 				\
		count -= todo;						\
		*optr++ = todo;						\
		*optr++ = cc;						\
	    }								\
	}								\
	*optr++ = 0;

int img_rle_compact(unsigned short *expbuf, int ibpp,
			unsigned short *rlebuf, int obpp, int cnt)
{
    if(ibpp == 1 && obpp == 1) {
	register unsigned char *iptr = (unsigned char *)expbuf;
	register unsigned char *ibufend = iptr+cnt;
	register unsigned char *sptr;
	register unsigned char *optr = (unsigned char *)rlebuf;
	register short todo, cc;
	register int count;

	docompact;
	return optr - (unsigned char *)rlebuf;
    } else if(ibpp == 1 && obpp == 2) {
	register unsigned char *iptr = (unsigned char *)expbuf;
	register unsigned char *ibufend = iptr+cnt;
	register unsigned char *sptr;
	register unsigned short *optr = rlebuf;
	register short todo, cc;
	register int count;

	docompact;
	return optr - rlebuf;
    } else if(ibpp == 2 && obpp == 1) {
	register unsigned short *iptr = expbuf;
	register unsigned short *ibufend = iptr+cnt;
	register unsigned short *sptr;
	register unsigned char *optr = (unsigned char *)rlebuf;
	register short todo, cc;
	register int count;

	docompact;
	return optr - (unsigned char *)rlebuf;
    } else if(ibpp == 2 && obpp == 2) {
	register unsigned short *iptr = expbuf;
	register unsigned short *ibufend = iptr+cnt;
	register unsigned short *sptr;
	register unsigned short *optr = rlebuf;
	register short todo, cc;
	register int count;

	docompact;
	return optr - rlebuf;
    } else  {
	i_errhdlr("rle_compact: bad bpp: %d %d\n",ibpp,obpp);
	return 0;
    }
}

#define doexpand				\
	while(1) {				\
	    pixel = *iptr++;			\
	    if ( !(count = (pixel & 0x7f)) )	\
		return;				\
	    if(pixel & 0x80) {			\
	       while(count--)			\
		    *optr++ = *iptr++;		\
	    } else {				\
	       pixel = *iptr++;			\
	       while(count--)			\
		    *optr++ = pixel;		\
	    }					\
	}

void img_rle_expand(unsigned short *rlebuf, int ibpp,
			unsigned short *expbuf, int obpp)
{
    if(ibpp == 1 && obpp == 1) {
	register unsigned char *iptr = (unsigned char *)rlebuf;
	register unsigned char *optr = (unsigned char *)expbuf;
	register unsigned short pixel,count;

	doexpand;
    } else if(ibpp == 1 && obpp == 2) {
	register unsigned char *iptr = (unsigned char *)rlebuf;
	register unsigned short *optr = expbuf;
	register unsigned short pixel,count;

	doexpand;
    } else if(ibpp == 2 && obpp == 1) {
	register unsigned short *iptr = rlebuf;
	register unsigned char  *optr = (unsigned char *)expbuf;
	register unsigned short pixel,count;

	doexpand;
    } else if(ibpp == 2 && obpp == 2) {
	register unsigned short *iptr = rlebuf;
	register unsigned short *optr = expbuf;
	register unsigned short pixel,count;

	doexpand;
    } else 
	i_errhdlr("rle_expand: bad bpp: %d %d\n",ibpp,obpp);
}


/*
 *	isetname and isetcolormap -
 *
 *				Paul Haeberli - 1984
 *
 */

void isetname(IMAGE *image, char *name)
{
    strncpy(image->name,name,80);
}

void isetcolormap(IMAGE *image, int colormap)
{
    image->colormap = colormap;
}

#ifdef CRAY
 
int cimg_write(IMAGE *image, char *buffer,int count)
{
  int retval, nitems;
  int tierr, ttype, tbitoff, ret_stat;
  int *intbuf;
  extern intsize, intsize;

   tbitoff = 0;  tierr = 0; ttype = 1;

   nitems = count/sizeof(int);

   intbuf = (int *)malloc(count);

   tierr = CRAY2IEG(&ttype, &nitems, intbuf, &tbitoff, buffer);

    write(image->file,intbuf,count*intsize/sizeof(int));
    image->offset += count*intsize/sizeof(int);
    return count*intsize/sizeof(int);
}

int cwrtshort(int f, short *shortptr, int nitems)
{
  int tierr, ttype, tbitoff, ret_stat;
  short *shortbuf;
  extern shortsize;

   tbitoff = 0;  tierr = 0; ttype = 7;

   shortbuf = (short *)malloc(sizeof(int)*nitems);
 
   tierr = CRAY2IEG(&ttype, &nitems, shortbuf, &tbitoff, shortptr);
   ret_stat = write(f, shortbuf, shortsize*nitems);
   free(shortbuf);
   if(tierr >= 0 && ret_stat == shortsize*nitems)
	return shortsize*nitems;
   else
        return 0;
}

int cwrtlint(int f, int *intptr, int nitems)
{
  int tierr, ttype, tbitoff, ret_stat;
  int *intbuf;
  extern intsize;

   tbitoff = 0;  tierr = 0; ttype = 1;

   intbuf = (int *)malloc(intsize*nitems);
 
   tierr = CRAY2IEG(&ttype, &nitems, intbuf, &tbitoff, intptr);
   ret_stat = write(f, intbuf, intsize*nitems);
   free(intbuf);
   if(tierr >= 0 && ret_stat == intsize*nitems)
	return intsize*nitems;
   else
        return 0;
}

#endif

#if defined(CRAY)

int crayimgwrite(int f, IMAGE *image, int count)
{
  extern shortsize, intsize;
  int zero = 0;
  int thou = 1000;
  int i;


/*
  Write out imagic, type, dim, xsize, ysize, zsize
*/
    if(cwrtshort(f, (short *)&image->imagic, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->type, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->dim, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->xsize, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->ysize, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->zsize, 1) != shortsize)
      return 0;

/*
  Write out min, max, wastebytes
*/
    if(cwrtlint(f, (int *)&image->min, 1) != intsize)
      return 0;
    if(cwrtlint(f, (int *)&image->max, 1) != intsize)
      return 0;
    if(cwrtlint(f, (int *)&image->wastebytes, 1) != intsize)
      return 0;

/*
  Write out name[80]
*/
    if(write(f, image->name, 80) != 80)
      return 0;


/*
  Write out colormap
*/
    if(cwrtlint(f, (int *)&image->colormap, 1) != intsize)
      return 0;

/*
  Write out file
*/
    if(cwrtlint(f, &image->file, 1) != intsize)
      return 0;

/*
  Write out flags, dorev, x, y, z, cnt
*/
    if(cwrtshort(f, (short *)&image->flags, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->dorev, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->x, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->y, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->z, 1) != shortsize)
      return 0;
    if(cwrtshort(f, (short *)&image->cnt, 1) != shortsize)
      return 0;

/*
  Write out *ptr, *base, *tmpbuf, offset, rleend, *rowstart, *rowsize
*/
    write(f, &zero, intsize);
    write(f, &zero, intsize);
    write(f, &zero, intsize);
    if(cwrtlint(f, (int *)&image->offset, 1) != intsize)
      return 0;
    if(cwrtlint(f, (int *)&image->rleend, 1) != intsize)
      return 0;
    write(f, &zero, intsize);
    write(f, &zero, intsize);

	return count;
}

#endif
