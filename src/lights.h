#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

#ifdef HDATA_INIT 
float kcolors[96] = 
         {
          0., 1., 1., 1., 1., 0., 1., 1., 0., 0., 
            .224, 0., .373, .412, 1., .625, .192, 0., .448, 1., 
            1., .351, .533, .625, .625, .1164, .116, .448, 
            .576, .306, .449, .448,
          1., 0., 0., 1., 0., 0., 0.25, 0., 1., 1.,
            0., 0.5, .588, .412, .248, .08, .286, 1., .212, .271, 
            .891, .051, .625, .005, .116, .625, .2079, 6.857e-1, 
            .351, .116, .051, 1.,
          0., 1., 0., 0., .488, 1., 0., .215, 1., .148,
           .448, 1., 0., .412, .248, .351, .384, .447, .890, .502,
           .522, 1., .212, 1., .116, .208, .6245, .148,
           1., 1., .891, .116
         };
#else
EXTERN float kcolors[96];
#endif

EXTERN float lt[5];
EXTERN float lightx, lighty;
EXTERN float matrgba[4*MAXMATS];
EXTERN float conrgba[4*(MAXCONTOURS+2)];
EXTERN int matbegpix, contbegpix;
EXTERN short fieldcolortype;
