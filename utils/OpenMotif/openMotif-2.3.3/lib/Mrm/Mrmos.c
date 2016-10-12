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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$XConsortium: Mrmos.c /main/7 1996/11/13 14:03:33 drk $"
#endif
#endif

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C" { /* some 'locale.h' do not have prototypes (sun) */
#endif
#include <X11/Xlocale.h>
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration */
#endif /* __cplusplus */

#include <Xm/Xm.h>	/* For _NO_PROTO declaration. */
#include "MrmosI.h"

/************************************************************
 *
 * The following routines are used for portable uid files to convert
 * between the host's native format for floats and IEEE format floats.
 * Type SINGLE_FLOAT in a uid file should be stored as a 32 bit IEEE float
 * Type FLOAT in a uid file should be stored as a 64 bit IEEE double
 * The routines need support for the VAX architecture, maybe others.
 *
 ************************************************************/

/*ARGSUSED*/
void _MrmOSHostFloatToIEEE(float *val) /* unused */
{
  /*EMPTY*/
}

/*ARGSUSED*/
void _MrmOSHostDoubleToIEEE(double *val) /* unused */
{
  /*EMPTY*/
}

/*ARGSUSED*/
void _MrmOSIEEEFloatToHost(float *val) /* unused */
{
  /*EMPTY*/
}


/*ARGSUSED*/
void _MrmOSIEEEDoubleToHost(double *val)	/* unused */
{
  /*EMPTY*/
}

/************************************************************************
 *  
 *    _MrmOSSetLocale   wrapper so vendor can disable call to set 
 *                    if locale is superset of "C".
 *  
 ************************************************************************/

String
_MrmOSSetLocale(String locale)
{
  return setlocale(LC_ALL, locale);
}
