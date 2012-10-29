/*
 *   Adapted 2012 by Jarvis Johnson
 *
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "socket.h"

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in

#include <cstring>             // for strerror
#include <errno.h>             // For errno

using namespace std;

// SocketException Code

SocketException::SocketException(const string &message, bool inclSysMsg)
  throw() : userMessage_(message) {
    if (inclSysMsg) {
      userMessage_.append(": ");
      userMessage_.append(strerror(errno));
    }
  }

SocketException::~SocketException() throw() {}

const char *SocketException::what() const throw() {
  return userMessage_.c_str();
}

// Function to fill in address structure given an address and port
static void fillAddr(const string &address, unsigned short port, 
    sockaddr_in &addr) {
  memset(&addr, 0, sizeof(addr));  // Zero out address structure
  addr.sin_family = AF_INET;       // Internet address

  hostent *host;  // Resolve name
  if ((host = gethostbyname(address.c_str())) == NULL) {
    // strerror() will not work for gethostbyname() and hstrerror() 
    // is supposedly obsolete
    throw SocketException("Failed to resolve name (gethostbyname())");
  }
  addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);

  addr.sin_port = htons(port);     // Assign port in network byte order
}

// Socket Code

Socket::Socket(int type, int protocol) throw(SocketException) 
  : sockDesc_(socket(PF_INET, type, protocol)) {
  if (sockDesc_ < 0) {
    throw SocketException("Creation of Socket Failed", true);
  }
}

Socket::Socket(int sockDesc) : sockDesc_(sockDesc) {}

Socket::~Socket() {
  ::close(sockDesc_);
  sockDesc_ = -1;
}

string Socket::getLocalAddress() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc_, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local address failed (getsockname())", true);
  }
  return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc_, (sockaddr *) &addr,
        (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local port failed (getsockname())", true);
  }
  return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort) throw(SocketException) {
  // Bind the socket to its port
  sockaddr_in localAddr;
  memset(&localAddr, 0, sizeof(localAddr)); /* Zero out structure */
  localAddr.sin_family = AF_INET; /* Address Family Internet */
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY); /*Any incoming interface */
  localAddr.sin_port = htons(localPort); /* Local port */

  if (bind(sockDesc_, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local port failed (bind())", true);
  }
}

void Socket::setLocalAddressAndPort(const string &localAddress,
    unsigned short localPort) throw(SocketException) {
  // Get the address of the requested host
  sockaddr_in localAddr;
  fillAddr(localAddress, localPort, localAddr);

  if (bind(sockDesc_, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local address and port failed (bind())", true);
  }
}

unsigned short Socket::resolveService(const string &service,
    const string &protocol) {
  struct servent *serv;        /* Structure containing service information */

  if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL) {
    return atoi(service.c_str());  /* Service is port number */
  }
  else {
    return ntohs(serv->s_port);    /* Found port (network byte order) by name */
  }
}
