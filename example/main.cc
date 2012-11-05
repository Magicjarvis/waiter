#include <waiter.h>

using waiter::server;
using waiter::http::response;
using waiter::http::request;

int main(int, char** argv) {
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
