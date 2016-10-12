#include <X11/Intrinsic.h>
#include <Xm/DrawUtils.h>

#define STATIC_RECTS 20
/*
 * Function:
 *	XmDrawBevel(dpy, d, top_GC, bottom_GC, x, y, size, option)
 * Description:
 *	Draws a shadow corner (beveled) at the given location and size.
 * Input:
 *	dpy       : Display*     - the display to draw to
 *	d         : Drawable     - the drawable to use
 *	top_GC    : GC           - the GC to use to draw the top half of
 *                                 the bevel
 *	bottom_GC : GC           - the GC to use to draw the bottom half
 *                                 of the bevel
 *	x         : int          - the x location of the corner
 *	y         : int          - the y location of the corner
 *	size      : unsigned int - the size of the corner
 *                                 (width = height = size)
 *	option    : XmBevelOption- what part of the bevel should we draw.
 * Output:
 *	None.
 */
void
#ifndef _NO_PROTO
XmDrawBevel(Display *dpy, Drawable d, GC top_gc, GC bottom_gc,
	    int x, int y, unsigned int size, XmBevelOption option)
#else
XmDrawBevel(dpy, d, top_gc, bottom_gc, x, y, size, option)
    Display       *dpy;
    Drawable      d;
    GC            top_gc, bottom_gc;
    int           x, y;
    unsigned int  size;
    XmBevelOption option;
#endif
{
    static     XRectangle saved[STATIC_RECTS], *alloced = NULL;
    static int numAlloced = 0;
    XRectangle *rt;
    int        i;

    /*
     * First lets see if we can get away with using our list rectangles
     * without allocating any.
     */
    if( size < STATIC_RECTS )
    {
	/*
	 * OK we don't need to allocate any so lets use the static
	 * array.
	 */
	rt = saved;
    }
    else
    {
	/*
	 * Well we need more than our static array holds so lets see
	 * if we have enough in our alloced array and if no lets
	 * allocate what we need.
	 */
	if( size > numAlloced )
	{
	    numAlloced = size;
	    alloced = (XRectangle*) XtRealloc((XtPointer) alloced,
					      sizeof(XRectangle) * numAlloced);
	}
	rt = alloced;
    }

    /*
     * Now that we have enough rectangles to fill in an area lets
     * set up the rectangles and pass them off to be drawn.  First the
     * top half of the beveled corner ...
     */
    if( option == XmBEVEL_TOP )
    {
	for( i = 0; i < size; ++i )
	{
	    rt[i].x = x;
	    rt[i].y = y + i;
	    rt[i].width = size - i;
	    rt[i].height = 1;
	}
	XFillRectangles(dpy, d, top_gc, rt, size);
    }
    else if( option == XmBEVEL_BOTH )
    {
	XFillRectangle(dpy, d, top_gc, x, y, size, size);
    }

    /*
     * ... And the the bottom half of the beveled corner.
     */
    if( option == XmBEVEL_BOTH || option == XmBEVEL_BOTTOM )
    {
	for( i = 0; i < size; ++i )
	{
	    rt[i].x = x + size - i;
	    rt[i].y = y + i;
	    rt[i].width = i;
	    rt[i].height = 1;
	}
	XFillRectangles(dpy, d, bottom_gc, rt, size);
    }
}

