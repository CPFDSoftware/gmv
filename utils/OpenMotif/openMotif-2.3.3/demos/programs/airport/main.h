/* $TOG: main.h /main/5 1997/03/31 13:46:32 dbl $ */
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

/*
 * The Airport Drag And Drop Demo
 *
 *    Airport Main Header File
 *
 *
 * The airport demo is a totally useless program to illustrate
 * Drag and Drop in Motif 1.2.
 * It consists of an airport containing 
 *	- plane icons that can be dragged
 *	- tracks where planes can be dropped (land)
 *	- a radio box that shows current state of outcoming flights
 *	- a raido box that shows current state of incoming flights
 *	- a message area that lists successive interesting messages.
 *	
 * The demo is more interesting when two instances of this program
 * are run simultaneously.
 *
 * Each time a plane is dragged, a flight number is randomly created
 * and the plane is announced to leave in the message area.
 *
 * Each time a plane is dropped onto an airport, if a gate is available,
 * the plane is taxi-ed to this gate. The data transferred between the 
 * source airport and the destination airport is the flight number.
 * The arriving flight is announced in the message area.
 * If the destination airport is saturated (no gate available) the drop
 * is rejected.
 * If the data transfer fails because the network is down or the source
 * client died, or whatever reason, the plane is announced to have crashed.
 *
 *
 *    Created: Vania JOLOBOFF / May 1992
 * ======================================================================
 * 
 */

#define DECLAREGLOBAL

#include "airport.h"
#include "dragsource.h"
#include "dropsite.h"

#include <Xm/XmAll.h>

/* ======================================================================
 *  Constants
 */

#define MIN_TRACK_SIZE	16 

/*
 * The flying plane icon
 */

#define flying_icon_width 30
#define flying_icon_height 31
static unsigned char flying_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
   0x00, 0x70, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
   0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xe0, 0x01, 0x00,
   0x00, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x07, 0x00,
   0x00, 0xc0, 0x0f, 0x00, 0xf8, 0xff, 0xff, 0x0f, 0xfc, 0xff, 0xff, 0x1f,
   0xfe, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xff, 0x0f,
   0x00, 0xc0, 0x0f, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0xc0, 0x03, 0x00,
   0x00, 0xc0, 0x03, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xe0, 0x01, 0x00,
   0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
   0x00, 0x70, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00};

#define flying_mask_width 30
#define flying_mask_height 31
static unsigned char flying_mask_bits[] = {
   0x00, 0x38, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00,
   0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0xf0, 0x03, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x0f, 0x00,
   0xf8, 0xff, 0xff, 0x0f, 0xfc, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x3f,
   0xff, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x1f,
   0xf8, 0xff, 0xff, 0x0f, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0xf0, 0x03, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x00, 0xf8, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00,
   0x00, 0x38, 0x00, 0x00};

/*
 * The parked plane icon
 */

static unsigned char plane_bits[] = {
   0x07, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
   0x7e, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x01,
   0xf8, 0xff, 0xff, 0x01, 0xf0, 0xff, 0xff, 0x01, 0xe0, 0xff, 0xff, 0x00,
   0xe0, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0xe0, 0xff, 0x00, 0x00,
   0xe0, 0xff, 0x00, 0x00, 0xe0, 0xff, 0x01, 0x00, 0xe0, 0xff, 0x03, 0x00,
   0xe0, 0xff, 0x07, 0x00, 0xe0, 0xe7, 0x0f, 0x00, 0xe0, 0xc7, 0x1f, 0x00,
   0xe0, 0x87, 0x3f, 0x00, 0xe0, 0x07, 0x7f, 0x00, 0xe0, 0x03, 0xfe, 0x00,
   0xe0, 0x03, 0xfc, 0x01, 0xe0, 0x03, 0xf8, 0x01, 0xe0, 0x01, 0xf0, 0x01,
   0xe0, 0x00, 0xe0, 0x01};

static XtResource AirportResourcesSpecs[] =
{
   { "tooSmall", XtCString, XtRString, sizeof(String),
       XtOffsetOf( struct _AirportResources, too_small), XtRImmediate, 
       (XtPointer) "Window too small, enlarge please" },
   { 
      "trackBackground", XmCBackground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, track_background),
      XtRString, (XtPointer)"XtDefaultBackground"
     },
   {
      "spotBackground", XmCBackground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, spot_background),
      XtRString, (XtPointer)"XtDefaultBackground"
     },
   {	
      "flyingBackground", XmCBackground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, flight_background),
      XtRString, (XtPointer)"XtDefaultBackground"
  },
   {
      "dragUnderBackground", XmCBackground, XmRPixel, sizeof(Pixel),
      XtOffsetOf( struct _AirportResources, drag_under_background),
      XtRString, (XtPointer)"XtDefaultForeground"
    },
   {
      "trackForeground", XmCForeground, XmRPixel, sizeof(Pixel),
      XtOffsetOf( struct _AirportResources, track_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "dragUnderForeground", XmCForeground, XmRPixel, sizeof(Pixel),
      XtOffsetOf( struct _AirportResources, drag_under_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "spotForeground", XmCForeground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, spot_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "flyingForeground", XmCForeground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, flight_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "invalidForeground", XmCForeground, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, invalid_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "validForeground", XmCHighlightColor, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, valid_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
      "noneForeground", XmCHighlightColor, XmRPixel, sizeof (Pixel),
      XtOffsetOf( struct _AirportResources, none_foreground),
      XtRString,(XtPointer)"XtDefaultForeground"
    },
   {
     "trackBorderWidth", XmCBorderWidth, XmRDimension, sizeof(Dimension),
     XtOffsetOf( struct _AirportResources, track_border_width),
     XmRImmediate, (XtPointer) 5
    },
   {
      "trackMargin", XmCDimension, XmRDimension, sizeof(Dimension),
      XtOffsetOf( struct _AirportResources, track_margin),
      XmRImmediate, (XtPointer)  16
   },
   {
      "bellDuration", XmCMaxValue, XmRInt, sizeof(int),
      XtOffsetOf( struct _AirportResources, bell_duration),
      XmRImmediate, (XtPointer)  100
   },
   {
      "bellPercent", XmCMaxValue, XmRInt, sizeof(int),
      XtOffsetOf( struct _AirportResources, bell_percent),
      XmRImmediate, (XtPointer) 100
   }
};
