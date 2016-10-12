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
static char rcsid[] = "$XConsortium: XmStringObso.c /main/6 1995/09/19 23:13:52 cde-sun $"
#endif
#endif

#include "XmStringI.h"
#include "XmI.h"

/*
 * as close as we can come to Latin1Create without knowing the charset of
 * Latin1.  This imposes the semantic of \n meaning separator.
 */
XmString 
XmStringLtoRCreate(
        char *text,
        XmStringTag tag )
{
  char *start, *end;
  Boolean done;
  XmString string;

  _XmProcessLock();
  if (!text) {
	_XmProcessUnlock();
	return (NULL);
  }

  start = text;
  done = FALSE;

  /* Set the direction once only at the beginning. */
  string = XmStringDirectionCreate(XmSTRING_DIRECTION_L_TO_R);
    
  while ( ! done)		/* loop thu local copy */
    {				/* looking for \n */
      end = start;

      while ((*end != '\0') && (*end != '\n'))  end++;

      if (*end == '\0')
	done = TRUE;		/* we are at the end */

      /* Don't convert empty string unless it's an initial newline. */
      /* Done so StringHeight has clue to size of empty lines. */
      if ((start != end) || (start == text))
        string = XmStringConcatAndFree (string, 
					_XmStringNCreate(start, tag, 
							 end - start));
      
      /* Make a separator if this isn't the last segment. */
      if (!done) {
        string = XmStringConcatAndFree(string, XmStringSeparatorCreate());
	start = ++end;		/* start at next char */
      }
    }

  _XmProcessUnlock();
  return (string);
}

XmString 
XmStringCreateLtoR(
        char *text,
        XmStringTag tag )
{
  return (XmStringLtoRCreate (text, tag));
}


/*
 * build an external TCS 'segment', just a high level create
 */
XmString 
XmStringSegmentCreate(
        char *text,
        XmStringTag tag,
#if NeedWidePrototypes
        int direction,
        int separator )
#else
        XmStringDirection direction,
        Boolean separator )
#endif /* NeedWidePrototypes */
{
  XmString result;

  result = XmStringConcatAndFree (XmStringDirectionCreate (direction),
				  XmStringCreate (text, tag));

  if (separator)
    result = XmStringConcatAndFree (result, XmStringSeparatorCreate ());

  return result;
}

/*
 * Convenience routine to create an XmString from a NULL terminated string.
 */
XmString 
XmStringCreateSimple(
        char *text )
{
  return (XmStringCreate(text, XmSTRING_DEFAULT_CHARSET));
}
/*
 * concat two external strings.  Only concat a component at a time
 * so that we always wind up with a meaningful string
 */
XmString 
XmStringNConcat(XmString first,
		XmString second,
		int n )
{
  XmString	tmp, ret_val;
  
  _XmProcessLock();
  tmp = XmStringConcat(first, second);
  
  ret_val = XmStringNCopy(tmp, XmStringLength(first) + n);
  
  XmStringFree(tmp);
  
  _XmProcessUnlock();
  return(ret_val);
}

/*
 * Copy a compound string, such that the equivalent ASN.1 form
 * has <= n bytes.  Only copy a component at a time
 * so that we always wind up with a meaningful string
 */
XmString 
XmStringNCopy(
        XmString str,
        int n )
{
  unsigned char	*tmp;
  unsigned int	len;
  XmString	ret_val;
  
  _XmProcessLock();
  len = XmCvtXmStringToByteStream(str, &tmp);
  
  if (n >= len) /* No need to truncate */
    {
      ret_val = XmStringCopy(str);
    }
  else /* Truncate and convert */
    {
      tmp = _XmStringTruncateASN1(tmp, n);
      ret_val = XmCvtByteStreamToXmString(tmp);
    }
  
  XtFree((char *)tmp);
  
  _XmProcessUnlock();
  return(ret_val);
}

/* Compare ASN.1 form of strings. */
Boolean 
XmStringByteCompare(
        XmString a1,
        XmString b1 )
{
    unsigned char  *a;
    unsigned char  *b;
    unsigned short a_length, b_length;
    Boolean	   ret_val;

    _XmProcessLock();
    if ((a1 == NULL) && (b1 == NULL)) {
	_XmProcessUnlock();
	return (TRUE);
    }
    if ((a1 == NULL) || (b1 == NULL)) {
	_XmProcessUnlock();
	return (FALSE);
    }

    a_length = XmCvtXmStringToByteStream(a1, &a);
    b_length = XmCvtXmStringToByteStream(b1, &b);

    if ((a_length != b_length) || (memcmp(a, b, a_length) != 0))
      ret_val = FALSE;
    else ret_val = TRUE;

    XtFree((char *)a);
    XtFree((char *)b);

    _XmProcessUnlock();
    return(ret_val);
}

