#include <waiter.h>
#include <iostream>


using waiter::server;
using waiter::http::response;
using waiter::http::request;

int main(int argc, char** argv) {
  if(argc != 2){
    std::cout << "Usage - Please provide a port number" << std::endl;
    return 0;
  }
  std::stringstream ss(argv[1]);
  int port;
  ss >> port;
  server serv;
  serv.bind("/", [] (request) {
    response res;  
    std::stringstream message;
    message << "Hey! Welcome to the site. <img src=\"foo.jpg\"><a href=\"foo\">Link</a>";
    res.write(message);
    return res.generate();
  });

  serv.bind("/foo", [] (request) {
    response res;  
    std::stringstream message;
    message << "Hey! Welcome to foo page. <a href=\"/\">back</a>";
    res.write(message);
    return res.generate();
  });

  serv.listen(port);
}
