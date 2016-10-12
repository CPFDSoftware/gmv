#!/bin/sh
#build the Mesa GMV gui
BUILD=Mesa
SUFFIX=.dynamic
make clean
make linux64mesa
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmv.X86_64.mesa-4.x.x$SUFFIX
SUFFIX=.partialstatic
./make-partial-static-linux64mesa.sh
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmv.X86_64.mesa-4.x.x$SUFFIX
#build the OpenGL GMV gui
BUILD=ogl
SUFFIX=.dynamic
make clean
make linux64ogl2
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmv.X86_64.opengl-4.x.x$SUFFIX
SUFFIX=.partialstatic
./make-partial-static-linux64ogl2.sh
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmv.X86_64.opengl-4.x.x$SUFFIX
#build GMV batch mode (non-gui)
BUILD=Batch
SUFFIX=.dynamic
make clean
make linux64batch
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmvbatch.X86_64-4.x.x$SUFFIX
SUFFIX=.partialstatic
./make-partial-static-linux64batch.sh
mv ./linux64$BUILD ./linux64$BUILD$SUFFIX
cp ./linux64$BUILD$SUFFIX /spider/tony/gmv/gmvbatch.X86_64-4.x.x$SUFFIX
make clean
