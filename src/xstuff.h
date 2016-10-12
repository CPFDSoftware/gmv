#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

EXTERN Widget toplevel, poplevel, glw, axglw, stglw, poplevel2;
EXTERN Colormap top_cmap, my_cmap;
EXTERN XColor mat_color;
EXTERN unsigned long  mat_pixels[MAXCONTOURS + 3];
EXTERN unsigned long  redpix, greenpix, bluepix, yellowpix,
                      whitepix, blackpix;
EXTERN Window topwin, winaxisid, windowid, winstid;
EXTERN Display *dpy;
EXTERN Cursor  crosshair, waitcsr;
EXTERN GC lpgc;
EXTERN XVisualInfo *vi;
EXTERN short mesacmap, truecolorcmap, beepflag;
