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

#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include <string>            // For string
#include <exception>         // For exception class

#include "socket.h"

using namespace std;

/**
 *   TCP socket for communication with other TCP sockets
 */
class TCPSocket : public Socket {
  public:
    /**
     *   Construct a TCP socket with no connection
     *   @exception SocketException thrown if unable to create TCP socket
     */
    TCPSocket() throw(SocketException);
    TCPSocket(int newConnSD);

    /**
     *   Construct a TCP socket with a connection to the given foreign address
     *   and port
     *   @param foreignAddress foreign address (IP address or name)
     *   @param foreignPort foreign port
     *   @exception SocketException thrown if unable to create TCP socket
     */
    TCPSocket(const string &foreignAddress, unsigned short foreignPort) 
      throw(SocketException);

    /**
     *   Establish a socket connection with the given foreign
     *   address and port
     *   @param foreignAddress foreign address (IP address or name)
     *   @param foreignPort foreign port
     *   @exception SocketException thrown if unable to establish connection
     */
    void connect(const string &foreignAddress, unsigned short foreignPort)
      throw(SocketException);

    /**
     *   Write the given buffer to this socket.  Call connect() before
     *   calling send()
     *   @param buffer buffer to be written
     *   @param bufferLen number of bytes from buffer to be written
     *   @exception SocketException thrown if unable to send data
     */
    void send(const void *buffer, int bufferLen) throw(SocketException);

    /**
     *   Read into the given buffer up to bufferLen bytes data from this
     *   socket.  Call connect() before calling recv()
     *   @param buffer buffer to receive the data
     *   @param bufferLen maximum number of bytes to read into buffer
     *   @return number of bytes read, 0 for EOF, and -1 for error
     *   @exception SocketException thrown if unable to receive data
     */
    int recv(void *buffer, int bufferLen) throw(SocketException);

    /**
     *   Get the foreign address.  Call connect() before calling recv()
     *   @return foreign address
     *   @exception SocketException thrown if unable to fetch foreign address
     */
    string getForeignAddress() throw(SocketException);

    /**
     *   Get the foreign port.  Call connect() before calling recv()
     *   @return foreign port
     *   @exception SocketException thrown if unable to fetch foreign port
     */
    unsigned short getForeignPort() throw(SocketException);

  protected:
    TCPSocket(int type, int protocol) throw(SocketException);
};

/**
 *   TCP socket class for servers
 */
class TCPServerSocket : public Socket {
  public:
    /**
     *   Construct a TCP socket for use with a server, accepting connections
     *   on the specified port on any interface
     *   @param localPort local port of server socket, a value of zero will
     *                   give a system-assigned unused port
     *   @param queueLen maximum queue length for outstanding 
     *                   connection requests (default 5)
     *   @exception SocketException thrown if unable to create TCP server socket
     */
    TCPServerSocket(unsigned short localPort, int queueLen = 5) 
      throw(SocketException);

    /**
     *   Construct a TCP socket for use with a server, accepting connections
     *   on the specified port on the interface specified by the given address
     *   @param localAddress local interface (address) of server socket
     *   @param localPort local port of server socket
     *   @param queueLen maximum queue length for outstanding 
     *                   connection requests (default 5)
     *   @exception SocketException thrown if unable to create TCP server socket
     */
    TCPServerSocket(const string &localAddress, unsigned short localPort,
        int queueLen = 5) throw(SocketException);

    /**
     *   Blocks until a new connection is established on this socket or error
     *   @return new connection socket
     *   @exception SocketException thrown if attempt to accept a new connection fails
     */
    TCPSocket *accept() throw(SocketException);

  private:
    // Access for TCPServerSocket::accept() connection creation
    //TCPSocket(int newConnSD);
    void setListen(int queueLen) throw(SocketException);
};

#endif // _TCP_SOCKET_H_
