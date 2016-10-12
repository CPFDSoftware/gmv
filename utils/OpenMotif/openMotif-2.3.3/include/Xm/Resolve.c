/* $TOG: Resolve.c /main/7 1997/07/14 14:35:00 csn $ */
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "XmI.h"
#include <Xm/PrimitiveP.h>
#include <Xm/ManagerP.h>
#include <Xm/GadgetP.h>
#include <Xm/BaseClassP.h> /* for _XmIsSubclassOf */

#define _ALIGN(size) (((size) + (sizeof(long)-1)) & ~(sizeof(long)-1))

/********    Static Function Declarations    ********/

static void ResolveSyntheticOffsets(
			WidgetClass wc, 
			XmOffsetPtr * ipot, 
			XmOffsetPtr * cpot) ;

/********    End Static Function Declarations    ********/



/************************************************************************
 *
 *   XmeResolvePartOffsets
 *
 ************************************************************************/

 
#define IsConstraintClass(wc) _XmIsSubclassOf(wc, constraintWidgetClass)
/*
 *  end FIX for 5178.
 */

static void
ResolveSyntheticOffsets(
			WidgetClass wc, 
			XmOffsetPtr * ipot, 
			XmOffsetPtr * cpot)
{
    XmSyntheticResource* sr = NULL;
    Cardinal nsr = 0;     /* normal resources */
    XmSyntheticResource* scr = NULL;
    Cardinal nscr = 0;    /* constraint resources */
    Cardinal i;

    /* Get synthetic resource and synthetic constraint resource lists */

    if (_XmIsSubclassOf(wc, xmPrimitiveWidgetClass)) {
        XmPrimitiveWidgetClass pwc = (XmPrimitiveWidgetClass)wc;

        sr = pwc->primitive_class.syn_resources;
        nsr = pwc->primitive_class.num_syn_resources;
    }
    else if (_XmIsSubclassOf(wc, xmManagerWidgetClass)) {
        XmManagerWidgetClass mwc = (XmManagerWidgetClass)wc;

        sr = mwc->manager_class.syn_resources;
        nsr = mwc->manager_class.num_syn_resources;
        scr = mwc->manager_class.syn_constraint_resources;
        nscr = mwc->manager_class.num_syn_constraint_resources;
    }
    else if (_XmIsSubclassOf(wc, xmGadgetClass)) {
        XmGadgetClass gwc = (XmGadgetClass)wc;

        sr = gwc->gadget_class.syn_resources;
        nsr = gwc->gadget_class.num_syn_resources;
    } else {
	return;
    }

    /* Patch resource offsets using part offset tables */

    if (sr && nsr > 0 && ipot) {
        for (i = 0; i < nsr; i++)
            sr[i].resource_offset = XmGetPartOffset(&(sr[i]),ipot);
    }

    if (scr && nscr > 0 && cpot) {
        for (i = 0; i < nscr; i++)
            scr[i].resource_offset = XmGetPartOffset(&(scr[i]),cpot);
    }
}

void 
XmeResolvePartOffsets(
        WidgetClass w_class,
        XmOffsetPtr *offset,
        XmOffsetPtr *constraint_offset )
{
   WidgetClass c, super;
   ConstraintWidgetClass cc = NULL, scc = NULL;
   int i, classcount = 0;
   XmPartResource *pr;

   _XmProcessLock();
   super = w_class->core_class.superclass;
   /*
    *  Set up constraint class pointers
    */
   if (IsConstraintClass(super))
   {
        cc = (ConstraintWidgetClass)w_class;
	scc = (ConstraintWidgetClass)super;
   }

   /*
    *  Update the part size value (initially, it is the size of this part)
    */
   w_class->core_class.widget_size =
       _ALIGN(w_class->core_class.widget_size) + _ALIGN(super->core_class.widget_size);
   if (cc && scc)
       cc->constraint_class.constraint_size =
	   _ALIGN(cc->constraint_class.constraint_size) +
	       _ALIGN(scc->constraint_class.constraint_size);

   /*
    *  Count the number of superclasses and allocate the offset record(s)
    */
   for (c = w_class; c != NULL; c = c->core_class.superclass) classcount++;

   *offset = (XmOffsetPtr) XtMalloc(classcount * sizeof(XmOffset));

   if (cc)
   {
       if(constraint_offset != NULL) 
	   *constraint_offset = (XmOffsetPtr) XtMalloc(classcount 
						       * sizeof(XmOffset));
   }
   else 
   {
       if(constraint_offset != NULL) *constraint_offset = NULL;
   }
   
   /*
    *  Fill in the offset table(s) with the offset of all parts
    */
   for (i = classcount-1, c = super; i > 0; c = c->core_class.superclass, i--)
       (*offset)[i] = c->core_class.widget_size;

   (*offset)[0] = 0;

   if (constraint_offset != NULL && *constraint_offset != NULL) {
       for (i = classcount-1, scc = (ConstraintWidgetClass) super; i > 0; 
	    scc = (ConstraintWidgetClass)(scc->core_class.superclass), i--)
	   if (IsConstraintClass((WidgetClass)scc))
	       (*constraint_offset)[i] = 
		   _ALIGN(scc->constraint_class.constraint_size);
	   else
	       (*constraint_offset)[i] = 0;
	
       (*constraint_offset)[0] = 0;
   }

   /*
    *  Update the resource list(s) offsets in place
    */
   for (i = 0; i < w_class->core_class.num_resources; i++) 
   {
      pr = (XmPartResource *) &w_class->core_class.resources[i];

      /* The next line updates this in place--be careful */

      w_class->core_class.resources[i].resource_offset =
         XmGetPartOffset(pr, offset);
   }

   if (cc && constraint_offset)
       for (i = 0; i < cc->constraint_class.num_resources; i++) 
       {
          pr = (XmPartResource *) &cc->constraint_class.resources[i];

          /* The next line updates this in place--be careful */

          cc->constraint_class.resources[i].resource_offset =
             XmGetPartOffset(pr, constraint_offset);
       }

   /*
    * deal with the synthetic resources. CR 9021
    */

   ResolveSyntheticOffsets(w_class, offset, constraint_offset);
   _XmProcessUnlock();
}



void 
XmResolveAllPartOffsets(
        WidgetClass w_class,
        XmOffsetPtr *offset,
        XmOffsetPtr *constraint_offset )
{
    XmeResolvePartOffsets( w_class, offset, constraint_offset ) ;
}


void 
XmResolvePartOffsets(
        WidgetClass w_class,
        XmOffsetPtr *offset)
{
    XmeResolvePartOffsets( w_class, offset, NULL) ;
}
