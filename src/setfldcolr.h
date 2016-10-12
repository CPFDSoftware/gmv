#include <math.h>

  (double fldval, double fldmin, double fldmax, double tempin, char logflag,
   int *iclr1)
{
  int iclr;
  float tempval;

   if (tempin < 1.0e-20)
     {
      *iclr1 = 400 + MAXMATS;
      return;
     }
   tempval = 253. / tempin;
   iclr = 0;
   if (logflag)
     {
      if (fldval >= fldmin)
         iclr = (int) ((log10(fldval) - log10(fldmin)) * tempval);
     }
   else
      iclr = (int) ((fldval - fldmin) * tempval);
   iclr += 400;
   if (iclr < 300) iclr = 300;
   if (fldval < fldmin) iclr = 300;
   if (fldval > fldmax) iclr = 400 + 255;

   *iclr1 = iclr + MAXMATS;
}
