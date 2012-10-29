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

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>            // For string
#include <exception>         // For exception class

using namespace std;

/**
 *   Signals a problem with the execution of a socket call.
 */
class SocketException : public exception {
  public:
    /**
     *   Construct a SocketException with a explanatory message.
     *   @param message explanatory message
     *   @param incSysMsg true if system message (from strerror(errno))
     *   should be postfixed to the user provided message
     */
    SocketException(const string &message, bool inclSysMsg = false) throw();

    /**
     *   Provided just to guarantee that no exceptions are thrown.
     */
    ~SocketException() throw();

    /**
     *   Get the exception message
     *   @return exception message
     */
    const char *what() const throw();

  private:
    string userMessage_;  // Exception message
};

/**
 *   Base class representing basic communication endpoint
 */
class Socket {
  public:
    /**
     *   Close and deallocate this socket
     */
    virtual ~Socket();

    /**
     *   Get the local address
     *   @return local address of socket
     *   @exception SocketException thrown if fetch fails
     */
    string getLocalAddress() throw(SocketException);

    /**
     *   Get the local port
     *   @return local port of socket
     *   @exception SocketException thrown if fetch fails
     */
    unsigned short getLocalPort() throw(SocketException);

    /**
     *   Set the local port to the specified port and the local address
     *   to any interface
     *   @param localPort local port
     *   @exception SocketException thrown if setting local port fails
     */
    void setLocalPort(unsigned short localPort) throw(SocketException);

    /**
     *   Set the local port to the specified port and the local address
     *   to the specified address.  If you omit the port, a random port 
     *   will be selected.
     *   @param localAddress local address
     *   @param localPort local port
     *   @exception SocketException thrown if setting local port or address fails
     */
    void setLocalAddressAndPort(const string &localAddress, 
        unsigned short localPort = 0) throw(SocketException);

    /**
     *   Resolve the specified service for the specified protocol to the
     *   corresponding port number in host byte order
     *   @param service service to resolve (e.g., "http")
     *   @param protocol protocol of service to resolve.  Default is "tcp".
     */
    static unsigned short resolveService(const string &service,
        const string &protocol = "tcp");

  private:
    // Prevent the user from trying to use value semantics on this object
    Socket(const Socket &sock);
    void operator=(const Socket &sock);

  protected:
    int sockDesc_;              // Socket descriptor
    Socket(int type, int protocol) throw(SocketException);
    Socket(int sockDesc);
};

#endif // _SOCKET_H_
