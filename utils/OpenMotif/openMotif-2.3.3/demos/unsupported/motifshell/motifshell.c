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
static char rcsid[] = "$TOG: motifshell.c /main/7 1997/03/31 13:41:20 dbl $"
#endif
#endif

/****************************************************************************
 ****************************************************************************
 **
 **   File:     motifShell.c
 **
 **   Project:     Motif - widget examination program
 **
 **   Description: Program which shows resources of widgets
 **
 ****************************************************************************
 ****************************************************************************/

/***************************************************
*                                                  *
*  Revision history:                               *
*                                                  *
*  05/26/89      strong        Initial Version     *
*  06/01/89      strong        1.0                 *
*  06/26/89      pjlevine      complete rewrite    *
*  03/12/92      deblois       yet another rewrite *
*                                                  *
****************************************************/

#include <X11/Xos.h>

/*  Standard C headers  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>

/*  X headers  */
#include <X11/IntrinsicP.h>


/*  Xm headers  */
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/MainW.h>
#include <Xm/MenuShell.h>
#include <Xm/PushB.h>
#include <Xm/RepType.h>
#include <Xm/ScrolledW.h>
#include <Xm/SelectioB.h>
#include <Xm/Text.h>


#define APP_CLASS       "XMdemo"
#define DEFAULT_FONT    "fixed"
#define TEST_STRING     "The quick brown fox jumps over the lazy dog."
#define TEST_HELP       "Type here to test the font.\nApply sets this window.\nOK sets main window."
#define HELP_FILE       "help"
#define WELCOME_FILE    "welcome"
#define MEMBERSHIP_FILE "membership"
#define RESEARCH_FILE   "research"
#define PRINCIPLES_FILE "principles"
#define MOTIF_FILE      "motif"
#define READ            0
#define WRITE           1

/*  Global Variables  */
Display *display;
char         filename [256];
Widget       TextWin;
Widget       LabelW;
XtAppContext AppContext;
int          perr[2], p;

/*  Declarations  */
int GetFileLen (int fd);




/*-------------------------------------------------------------*
 |                    ExtractNormalString                      |
 | support routine to get normal string from XmString          |
 *-------------------------------------------------------------*/
char *ExtractNormalString(XmString cs)
{
  XmStringContext    context;
  XmStringCharSet    charset;
  XmStringDirection  direction;
  Boolean            separator;
  static char       *primitiveString;


  XmStringInitContext (&context, cs);
  XmStringGetNextSegment (context, &primitiveString, &charset,
			  &direction, &separator);
  XmStringFreeContext (context);

  return ((char *) primitiveString);
}


/*-------------------------------------------------------------*
 |                     FontSelectApply                         |
 *-------------------------------------------------------------*/
void FontSelectApply (Widget w, XtPointer client_data, XtPointer call_data)
{
  XmSelectionBoxCallbackStruct *cdata = (XmSelectionBoxCallbackStruct *)call_data;
  Widget       textWidget = (Widget)client_data;
  char        *textstr;
  XmFontList   fontList; 
  XFontStruct *mfinfo;


  /* no font selected... */
  if (cdata->value == NULL) return;

  textstr = ExtractNormalString (cdata->value);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, XmFONTLIST_DEFAULT_TAG);

  XtVaSetValues(textWidget, XmNfontList,  fontList, NULL);
}


/*-------------------------------------------------------------*
 |                       FontSelectOK                          |
 *-------------------------------------------------------------*/
void FontSelectOK (Widget w, XtPointer client_data, XtPointer call_data)
{
  XmSelectionBoxCallbackStruct *callback_data =
    (XmSelectionBoxCallbackStruct *)call_data;
  char        *textstr;
  XmFontList   fontList;
  XFontStruct *mfinfo;

  if (callback_data->value == (XmString) NULL)
    return;

  textstr = ExtractNormalString (callback_data->value);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, XmFONTLIST_DEFAULT_TAG);

  XtVaSetValues(TextWin, XmNfontList, fontList, NULL);
}



/*-------------------------------------------------------------*
 |                        FontTest                             |
 *-------------------------------------------------------------*/
void FontTest (Widget w, XtPointer client_data, XtPointer call_data)
{
  XmSelectionBoxCallbackStruct *callback_data = 
    (XmSelectionBoxCallbackStruct *)call_data;
  Widget       txtWidget = (Widget)client_data;
  char        *textstr = DEFAULT_FONT;
  XmFontList   fontList; 
  XFontStruct *mfinfo;


  if (callback_data->value == (XmString) NULL)
    return;

  textstr = ExtractNormalString (callback_data->value);
  if (textstr == NULL) sprintf(textstr, "%s", DEFAULT_FONT);

  if ((mfinfo = XLoadQueryFont(display, textstr))==NULL)
      printf ("couldn't open %s font\n", textstr);
  fontList = XmFontListCreate (mfinfo, " ");

  XtVaSetValues(txtWidget,
		XmNfontList,  fontList,
		XmNvalue,     TEST_HELP,
		NULL);
}

/*-------------------------------------------------------------*
 |                        NextCap                              |
 *-------------------------------------------------------------*/
  
char *NextCap (char *path, char *cp, int len)
{
  static int  finish = 0;
  int         span;
  char       *ep, *np;
   
  
  if (!finish)
    return(NULL);
  
  if (ep = strchr(cp, ':'))
    span = ep - cp;
  else
    {
      finish++;
      ep = strchr(cp, '\0');
      span = ep - cp;
    };
  
  np = malloc(span + len + 2);
  strncpy(np, cp, span);

  np[span] = '/';
  np[span+1] = '\0';

  return(np);
}


/*-------------------------------------------------------------*
 |                        file_exist                           |
 *-------------------------------------------------------------*/
int file_exist (char *fullname)
{
  /* Save the file pointer so it can be closed */
  FILE *tmpf;

  if (tmpf=fopen(fullname,"r"))
  {
      fclose(tmpf);
      return(1);
  }
  else
    return(0);
}  


/*-------------------------------------------------------------*
 |                   search_in_env                             |
 *-------------------------------------------------------------*/
char *search_in_env (char *filename)
{
  int   i, len;
  char *envpath, *prefix, *cp;


  len = strlen(filename);
  if (envpath = getenv("PATH"))
    {
      cp  = envpath;
      cp += 2; 
      
      while (prefix = NextCap(envpath, cp, len))
	{
	  cp += strlen(prefix);
	  strncat(prefix, filename, len);

	  if (file_exist(prefix))
	    return(prefix);
	  
	  free(prefix);
        }  
   
    }

  return(NULL);
}

/*-------------------------------------------------------------*
 |                        GetSource                            |
 *-------------------------------------------------------------*/
char *GetSource (char *fileptr)
{
  static char *retbuff;
  int          fd, flen, catlen;
  char        *capfileptr, *defaultcap, *datahome;
  
  
  if ((fd = open (fileptr, O_RDONLY)) < 0)
  {
    /* Try looking in MSHELLDIR. */
    char pathname[1024];
    /* strcpy(pathname, MSHELLDIR); */
    strcat(pathname, "/");
    strcat(pathname, fileptr);
      
    if ((fd = open (pathname, O_RDONLY)) < 0)
    {
      if (defaultcap = getenv("MOTIFSHELLFILES"))
	{
	  catlen = strlen(defaultcap);
	  datahome = (char *) malloc(catlen + strlen(fileptr) + 2);
	  strncpy(datahome, defaultcap, catlen);
	  datahome[catlen] = '/';
	  datahome[catlen + 1] = '\0';
	  strcat(datahome, fileptr);

	  if ((fd = open(datahome, O_RDONLY)) < 0)
	    {
	      printf ("Cannot find the file %s in %s\n", fileptr, datahome);
	      free(datahome);
	      return((char *) NULL);
	    }
	  free(datahome);
	}
      else
	if (capfileptr = search_in_env(fileptr))
	  {
	    fd = open (capfileptr, O_RDONLY);
	    free(capfileptr);
	  }
	else
	  {
	    printf ("Cannot find the file %s\n", fileptr);
	    printf ("Please setup MOTIFSHELLFILES entry in environment and put data files there.\n");
	    return ((char *) NULL);
	  }
    }
  }

  flen = GetFileLen(fd);
  retbuff = (char*) calloc (1, flen + 1);

  if (read (fd, retbuff, flen) <= 0)
    {
      printf ("Error reading file %s\n", fileptr);
      return ((char *) NULL);
    }
  close (fd);

  return (retbuff);
}


/*-------------------------------------------------------------*
 |                          DoTheFont                          |
 *-------------------------------------------------------------*/
XmString *DoTheFont (int *count)
{
  char **fontlist;
  int i;
  static XmString *addrstr;


  fontlist = XListFonts (display, "*", 1200, count);
  addrstr  = (XmString *) XtCalloc (*count, sizeof (XmString));
  for (i = 0; i < *count; i++) addrstr[i] = XmStringCreateLocalized(fontlist[i]);

  return (addrstr);
}


/*-------------------------------------------------------------*
 |                       PopupHelpShell                        |
 *-------------------------------------------------------------*/
void PopupHelpShell (Widget w, XtPointer client_data, XtPointer call_data)
{
  Widget helpText = (Widget)client_data;
  char *buffer;


  buffer = GetSource (HELP_FILE);
  XmTextSetString (helpText, buffer);
}


/*-------------------------------------------------------------*
 |                         SetLabelStr                         |
 *-------------------------------------------------------------*/
void SetLabelStr (XmString tcs)
{
  XtVaSetValues(LabelW, XmNlabelString, tcs, NULL);
}


/*-------------------------------------------------------------*
 |                          SetLabel                           |
 *-------------------------------------------------------------*/
void SetLabel (char *label)
{
  XmString      tcs;

  tcs = XmStringCreateLocalized(label);
  SetLabelStr(tcs);
  XmStringFree(tcs);
}

/*-------------------------------------------------------------*
 |                    ShowFontDialogShell                      |
 *-------------------------------------------------------------*/
void ShowFontDialogShell (Widget parent, char *label)
{
  Widget         workText, list;
  static Widget  dlog = NULL;
  int            count;
  XmString      *fonts;


  if (dlog == NULL)
    {
      fonts = DoTheFont(&count);

      dlog = XmCreateSelectionDialog(parent, label, NULL, 0);

      workText = XtVaCreateManagedWidget ("workText", xmTextWidgetClass, dlog,
					  XmNeditable,     True,
					  XmNeditMode,     XmMULTI_LINE_EDIT,
					  XmNcolumns,      30,
					  XmNrows,         3,
					  XmNresizeHeight, False,
					  XmNwordWrap,     True,
					  XmNvalue,        TEST_STRING,
					  NULL);
      XtVaSetValues(dlog,
		    XmNwidth,                450,
		    XmNheight,               450,
		    XmNallowShellResize,     True,
		    XmNlistItems,            fonts,
		    XmNlistItemCount,        count,
		    XmNlistVisibleItemCount, 10,
		    NULL);

      list = XmSelectionBoxGetChild(dlog, XmDIALOG_LIST);
      XtVaSetValues(list, XmNselectionPolicy, XmSINGLE_SELECT, NULL);

      XtUnmanageChild(XmSelectionBoxGetChild(dlog, XmDIALOG_APPLY_BUTTON));

      XtAddCallback (dlog, XmNokCallback,     FontSelectOK,     NULL);
      XtAddCallback (dlog, XmNhelpCallback,   FontTest,         (XtPointer)workText);
      XtAddCallback (list, XmNsingleSelectionCallback, FontSelectApply,
		     (XtPointer)workText);
    }

  XtManageChild(dlog);
}




/*-------------------------------------------------------------*
 |                   CreateHelpDialogShell                     |
 *-------------------------------------------------------------*/
void CreateHelpDialogShell (Widget parent, char *say)
{
  static Widget dlog = NULL;
  Widget helpText, button;

  if (dlog == NULL)
    {
      dlog     = XmCreateFormDialog(parent, "Help Window", NULL, 0);
      helpText = XtVaCreateManagedWidget("helpText", xmTextWidgetClass, dlog,
					 XmNeditable,         False,
					 XmNeditMode,         XmMULTI_LINE_EDIT,
					 XmNcolumns,          50,
					 XmNresizeHeight,     True,
					 XmNwordWrap,         True,
					 XmNleftAttachment,   XmATTACH_FORM,
					 XmNtopAttachment,    XmATTACH_FORM,
					 XmNrightAttachment,  XmATTACH_FORM,
					 XmNbottomAttachment, XmATTACH_FORM,
					 XmNbottomOffset,     30,
					 NULL);
      button   = XtVaCreateManagedWidget("dismiss", xmPushButtonWidgetClass, dlog,
					 XmNleftAttachment,   XmATTACH_FORM,
					 XmNtopAttachment,    XmATTACH_WIDGET,
					 XmNtopWidget,        helpText,
					 XmNrightAttachment,  XmATTACH_FORM,
					 XmNbottomAttachment, XmATTACH_FORM,
					 NULL);
      XtAddCallback (XtParent(dlog), XmNpopupCallback, PopupHelpShell,
		     (XtPointer)helpText);
    }

  XtManageChild(dlog);
}



/*-------------------------------------------------------------*
 |                         CreateTextWin                       |
 *-------------------------------------------------------------*/
Widget CreateTextWin (Widget parent)
{
  Widget    stext;
  char     *buffer;
  XmString  tcs;


  tcs    = XmStringCreateLocalized ("Welcome to Motif");
  LabelW = XtVaCreateManagedWidget("Label", xmLabelWidgetClass, parent,
				   XmNleftAttachment,  XmATTACH_FORM,
				   XmNtopAttachment,   XmATTACH_FORM,
				   XmNrightAttachment, XmATTACH_FORM,
				   XmNlabelString,     tcs,
				   XmNmarginHeight,    5,
				   XmNshadowThickness, 1,
				   NULL);
  XmStringFree (tcs);

  stext  = XmCreateScrolledText(parent, "s_text", NULL, 0);
  XtVaSetValues(stext,
		XmNeditMode,         XmMULTI_LINE_EDIT,
		XmNeditable,         False,
		XmNcolumns,          80,
		XmNrows,             30,
		NULL);
  XtVaSetValues(XtParent(stext),
		XmNleftAttachment,   XmATTACH_FORM,
		XmNtopAttachment,    XmATTACH_WIDGET,
		XmNrightAttachment,  XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopWidget,        LabelW,
		XmNleftOffset,       5,
		XmNrightOffset,      5,
		XmNbottomOffset,     5,
		XmNheight,           550,
		XmNwidth,            500,
		XmNresizeWidth,      True,
		XmNresizeHeight,     False,
		NULL);
  XtManageChild(stext);

  buffer = GetSource (WELCOME_FILE);
  XmTextSetString (stext, buffer);

  return(stext);
}


/*-------------------------------------------------------------*
 |                         GetFileLen                          |
 *-------------------------------------------------------------*/
int GetFileLen (int fd)
{
  static int retval;

#if defined(L_SET) && defined(L_XTND)
  lseek (fd, 0, L_SET);  
  retval = lseek (fd, 0, L_XTND);
  lseek (fd, 0, L_SET);  
#else
  lseek (fd, 0, SEEK_SET);
  retval = lseek (fd, 0, SEEK_END);
  lseek (fd, 0, SEEK_SET);
#endif
  return (retval);
}


/*-------------------------------------------------------------*
 |                         SysCall                             |
 *-------------------------------------------------------------*/
void SysCall (Widget widget, char *systemCommand, Boolean set_uidpath)
{
#define CMD	"find .. -name '%s' \\( -type f -o -type l \\) -print"
  char  str[256];
  char *findCmd;
  FILE *file;
  pid_t p;

  if ((p = fork()) == 0)
    {
      /* note - execlp uses PATH */
      execlp(systemCommand, systemCommand, NULL);

      /* if we fail to find the systemCommand, use 'find' to look for it. */
      fprintf(stderr, "can't find %s\n", systemCommand);

      findCmd = (char *) XtMalloc(strlen(systemCommand) + sizeof(CMD) + 1);
      sprintf(findCmd, CMD, systemCommand);
      file = popen(findCmd, "r");
      XtFree(findCmd);

      /* If this program is not on our path, then it's uid file */
      /* probably isn't either.  Set UIDPATH if requested and unset. */
      if (set_uidpath)
	set_uidpath = (getenv("UIDPATH") == NULL);

      while(fgets(str, 80, file) != NULL)
	{
	  str[strlen(str)-1] = '\0';
	  printf("Trying: %s\n", str);
	  if (set_uidpath)
	    {
#ifndef NO_PUTENV
	      char *uidpath = XtMalloc(8 + strlen(str) + 4 + 1);
	      sprintf(uidpath, "UIDPATH=%s.uid", str);
	      putenv(uidpath);
#else
	      char *uidpath = XtMalloc(strlen(str) + 4 + 1);
	      sprintf(uidpath, "%s.uid", str);
	      setenv("UIDPATH", uidpath, 1);
#endif
	    }
	  execl(str, systemCommand, NULL);
	  printf("Still can't find %s...\n", systemCommand);
	}
      printf("I give up!\n");
      exit(0);
    }
}


/*-------------------------------------------------------------*
 |                           Quit                              |
 *-------------------------------------------------------------*/
void Quit(int i)
{
  printf("Bye!\n");
  exit(0);
}


/*-------------------------------------------------------------*
 |                          Menu1CB                            |
 | File  pulldown menu items.                                  |
 *-------------------------------------------------------------*/
void Menu1CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int   itemNo = (int)clientData;

  switch (itemNo)
    {
    case 0: Quit(0);  break;  /* Quit */
    }
}


/*-------------------------------------------------------------*
 |                          Menu2CB                            |
 | OSF Happenings  pulldown menu items.                        |
 *-------------------------------------------------------------*/
void Menu2CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int        itemNo = (int)clientData;
  char      *buffer;
  XmString   labelStr;


  XtVaGetValues(w, XmNlabelString, &labelStr, NULL);
  SetLabelStr (labelStr);

  switch (itemNo)
    {
    case 0: buffer = GetSource (MEMBERSHIP_FILE);  break;
    case 1: buffer = GetSource (RESEARCH_FILE);    break;
    case 2: buffer = GetSource (PRINCIPLES_FILE);  break;
    case 3: buffer = GetSource (MOTIF_FILE);       break;
    }
  XmTextSetString (TextWin, buffer); 
}


/*-------------------------------------------------------------*
 |                          Menu3CB                            |
 | Demos  pulldown menu items.                                 |
 *-------------------------------------------------------------*/
void Menu3CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int      itemNo = (int)clientData;
  XmString labelStr;


  XtVaGetValues(w, XmNlabelString, &labelStr, NULL);
  SetLabelStr (labelStr);

  switch (itemNo)
    {
    case 0: SysCall (w, "periodic", True);  break;
    case 1: SysCall (w, "dogs", True);  break;
    case 2: SysCall (w, "motifanim", False);  break;
    }
}


/*-------------------------------------------------------------*
 |                          Menu4CB                            |
 | Unix Commands  pulldown menu items.                         |
 *-------------------------------------------------------------*/
void Menu4CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int       itemNo = (int)clientData;
  char     *buffer;
  XmString  labelStr;

  switch (itemNo)
    {
    case 0:                                /* File Listing */
      {
	XtVaGetValues(w, XmNlabelString, &labelStr, NULL);
	SetLabelStr (labelStr);
	system ("ls -al > /tmp/motifshell.tmp");
	buffer = GetSource ("/tmp/motifshell.tmp");
	XmTextSetString (TextWin, buffer);
	system ("rm -r /tmp/motifshell.tmp");
	break;
      }
    case 1:                                /* Process Status */
      {
	XtVaGetValues(w, XmNlabelString, &labelStr, NULL);
	SetLabelStr (labelStr);
#ifdef linux
	system ("ps a > /tmp/motifshell.tmp");
#else
	system ("ps -a > /tmp/motifshell.tmp");
#endif
	buffer = GetSource ("/tmp/motifshell.tmp");
	XmTextSetString (TextWin, buffer);
	system ("rm -r /tmp/motifshell.tmp");
	break;
      }
    case 2:                                /* Show Source */
      {
	XtVaGetValues(w, XmNlabelString, &labelStr, NULL);
	SetLabelStr (labelStr);
	buffer = GetSource (filename);
	XtVaSetValues(TextWin, XmNvalue, buffer, NULL);
	break;
      }
    }
}


/*-------------------------------------------------------------*
 |                          Menu5CB                            |
 | X Programs  pulldown menu items.                            |
 *-------------------------------------------------------------*/
void Menu5CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int itemNo = (int)clientData;

  switch (itemNo)
    {
    case 0: SysCall (w, "xterm", False);  break;
    case 1: SysCall (w, "xclock", False);  break;
    case 2: SysCall (w, "xload", False);   break;
    }
}


/*-------------------------------------------------------------*
 |                          Menu6CB                            |
 | Font  pulldown menu items.                                  |
 *-------------------------------------------------------------*/
void Menu6CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int itemNo = (int)clientData;

  switch (itemNo)
    {
    case 0: /* Load... */
      {
	ShowFontDialogShell (XtParent(w), "List O' Fonts");
	break;
      }
    }
}


/*-------------------------------------------------------------*
 |                          Menu7CB                            |
 | Help  pulldown menu items.                                  |
 *-------------------------------------------------------------*/
void Menu7CB (Widget w, XtPointer clientData, XtPointer callData)
{
  int itemNo = (int)clientData;
  

  switch (itemNo)
    {
    case 0: /* Are you sure? */
      {
	CreateHelpDialogShell (XtParent(w), "Help Window");
	break;
      }
    }
}


static  char     *menuString[] = {
    "File", "OSF Happenings", "Demos", "Unix Commands", "X Programs", "Font", "Help"
    };
static   char     *subString[][XtNumber(menuString)] = {
    { "Quit" },
    { "OSF Membership", "OSF's Research Institute",  "OSF's Principles", "OSF/Motif" },
    { "Periodic Table", "Dogs", "Animation" },
    { "File Listing", "Process Status", "Show Source" },
    { "Xterm", "Xclock", "Xload"},
    { "Load..." },
    { "Are you sure ?" }
  };


/*-------------------------------------------------------------*
 |                         CreateMenuBar                       |
 *-------------------------------------------------------------*/
Widget CreateMenuBar (Widget parent)
{
  int       i;
  XmString  s[10];
  Widget    menuBar, helpCascade;

  menuBar = XmVaCreateSimpleMenuBar(parent, "MenuBar",
	XmVaCASCADEBUTTON, s[0] = XmStringCreateSimple(menuString[0]), menuString[0][0],
	XmVaCASCADEBUTTON, s[1] = XmStringCreateSimple(menuString[1]), menuString[1][0],
	XmVaCASCADEBUTTON, s[2] = XmStringCreateSimple(menuString[2]), menuString[2][0],
	XmVaCASCADEBUTTON, s[3] = XmStringCreateSimple(menuString[3]), menuString[3][0],
	XmVaCASCADEBUTTON, s[4] = XmStringCreateSimple(menuString[4]), menuString[4][0],
	XmVaCASCADEBUTTON, s[5] = XmStringCreateSimple(menuString[5]), menuString[5][0],
	XmVaCASCADEBUTTON, s[6] = XmStringCreateSimple(menuString[6]), menuString[6][0],
	NULL);
  for (i=0; i<=6; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[0], 0, Menu1CB,   /* File */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[0][0]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<1; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[1], 1, Menu2CB,   /* OSF Happenings */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[1][0]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[1] = XmStringCreateSimple(subString[1][1]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[2] = XmStringCreateSimple(subString[1][2]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[3] = XmStringCreateSimple(subString[1][3]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=3; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[2], 2, Menu3CB,     /* Demos */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[2][0]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[1] = XmStringCreateSimple(subString[2][1]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[2] = XmStringCreateSimple(subString[2][2]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=2; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[3], 3, Menu4CB,   /* Unix Commands */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[3][0]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[1] = XmStringCreateSimple(subString[3][1]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[2] = XmStringCreateSimple(subString[3][2]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=2; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[4], 4, Menu5CB,   /* X Programs */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[4][0]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[1] = XmStringCreateSimple(subString[4][1]), NULL, NULL, NULL,
	XmVaPUSHBUTTON, s[2] = XmStringCreateSimple(subString[4][2]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=2; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[5], 5, Menu6CB,   /* Font */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[5][0]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=0; i++) XmStringFree(s[i]);

  XmVaCreateSimplePulldownMenu(menuBar, menuString[6], 6, Menu7CB,     /* Help */
	XmVaPUSHBUTTON, s[0] = XmStringCreateSimple(subString[6][0]), NULL, NULL, NULL,
	NULL);
  for (i=0; i<=0; i++) XmStringFree(s[i]);
  XtVaSetValues(menuBar, XmNmenuHelpWidget, XtNameToWidget(menuBar, "button_6"), NULL);


  XtManageChild(menuBar);

  return (menuBar);
}



/*-------------------------------------------------------------*
 |                            main                             |
 *-------------------------------------------------------------*/

int main (int argc, char **argv)
{
  Widget shell, mainWindow, workRegion, menuBar;


  signal(SIGHUP,  Quit);
  signal(SIGINT,  Quit);
  signal(SIGQUIT, Quit);

  system ("touch /tmp/motifshell.tmp");
  system ("rm -r /tmp/motifshell.tmp");

  XtToolkitInitialize();
  AppContext = XtCreateApplicationContext();
  if ((display = XtOpenDisplay (AppContext, NULL, argv[0], APP_CLASS,
				NULL, 0, &argc, argv)) == NULL)
    {
      fprintf (stderr,"\n%s:  Can't open display\n", argv[0]);
      exit(1);
    }

  /* capture the source-code file name for use later. */
  sprintf(filename, "%s.c", argv[0]);

  XmRepTypeInstallTearOffModelConverter();

  shell = XtVaAppCreateShell(argv[0], APP_CLASS, applicationShellWidgetClass,
			     display, XmNallowShellResize, True, NULL);
			     

  mainWindow = XtVaCreateManagedWidget("mainWindow", xmMainWindowWidgetClass, shell,
				       XmNmarginWidth,  2,
				       XmNmarginHeight, 2, NULL);
  workRegion = XtVaCreateManagedWidget("s_text", xmFormWidgetClass, mainWindow, NULL);

  menuBar = CreateMenuBar (mainWindow);
  TextWin = CreateTextWin (workRegion);
  XmMainWindowSetAreas(mainWindow, menuBar, NULL, NULL, NULL, workRegion);


  XtRealizeWidget(shell);

  XtAppMainLoop(AppContext);

  return 0;    /* make compiler happy */
}
