XCOMM!/bin/sh
XCOMM
XCOMM $TOG: mergelib.cpp /main/5 1998/02/06 11:24:31 kaleb $
XCOMM 
XCOMM @OPENGROUP_COPYRIGHT@
XCOMM COPYRIGHT NOTICE
XCOMM Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
XCOMM Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
XCOMM ALL RIGHTS RESERVED (MOTIF). See the file named COPYRIGHT.MOTIF for
XCOMM the full copyright text.
XCOMM  
XCOMM The above copyright notice and this permission notice shall be included in
XCOMM all copies or substantial portions of the Software.
XCOMM 
XCOMM This software is subject to an open license. It may only be
XCOMM used on, with or for operating systems which are themselves open
XCOMM source systems. You must contact The Open Group for a license
XCOMM allowing distribution and sublicensing of this software on, with,
XCOMM or for operating systems which are not Open Source programs.
XCOMM 
XCOMM See http://www.opengroup.org/openmotif/license for full
XCOMM details of the license agreement. Any use, reproduction, or
XCOMM distribution of the program constitutes recipient's acceptance of
XCOMM this agreement.
XCOMM 
XCOMM EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
XCOMM PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
XCOMM KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
XCOMM WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
XCOMM OR FITNESS FOR A PARTICULAR PURPOSE
XCOMM 
XCOMM EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
XCOMM NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
XCOMM INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
XCOMM DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
XCOMM AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
XCOMM LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
XCOMM ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
XCOMM EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
XCOMM POSSIBILITY OF SUCH DAMAGES.
XCOMM 
XCOMM Author:  Jim Fulton, MIT X Consortium
XCOMM 
XCOMM mergelib - merge one library into another; this is commonly used by X
XCOMM     to add the extension library into the base Xlib.
XCOMM

usage="usage:  $0  to-library from-library [object-filename-prefix]"
objprefix=_

case $# in
    2) ;;
    3) objprefix=$3 ;;
    *) echo "$usage" 1>&2; exit 1 ;;
esac

tolib=$1
fromlib=$2

if [ ! -f $fromlib ]; then
    echo "$0:  no such from-library $fromlib" 1>&2
    exit 1
fi

if [ ! -f $tolib ]; then
    echo "$0:  no such to-library $tolib" 1>&2
    exit 1
fi


XCOMM
XCOMM Create a temp directory, and figure out how to reference the 
XCOMM object files from it (i.e. relative vs. absolute path names).
XCOMM

tmpdir=tmp.$$
origdir=..

mkdir $tmpdir

if [ ! -d $tmpdir ]; then
    echo "$0:  unable to create temporary directory $tmpdir" 1>&2
    exit 1
fi

case "$fromlib" in
    /?*) upfrom= ;;
    *)  upfrom=../ ;;
esac

case "$tolib" in
    /?*) upto= ;;
    *)  upto=../ ;;
esac


XCOMM
XCOMM In the temp directory, extract all of the object files and prefix
XCOMM them with some symbol to avoid name clashes with the base library.
XCOMM
cd $tmpdir
ar x ${upfrom}$fromlib
for i in *.o; do
    mv $i ${objprefix}$i
done


XCOMM
XCOMM Merge in the object modules, ranlib (if appropriate) and cleanup
XCOMM
ARCMD ${upto}$tolib *.o
RANLIB ${upto}$tolib
cd $origdir
rm -rf $tmpdir



