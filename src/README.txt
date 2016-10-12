Last Edited: 2015-11-10

Unzip the contents of your chosen Version of GMV to a folder.  From that
folder you should see one of the following names from the "Binary files"
below.
We recommend that you copy the binary to an appropriate location on your
system for all users to access, and name it a generic "gmv".  For example, if
you had the Linux 64 bit OpenGL version:
    sudo cp ./linux64ogl /usr/local/bin/gmv
    <Enter root password>
The file is then copied to /usr/local/bin/gmv for all to use.  You may need to
also update each user's PATH environment variable to include "/usr/local/bin".
Generally this can be done by adding this line to the user's ".bashrc" or
".profile" file:
    export PATH=$PATH:/usr/local/bin

====================================
Binary files for GMV:
------------------------------------------------------------------------------
linux64ogl - Linux 64 bit OpenGL version
linux64Mesa - Linux 64 bit version with Mesa OpenGL to X API
cygwinMesa - Windows 7 64 bit version (built with Cygwin 32 bit) with Mesa 
               OpenGL to X API, requires Cygwin 32 bit or other X-Server

====================================
Batch versions: These render the image in the background and do not 
                require a window to draw into and is not interactive.
                Note: GMV Batch is included in the BATCHMOVIE script purchase.
------------------------------------------------------------------------------
linux64Batch - Linux 64 bit batch version with Mesa OpenGL to X API
cygwinBatch - Windows 7 64 bit batch version (built with Cygwin 32 bit), 
              requires Cygwin 32 bit or other X-Server

============
Other files:
------------
Readme.changes - Change history file
gmvdoc.pdf - Users Manual in PDF format
gmvrc.sample - example of a gmvrc file
gmvtest.inp - test input file for GMV

===============================================================================

Notes:

The only supported platforms are now Linux 64 bit and Windows 7 64 bit.

Sandia's ExodusII Library, version 4.46, is used to read ExodusII files.
University Corporation for Atmospheric Research/Unidata's netcdf library, 
version 3.62, is used by ExodusII to store/extract data from ExodusII files.

The Independent JPEG Group's jpeg library, version 6b, is used to make the 
jpeg images.  The library is statically linked into GMV.

The Mesa versions have been statically linked with the Mesa library.  
These versions convert the OpenGL commands to X11 commands via the Mesa 
interface.  There is no graphics acceleration, so these versions can be slow.

All of the Mesa versions will now create it's own colormap so color 
flashing may occur.  The Mesa versions should now start under any color 
map conditions and will provide better colors. 

The Mesa library is written by Brian Paul.  Information on Mesa can 
be found on the internet at: http://www.mesa3d.org.  
The version that GMV uses is 7.8.2.

Linux OpenGL versions require OpenGL elf libraries and the OpenGL extensions to
the X server (glx).  

The Batch versions use Mesa's OSMesa off-screen buffers to generate the 
image.  Rename the binaries to gmvbatch.

The Cygwin GMV version is for Windows 7 64 bit and requires Cygwin/X.
This can be found on the internet at:
http://x.cygwin.com
There may be some errors with Cygwin versions, because Cygwin has some 
differences from Linux.
Any OpenGL capable X-server should be usable with a Cygwin version, just be 
sure you have the following in your bashrc file:
#! /bin/sh
export DISPLAY=127.0.0.1:0.0
PATH=/usr/X11R6/bin:$PATH

With any questions, please contact:
gmv (at) cpfd-software.com