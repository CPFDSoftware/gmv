/* $XConsortium: Print.h /main/5 1995/07/15 20:43:54 drk $ */
/*
 * @OPENGROUP_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 * ALL RIGHTS RESERVED (MOTIF).  See the file named COPYRIGHT.MOTIF for
 * the full copyright text.
 * 
 * This software is subject to an open license. It may only be
 * used on, with or for operating systems which are themselves open
 * source systems. You must contact The Open Group for a license
 * allowing distribution and sublicensing of this software on, with,
 * or for operating systems which are not Open Source programs.
 * 
 * See http://www.opengroup.org/openmotif/license for full
 * details of the license agreement. Any use, reproduction, or
 * distribution of the program constitutes recipient's acceptance of
 * this agreement.
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 * PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 * WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 * OR FITNESS FOR A PARTICULAR PURPOSE
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 * NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 * EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 * 
 */
/*
 * HISTORY
 */

/* Ensure that the file be included only once. */
#ifndef _XmdPrint_h
#define _XmdPrint_h


/* Include appropriate files. */
#include <Xm/Xm.h> /* widget public header file for XmManager */

/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif

/* Define the widget class and widget record. */
externalref WidgetClass xmdPrintWidgetClass;

typedef struct _XmdPrintClassRec * XmdPrintWidgetClass;
typedef struct _XmdPrintRec      * XmdPrintWidget;

/* Define an IsSubclass macro. */
#ifndef XmdIsPrint
#define XmdIsPrint(w) XtIsSubclass(w, XmdPrintWidgetClass)
#endif

#define XmdNprinterList		"printerList"
#define XmdCPrinterList		"PrinterList"
#define XmdNprintAll		"printAll"
#define XmdCPrintAll		"PrintAll"
#define XmdNtwoSided		"twoSided"
#define XmdCTwoSided		"TwoSided"
#define XmdNreverseOrder	"reverseOrder"
#define XmdCReverseOrder	"ReverseOrder"
#define XmdNfromPage		"fromPage"
#define XmdCFromPage		"FromPage"
#define XmdNtoPage		"toPage"
#define XmdCToPage		"ToPage"
#define XmdNnumberOfCopies	"numberOfCopies"
#define XmdCNumberOfCopies	"NumberOfCopies"
#define XmdNprintCallback	"printCallback"

/* Print callback */
typedef struct _XmdPrintCallbackStruct {
  int		reason; 	/* Always XmCR_OK */
  XEvent	*event;		/* Always NULL */
  Boolean	two_sided;	/* True if should print double sided */
  Boolean	reverse_order;	/* True if should reverse order */
  Boolean	to_printer;	/* True if output should go to printer */
  int		first;		/* First page to print.  If first==last==0 */
  int		last;		/* then print all pages */
  int		copies;		/* how many copies to print */
  char*		printer;	/* Name of printer or path of file */
} XmdPrintCallbackStruct;

/* Specify the API for this widget. */
extern Widget XmdCreatePrint(
			Widget parent,
			char   *name,
			ArgList arglist,
			Cardinal argcount);
extern Widget XmdCreatePrintDialog(
			Widget parent,
			char   *name,
			ArgList arglist,
			Cardinal argcount);
extern void XmdPrintDocument(char* pathname,
			     XmdPrintCallbackStruct* cb);

/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _XmdPrint_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */

