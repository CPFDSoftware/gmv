/*
 * writeRGB()
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IMAGIC 	0732

#define BPPMASK			0x00ff
#define ITYPE_VERBATIM		0x0000
#define ITYPE_RLE		0x0100
#define ISRLE(type)		(((type) & 0xff00) == ITYPE_RLE)
#define ISVERBATIM(type)	(((type) & 0xff00) == ITYPE_VERBATIM)
#define BPP(type)		((type) & BPPMASK)
#define RLE(bpp)		(ITYPE_RLE | (bpp))
#define VERBATIM(bpp)		(ITYPE_VERBATIM | (bpp))

typedef unsigned short u_shrt;
typedef unsigned long u_lng;

struct {
    u_shrt	imagic;		/* stuff saved on disk . . */
    u_shrt 	type;
    u_shrt 	dim;
    u_shrt 	xsize;
    u_shrt 	ysize;
    u_shrt 	zsize;
    u_lng 	min;
    u_lng 	max;
    u_lng	wastebytes;	
    char 	name[80];
    u_lng	colormap;

    long 	file;		/* stuff used in core only */
    u_shrt 	flags;
    short	dorev;
    short	x;
    short	y;
    short	z;
    short	cnt;
    u_shrt	*ptr;
    u_shrt	*base;
    u_shrt	*tmpbuf;
    u_lng	offset;
    u_lng	rleend;		/* for rle images */
    u_lng	*rowstart;	/* for rle images */
    long	        *rowsize;	/* for rle images */
} img;


#define TAGLEN	(5)

#define RINTLUM (79)
#define GINTLUM (156)
#define BINTLUM (21)

#define OFFSET_R	3	/* this is byte order dependent */
#define OFFSET_G	2
#define OFFSET_B	1
#define OFFSET_A	0

#define ILUM(r,g,b)     ((int)(RINTLUM*(r)+GINTLUM*(g)+BINTLUM*(b))>>8)
#define CHANOFFSET(z)	(3-(z))	/* this is byte order dependent */

typedef unsigned char byte;

static void lumrow(byte *rgbptr, byte *lumptr, int n);
static int  compressrow(byte *lbuf, byte *rlebuf, int z, int cnt);
static void writetab(FILE *outf, u_lng *tab, int n);

static void putshort(FILE *outf, u_shrt val);
static void putlong(FILE *outf, u_lng val);
void zeroit(char *s, size_t len);



/*************************************************/
/* IRIS image-writing routines                   */
/*************************************************/



/*************************************************/
int writeRGB(FILE *fp, byte *pic, int w, int h)
{
  /*  writes 24-bit RGB IRIS file to the   */
  /*  already open stream, rle compressed  */

  /* IMAGE img; */
  int     i, j, k, pos, len, tablen, rlebuflen, zsize;
  u_lng *starttab, *lengthtab;
  byte   *rlebuf, *pptr;
  byte   *lumbuf, *lptr;

  zeroit((char *) &img, sizeof(img));
  
  /* write header information */
  fwrite(&img, sizeof(img), (size_t) 1, fp);
  fseek(fp, 0L, 0);

  /* load up header */
  img.imagic = IMAGIC;
  img.type   = ITYPE_RLE | (1 & BPPMASK);   /* RLE, 1 byteperpix */
  img.dim    = 3;
  img.xsize  = w;
  img.ysize  = h;
  img.zsize  = zsize = 3;
  img.min    = 0;
  img.max    = 255;

  putshort(fp, img.imagic);
  putshort(fp, img.type);
  putshort(fp, img.dim);
  putshort(fp, img.xsize);
  putshort(fp, img.ysize);
  putshort(fp, img.zsize);
  putlong (fp, img.min);
  putlong (fp, img.max);
  putlong (fp, 0L);
  fwrite  ("no name", (size_t) 8, (size_t) 1,fp);

  if (ferror(fp)) { fclose(fp);  return -1; }

  /* allocate RLE compression tables & stuff */
  rlebuflen = 2*w + 10;
  tablen    = h * zsize;

  starttab  = (u_lng *) malloc((size_t) tablen * sizeof(long));
  lengthtab = (u_lng *) malloc((size_t) tablen * sizeof(long));
  rlebuf    = (byte *)   malloc((size_t) rlebuflen);
  lumbuf    = (byte *)   malloc((size_t) w * 4);

  if (!starttab || !lengthtab || !rlebuf || !lumbuf) 
    memerr();

  pos = 512 + 2 * (tablen * 4);
  fseek(fp, (long) pos, 0);

  /* compress and write the data */
  lptr = pic;
  for (i=0; i<h; i++) {
    for (j=0; j<zsize; j++) {
      if (zsize == 1) {
	lumrow(lptr, lumbuf, w);
	len = compressrow(lumbuf, rlebuf, CHANOFFSET(j), w);
      } 
      else {
	len = compressrow(lptr, rlebuf, CHANOFFSET(j), w);
      }

      if (len > rlebuflen) {
	printf("writeRGB: rlebuf is too small\n");
	return -1;
      }

      fwrite(rlebuf, (size_t) len, (size_t) 1, fp);
      starttab [i + j*h] = pos;
      lengthtab[i + j*h] = len;
      pos += len;
    }
    lptr += (w*4);
  }

  /* write out line start and length tables */
  fseek   (fp, 512L, 0);
  writetab(fp, starttab, tablen);
  writetab(fp, lengthtab,tablen);

  free(starttab);
  free(lengthtab);
  free(rlebuf);
  free(lumbuf);

  if (ferror(fp)) return -1;

  return 0;
}
  

/*************************************/
static void lumrow(byte *rgbptr, byte *lumptr, int n) 
{
  lumptr += CHANOFFSET(0);
  while (n--) {
    *lumptr = ILUM(rgbptr[OFFSET_R],rgbptr[OFFSET_G],rgbptr[OFFSET_B]);
    lumptr += 4;
    rgbptr += 4;
  }
}


/*************************************/
static int compressrow(byte *lbuf, byte *rlebuf, int z, int cnt)
{
  byte *iptr, *ibufend, *sptr, *optr;
  short todo, cc;							
  long  count;

  lbuf    += z;
  iptr    = lbuf;
  ibufend = iptr+cnt*4;
  optr    = rlebuf;

  while (iptr<ibufend) {
    sptr = iptr;
    iptr += 8;
    while ((iptr<ibufend) && ((iptr[-8]!=iptr[-4])||(iptr[-4]!=iptr[0])))
      iptr += 4;
    iptr -= 8;

    count = (iptr-sptr)/4;

    while (count) {
      todo = count>126 ? 126 : count;
      count -= todo;
      *optr++ = 0x80|todo;
      while (todo>8) {
	optr[0] = sptr[0*4];
	optr[1] = sptr[1*4];
	optr[2] = sptr[2*4];
	optr[3] = sptr[3*4];
	optr[4] = sptr[4*4];
	optr[5] = sptr[5*4];
	optr[6] = sptr[6*4];
	optr[7] = sptr[7*4];
	optr += 8;
	sptr += 8*4;
	todo -= 8;
      }

      while (todo--) {
	*optr++ = *sptr;
	sptr += 4;
      }
    }

    sptr = iptr;
    cc = *iptr;
    iptr += 4;
    while ((iptr<ibufend) && (*iptr == cc))  iptr += 4;
    
    count = (iptr-sptr)/4;
    while (count) {
      todo = count>126 ? 126:count;
      count -= todo;
      *optr++ = todo;
      *optr++ = cc;
    }
  }
  
  *optr++ = 0;
  return (optr - rlebuf);
}


/****************************************************/
static void writetab(FILE *outf, u_lng *tab, int n)
{
  while (n) {
    putlong(outf,*tab++);
    n--;
  }
}




/* byte order independent write of shorts and longs. */

static void putshort(FILE *outf, u_shrt val)
{
  byte buf[2];
  buf[0] = (val>>8);
  buf[1] = (val>>0);
  fwrite(buf,(size_t) 2,(size_t) 1,outf);
}


static void putlong(FILE *outf,u_lng val)
{
  byte buf[4];
  buf[0] = (val>>24);
  buf[1] = (val>>16);
  buf[2] = (val>>8);
  buf[3] = (val>>0);
  fwrite(buf,(size_t) 4,(size_t) 1,outf);
}


void zeroit(char *s, size_t len)
{
  for ( ; len>0; len--) *s++ = 0;
}
