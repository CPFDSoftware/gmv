#include "JpegI.h"
#include <jerror.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>

void
_XmJpegErrorExit(j_common_ptr cinfo)
{
    int rc;
    XmJpegErrorMgr err = (XmJpegErrorMgr) cinfo->err;

    switch (cinfo->err->msg_code) {
    case JERR_NO_SOI:
        rc = 1;
        break;
    case JERR_OUT_OF_MEMORY:
        rc = 4;
        break;
    default:
        rc = 2;
        break;
    }
    longjmp(err->setjmp_buffer, rc);
}

int
load_jpeg(FILE * infile, unsigned long *pWidth, unsigned long *pHeight,
          CTable ** image_data)
{
    CTable *buf;
    struct jpeg_decompress_struct cinfo;
    XmJpegErrorMgrRec jerr;
    JSAMPROW row_pointer[1];
    int x, y;
    int rc;

    *image_data = NULL;
    cinfo.err = jpeg_std_error((struct jpeg_error_mgr *) &jerr);
    jerr.pub.error_exit = _XmJpegErrorExit;
    if ((rc = setjmp(jerr.setjmp_buffer))) {
        jpeg_destroy_decompress(&cinfo);
        return rc;
    }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);
    *pWidth = cinfo.output_width;
    *pHeight = cinfo.output_height;
    *image_data =
        malloc(cinfo.output_width * cinfo.output_height * sizeof(CTable));
    for (buf = *image_data;
         cinfo.output_scanline < cinfo.output_height;
         buf += cinfo.output_width)
        jpeg_read_scanlines(&cinfo, (JSAMPARRAY) (&buf), 1);
    if (cinfo.out_color_space == JCS_GRAYSCALE) {
        for (y = 0, buf = *image_data; y < cinfo.output_height;
             y++, buf += cinfo.output_width)
            for (x = cinfo.output_width - 1; x >= 0; x--)
                buf[x].red = buf[x].green = buf[x].blue =
                    ((JSAMPLE *) buf)[x];
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return 0;
}

Pixel
get_cval(unsigned char c, unsigned long mask)
{
    Pixel value = c, x;
    int i;
    for (i = 0, x = 1; i < 32; i++, x <<= 1)
        if (mask & x)
            break;
    for (; i < 32; i++, x <<= 1)
        if (!(mask & x))
            break;
    if (i < 8)
        value >>= 8 - i;
    else if (i > 8)
        value <<= i - 8;
    return (value & mask);
}

void
store_pixel(Screen * screen, CTable * p, int x, char *cdata)
{
    Pixel px = get_cval(p->red, screen->root_visual->red_mask)
        | get_cval(p->green, screen->root_visual->green_mask)
        | get_cval(p->blue, screen->root_visual->blue_mask);
    if (screen->root_depth <= 16) {
        if (ImageByteOrder(screen->display) == MSBFirst) {
            cdata[x * 2] = (px >> 8);
            cdata[x * 2 + 1] = (px & 0xff);
        } else {
            cdata[x * 2] = (px & 0xff);
            cdata[x * 2 + 1] = (px >> 8);
        }
    } else {
        if (ImageByteOrder(screen->display) == MSBFirst) {
            cdata[x * 4] = (px >> 24);
            cdata[x * 4 + 1] = (px >> 16);
            cdata[x * 4 + 2] = (px >> 8);
            cdata[x * 4 + 3] = (px & 0xff);
        } else {
            cdata[x * 4 + 3] = (px >> 24);
            cdata[x * 4 + 2] = (px >> 16);
            cdata[x * 4 + 1] = (px >> 8);
            cdata[x * 4] = (px & 0xff);
        }
    }
}

int
_XmJpegGetImage(Screen * screen, FILE * infile, XImage ** ximage)
{
    unsigned long image_width, image_height;
    unsigned char *xdata;
    int pad;
    CTable *image_data;
    int rc;

    if ((rc = load_jpeg(infile, &image_width, &image_height, &image_data)))
        return rc;
    if (screen->root_depth == 24 || screen->root_depth == 32) {
        xdata = (unsigned char *) malloc(4 * image_width * image_height);
        pad = 32;
    } else if (screen->root_depth == 16) {
        xdata = (unsigned char *) malloc(2 * image_width * image_height);
        pad = 16;
    } else {                    /* depth == 8 */
        xdata = (unsigned char *) malloc(image_width * image_height);
        pad = 8;
    }

    if (!xdata) 
        return 4;

    *ximage =
        XCreateImage(screen->display, screen->root_visual,
                     screen->root_depth, ZPixmap, 0, (char *) xdata,
                     image_width, image_height, pad, 0);
    if (!*ximage) {
        free(xdata);
        return 4;
    }

    {
        int xx, yy;
        CTable *p;
        for (yy = 0; yy < (*ximage)->height; yy++) {
            p = image_data + yy * (*ximage)->width;
            for (xx = 0; xx < (*ximage)->width; xx++, p++)
                store_pixel(screen, p, xx + yy * (*ximage)->width,
                            (*ximage)->data);
        }
    }

    if (image_data) {
        free(image_data);
        image_data = NULL;
    }
    return 0;
}
