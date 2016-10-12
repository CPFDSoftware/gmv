/* $TOG: chownxterm.c /main/4 1998/02/06 11:23:10 kaleb $ */
/*

@OPENGROUP_COPYRIGHT@
COPYRIGHT NOTICE
Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
ALL RIGHTS RESERVED (MOTIF). See the file named COPYRIGHT.MOTIF for
the full copyright text.
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

This software is subject to an open license. It may only be
used on, with or for operating systems which are themselves open
source systems. You must contact The Open Group for a license
allowing distribution and sublicensing of this software on, with,
or for operating systems which are not Open Source programs.

See http://www.opengroup.org/openmotif/license for full
details of the license agreement. Any use, reproduction, or
distribution of the program constitutes recipient's acceptance of
this agreement.

EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
OR FITNESS FOR A PARTICULAR PURPOSE

EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

*/

/*
 * chownxterm --- make xterm suid root
 *
 * By Stephen Gildea, December 1993
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#define XTERM_PATH "/x11/programs/xterm/xterm"

#include <stdio.h>
#include <errno.h>

char *prog_name;

void help()
{
    setgid(getgid());
    setuid(getuid());
    printf("chown-xterm makes %s suid root\n", XTERM_PATH);
    printf("This is necessary on Ultrix for /dev/tty operation.\n");
    exit(0);
}

void print_error(err_string)
    char *err_string;
{
    setgid(getgid());
    setuid(getuid());
    fprintf(stderr, "%s: \"%s\"", prog_name, err_string);
    perror(" failed");
    exit(1);
}

main(argc, argv)
    int argc;
    char **argv;
{
    prog_name = argv[0];
    if (argc >= 2 && strcmp(argv[1], "-help") == 0) {
	help();
    } else {
	if (chown(XTERM_PATH, 0, -1) != 0)
	    print_error("chown root " XTERM_PATH);
	if (chmod(XTERM_PATH, 04555) != 0)
	    print_error("chmod 4555 " XTERM_PATH);
    }
    exit(0);
}
