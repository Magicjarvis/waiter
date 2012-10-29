#include "tcp_socket.h"
#include <iostream>


int main() {
  const int MAX_WAIT = 5;
  TCPServerSocket socket("localhost", 4000, MAX_WAIT);


  char buffer[1000];
  while (true) {

    std::cout << "Accepting Connections on " << socket.getLocalAddress() << ":" << socket.getLocalPort() << std::endl;
    TCPSocket *client = socket.accept();
    std::cout << client->getForeignAddress() << ":" << client->getForeignPort() << " Connected" << std::endl;
    client->recv(buffer, 1000);
    client->send("HTTP/1.1 200 OK\nDate: Mon, 29 Oct 2012 09:04:06 GMT\nExpires: -1\nCache-Control: private, max-age=0\nContent-Type: text/html; charset=ISO-8859-1\n\nfoo", 153);
    delete client;
    std::cout << buffer << std::endl;

  }
}
