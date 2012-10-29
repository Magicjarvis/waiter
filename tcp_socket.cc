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

#include "tcp_socket.h"

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in

typedef void raw_type;       // Type used for raw data on this platform

#include <errno.h>             // For errno
#include <cstring>             // For errno

using namespace std;

// TCPSocket Code

TCPSocket::TCPSocket(int type, int protocol)  
  throw(SocketException) : Socket(type, protocol) {
  }

TCPSocket::TCPSocket(int newConnSD) : Socket(newConnSD) {
}

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

void TCPSocket::connect(const string &foreignAddress,
    unsigned short foreignPort) throw(SocketException) {
  // Get the address of the requested host
  sockaddr_in destAddr;
  fillAddr(foreignAddress, foreignPort, destAddr);

  // Try to connect to the given port
  if (::connect(sockDesc_, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
    throw SocketException("Connect failed (connect())", true);
  }
}

void TCPSocket::send(const void *buffer, int bufferLen) 
  throw(SocketException) {
    if (::send(sockDesc_, (raw_type *) buffer, bufferLen, 0) < 0) {
      throw SocketException("Send failed (send())", true);
    }
  }

int TCPSocket::recv(void *buffer, int bufferLen) 
  throw(SocketException) {
    int rtn;
    if ((rtn = ::recv(sockDesc_, (raw_type *) buffer, bufferLen, 0)) < 0) {
      throw SocketException("Received failed (recv())", true);
    }

    return rtn;
  }

string TCPSocket::getForeignAddress() 
  throw(SocketException) {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getpeername(sockDesc_, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
      throw SocketException("Fetch of foreign address failed (getpeername())", true);
    }
    return inet_ntoa(addr.sin_addr);
  }

unsigned short TCPSocket::getForeignPort() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sockDesc_, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of foreign port failed (getpeername())", true);
  }
  return ntohs(addr.sin_port);
}

// TCPSocket Code

TCPSocket::TCPSocket() 
  throw(SocketException) : TCPSocket(SOCK_STREAM, 
      IPPROTO_TCP) {
  }

TCPSocket::TCPSocket(const string &foreignAddress, unsigned short foreignPort)
  throw(SocketException) : TCPSocket(SOCK_STREAM, IPPROTO_TCP) {
    connect(foreignAddress, foreignPort);
  }

//TCPSocket::TCPSocket(int newConnSD) : Socket(newConnSD) {
//}

// TCPServerSocket Code

TCPServerSocket::TCPServerSocket(unsigned short localPort, int queueLen) 
  throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
    setLocalPort(localPort);
    setListen(queueLen);
  }

TCPServerSocket::TCPServerSocket(const string &localAddress, 
    unsigned short localPort, int queueLen) 
  throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
    setLocalAddressAndPort(localAddress, localPort);
    setListen(queueLen);
  }

TCPSocket *TCPServerSocket::accept() throw(SocketException) {
  int newConnSD;
  if ((newConnSD = ::accept(sockDesc_, NULL, 0)) < 0) {
    throw SocketException("Accept failed (accept())", true);
  }

  return new TCPSocket(newConnSD);
}

void TCPServerSocket::setListen(int queueLen) throw(SocketException) {
  if (listen(sockDesc_, queueLen) < 0) {
    throw SocketException("Set listening socket failed (listen())", true);
  }
}
