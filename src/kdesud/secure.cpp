/* vi: ts=8 sts=4 sw=4
 *
 * $Id$
 *
 * This file is part of the KDE project, module kdesu.
 * Copyright (C) 1999 Geert Jansen <g.t.jansen@stud.tue.nl>
 * 
 * secure.cpp: Peer credentials for a UNIX socket.
 */

#include <config.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <qglobal.h>

#include "secure.h"


/**
 * Under Linux, Socket_security is supported.
 */

#if defined(SO_PEERCRED) 

SocketSecurity::SocketSecurity(int sockfd)
{
    ksize_t len = sizeof(struct ucred);
    if (getsockopt(sockfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
	qWarning("getsockopt(SO_PEERCRED): %s", strerror(errno));
	return; 
    }

    ok = true;
}

#else


/**
 * The default version does nothing.
 */

SocketSecurity::SocketSecurity(int sockfd)
{
    qWarning("Using void socket security. Please add support for your");
    qWarning("platform to src/kdesud/secure.cpp");

    // This passes the test made in handler.cpp
    cred.uid = getuid();
    ok = true;
}

#endif 
