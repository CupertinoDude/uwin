/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char sccsid[] = "@(#)ttymsg.c	8.2 (Berkeley) 11/16/93";
#endif /* not lint */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/uio.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ERRBUF 1024
/*
 * Display the contents of a uio structure on a terminal.  Used by wall(1),
 * syslogd(8), and talkd(8).  Forks and finishes in child if write would block,
 * waiting up to tmout seconds.  Returns pointer to error string on unexpected
 * error; string is not newline-terminated.  Various "normal" errors are
 * ignored (exclusive-use, lack of permission, etc.).
 */
char *
ttymsg(iov, iovcnt, line, tmout)
	struct iovec *iov;
	int iovcnt;
	char *line;
	int tmout;
{
	static char device[MAXNAMLEN] = PATH_TTY_PFX;
	static char errbuf[MAX_ERRBUF];
	register int cnt, fd, left, wret;
	struct iovec localiov[6];
	int forked = 0;

	if (iovcnt > sizeof(localiov) / sizeof(localiov[0]))
		return ("too many iov's (change code in wall/ttymsg.c)");

/* 
 * we're watching for '/', ".", ".."
 * '/' --> somebody could specify tty as ../etc/passwd
 * ".", ".." those are not security related it's just
 * sanity checks
 */
	if (strchr(line, '/') && ! strcmp(line, ".") && ! strcmp(line, "..")) {
		/* A slash is an attempt to break security... */
		(void) snprintf(errbuf, sizeof(errbuf), "'/' in \"%s\"",
		    device);
		return (errbuf);
	}
	(void) strncpy(device + sizeof(PATH_TTY_PFX) - 1, line, MAXNAMLEN - sizeof(PATH_TTY_PFX));
  device[MAXNAMLEN -1] = '\0';

	/*
	 * open will fail on slip lines or exclusive-use lines
	 * if not running as root; not an error.
	 */
	if ((fd = open(device, O_WRONLY|O_NONBLOCK, 0)) < 0) {
		if (errno == EBUSY || errno == EACCES)
			return (NULL);
		(void) snprintf(errbuf, sizeof(errbuf),
		    "%s: %s", device, strerror(errno));
		return (errbuf);
	}

	for (cnt = left = 0; cnt < iovcnt; ++cnt)
		left += iov[cnt].iov_len;

	for (;;) {
		wret = writev(fd, iov, iovcnt);
		if (wret >= left)
			break;
		if (wret >= 0) {
			left -= wret;
			if (iov != localiov) {
				bcopy(iov, localiov,
				    iovcnt * sizeof(struct iovec));
				iov = localiov;
			}
			for (cnt = 0; wret >= iov->iov_len; ++cnt) {
				wret -= iov->iov_len;
				++iov;
				--iovcnt;
			}
			if (wret) {
#if 0
				iov->iov_base += wret;
#else
				iov->iov_base = (char*)iov->iov_base+wret;
#endif
				iov->iov_len -= wret;
			}
			continue;
		}
		if (errno == EWOULDBLOCK) {
			int cpid, off = 0;

			if (forked) {
				(void) close(fd);
				_exit(1);
			}
			cpid = fork();
			if (cpid < 0) {
				(void) snprintf(errbuf, sizeof(errbuf),
				    "fork: %s", strerror(errno));
				(void) close(fd);
				return (errbuf);
			}
			if (cpid) {	/* parent */
				(void) close(fd);
				return (NULL);
			}
			forked++;
			/* wait at most tmout seconds */
			(void) signal(SIGALRM, SIG_DFL);
			(void) signal(SIGTERM, SIG_DFL); /* XXX */
#ifdef HAVE_SIGACTION
			{
			  sigset_t empty;
			  sigemptyset(&empty);
			  sigprocmask(SIG_SETMASK, &empty, 0);
			}
#else
			(void) sigsetmask(0);
#endif
			(void) alarm((u_int)tmout);
			(void) fcntl(fd, O_NONBLOCK, &off);
			continue;
		}
		/*
		 * We get ENODEV on a slip line if we're running as root,
		 * and EIO if the line just went away.
		 */
		if (errno == ENODEV || errno == EIO)
			break;
		(void) close(fd);
		if (forked)
			_exit(1);
		(void) snprintf(errbuf, sizeof(errbuf),
		    "%s: %s", device, strerror(errno));
		return (errbuf);
	}

	(void) close(fd);
	if (forked)
		_exit(0);
	return (NULL);
}
