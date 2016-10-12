/* $XConsortium: airport.h /main/5 1995/07/15 20:44:29 drk $ */
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
 *    Airport Data Structure
 *
 *
 *    Created: Vania JOLOBOFF / May 1992
 *
 *
 * ======================================================================
 * 
 */

/*
 * ===================== include ========================================
 */

#if (! defined airport_h)
#define airport_h 

#include <stdlib.h>
#include <stdio.h>
#include <Xm/XmAll.h>

/* ======================================================================
 *  General Types
 */

typedef struct _Source {
   Widget departure;
   Widget over_ocean;
   Widget over_land;
   Widget over_site;
   Widget landing;
   Widget landed;
} SourceRec, *Source;

typedef struct _Destination {
   Widget enter;
   Widget leave;
   Widget landing;
   Widget parked;
   int gate;
} DestinationRec, *Destination;

typedef struct _Spot {
   Boolean empty;
   Position x,y;
} SpotRec, *Spot;

typedef enum {empty, normal, full} ParkState ;
 
typedef struct _Park {
   ParkState state;
   GC gc;
   int spot_count;
   int plane_count;
   Spot spots; /* variable length array of spots */
} ParkRec, *Park;

typedef struct _Track {
   int borderwidth;
   XRectangle bbox;
   GC gc;
} TrackRec, *Track;

typedef enum {closed, open} AirportState ;

/*
 * The airport data structure consists of
 * 	- the widget needed in various callbacks 
 *	- the drag source data
 *	- the drop site data
 *	- the details of the track graphics 
 */

typedef struct _Airport {
   XtAppContext context; 
   Display * display;
   Window ocean;
   int screen_width;
   int screen_height;
   AirportState state;
   Widget main; /* the main window */
   Widget airport; /* the airport drawing area */
   Widget help_dialog;
   Widget error_dialog;
   Widget warning_dialog;
   Widget dragIcon;
   Widget msg_area;
   SourceRec from; /* the drag data */
   DestinationRec to; /* drop site data */
   ParkRec park; /* info on available gates at the airport */
   TrackRec track; /* info on track graphics */
} AirportRec, *Airport;

/*
 * A flight has a flight number. It starts from some airport (from)
 * When landing, it will be taxi-ed to some gate.
 */

typedef struct _Flight {
   long number;
   int gate;     /* the gate it's going to be taxi-ed */
   Airport from; /* the airport it took off */
} FlightRec, *Flight;

/* ======================================================================
 * Global constants
 */

#define plane_width 25
#define plane_height 25

/* ======================================================================
 * Global variables 
 */

#ifdef DECLAREGLOBAL
#define global
#else
#define global extern
#endif

global Atom FlightAtom;
global Atom DeleteAtom;

/* 
 * Application Resources
 */

global struct _AirportResources {
   String too_small;
   Pixel spot_background;
   Pixel spot_foreground;
   Pixel flight_background;
   Pixel flight_foreground;
   Pixel invalid_foreground;
   Pixel valid_foreground;
   Pixel none_foreground;
   Pixel track_background;
   Pixel track_foreground;
   Pixel drag_under_background;
   Pixel drag_under_foreground;
   Dimension track_border_width;
   Dimension track_margin;
   int bell_duration;
   int bell_percent;
} AirportResources;

global void DrawAirport(Airport this, Window wd, Pixel bg, Pixel fg);
/* draw the tracks and parked planes */

global void Paging(Airport this, String msg);
/* display a message in message area */


#endif /* airport_h */
