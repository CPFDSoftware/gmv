c     machine specific xdr infinity constants	
c
      double precision XDRDINF,XDRFINF
c     ifdef vax
c
      parameter(XDRDINF = 1.7014118346046923e+38)
      parameter(XDRFINF = 2.93873588e-39)
      
c     ifdef sun
c     parameter(XDRDINF = 1.797693134862315900e+308)
c     parameter(XDRFINF = 3.40282357e+38)
c     
c     Fill values
c     These values are stuffed into newly allocated space as appropriate.
c     The hope is that one might use these to notice that a particular dataum
c     has not been set.
c     
      integer FILBYTE,FILCHAR, FILSHORT,FILLONG
      real FILFLOAT
      double precision FILDOUB
c
c     Largest Negative value 
      parameter(FILBYTE = 128)
      parameter(FILCHAR = 0)
      parameter(FILSHORT = 32768)
      parameter(FILLONG = -2147483648)
c     IEEE Infinity 
      parameter(FILFLOAT = XDRFINF)
      parameter(FILDOUB = XDRDINF)
