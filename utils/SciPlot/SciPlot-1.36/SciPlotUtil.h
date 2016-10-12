#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* SciPlotUtil.c */
Widget SciPlotDialog P_((Widget parent, char *title));
void SciPlotDialogPopup P_((Widget w));
void SciPlotReadDataFile P_((Widget parent, FILE *fd));

#undef P_
