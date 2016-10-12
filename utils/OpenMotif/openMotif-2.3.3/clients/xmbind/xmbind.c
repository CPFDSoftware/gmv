/* 
 *  @OPENGROUP_COPYRIGHT@
 *  COPYRIGHT NOTICE
 *  Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 *  Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 *  ALL RIGHTS RESERVED (MOTIF). See the file named COPYRIGHT.MOTIF for
 *  the full copyright text.
 *  
 *  This software is subject to an open license. It may only be
 *  used on, with or for operating systems which are themselves open
 *  source systems. You must contact The Open Group for a license
 *  allowing distribution and sublicensing of this software on, with,
 *  or for operating systems which are not Open Source programs.
 *  
 *  See http://www.opengroup.org/openmotif/license for full
 *  details of the license agreement. Any use, reproduction, or
 *  distribution of the program constitutes recipient's acceptance of
 *  this agreement.
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 *  PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 *  WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 *  OR FITNESS FOR A PARTICULAR PURPOSE
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 *  NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 *  EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGES.
*/ 
/* 
 * HISTORY
*/ 
#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$TOG: xmbind.c /main/10 1997/06/18 17:34:48 samborn $"
#endif
#endif
#include <stdio.h>
#include <Xm/Xm.h>

/* Internal routines "borrowed" from libXm.  Don't try this at home! */
extern Boolean _XmVirtKeysLoadFileBindings(char *fileName, String *binding);
extern int _XmVirtKeysLoadFallbackBindings(Display *display, String *binding);

int main(argc, argv)
    int argc;
    char **argv;
{
    enum { XA_MOTIF_DEFAULT_BINDINGS, XA_MOTIF_BINDINGS };
    static char *atom_names[] = {
      "_MOTIF_DEFAULT_BINDINGS", "_MOTIF_BINDINGS" };

    Atom atoms[XtNumber(atom_names)];
    XtAppContext  app_context;
    Display *display;
    String bindings = NULL;

    XtToolkitInitialize();
    app_context = XtCreateApplicationContext();
    display = XtOpenDisplay(app_context, NULL, argv[0], "Xmbind",
			NULL, 0, &argc, argv);
    
    if (display == NULL) {
	fprintf(stderr, "%s:  Can't open display\n", argv[0]);
	exit(1);
    }

    XInternAtoms(display, atom_names, XtNumber(atom_names), False, atoms);
    if (argc == 2) {
	if (_XmVirtKeysLoadFileBindings (argv[1], &bindings) == True) {
	    XDeleteProperty (display, RootWindow (display, 0),
			     atoms[XA_MOTIF_DEFAULT_BINDINGS]);
	    XChangeProperty (display, RootWindow(display, 0),
			     atoms[XA_MOTIF_BINDINGS],
			     XA_STRING, 8, PropModeReplace,
			     (unsigned char *)bindings, strlen(bindings));
	    XFlush (display);
	    XtFree (bindings);
	    exit(0);
	}
	else {
	    fprintf(stderr, "%s:  Can't open %s\n", argv[0], argv[1]);
	    exit(1);
	}
    }

    XDeleteProperty (display, RootWindow (display, 0),
		     atoms[XA_MOTIF_BINDINGS]);
    XDeleteProperty (display, RootWindow (display, 0),
		     atoms[XA_MOTIF_DEFAULT_BINDINGS]);

    _XmVirtKeysLoadFallbackBindings (display, &bindings);

    XFlush (display);
    XtFree (bindings);
   
    exit(0);

    return 0;    /* make compiler happy */
}

