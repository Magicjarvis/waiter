waiter
======

A simple (HTTP) server

## About
Simple webserver using BSD Sockets.

Development has only just begun! Right now, please pay this no mind.

## Build Instructions

    $ git clone git@github.com:Magicjarvis/waiter.git
    $ sudo make install
    $ sudo ldconfig

## Sample main.cc
    $ #include <waiter.h>
    $
    $ using waiter::server;
    $ using waiter::http::response;
    $ using waiter::http::request;
    $ int main(int, char** argv) {
    $   std::stringstream ss(argv[1]);
    $   int port;
    $   ss >> port;
    $   server serv;
    $   serv.bind("/", [] (request) {
    $     response res;  
    $     std::stringstream message;
    $     message << "Hey! Welcome to the site. <img src=\"foo.jpg\"><a href=\"foo\">Link</a>";
    $     res.write(message);
    $     return res.generate();
    $   });
    $ 
    $   serv.bind("/foo", [] (request) {
    $     response res;  
    $     std::stringstream message;
    $     message << "Hey! Welcome to foo page. <a href=\"/\">back</a>";
    $     res.write(message);
    $     return res.generate();
    $   });
    $ 
    $   serv.listen(port);
    $ }

## Running Instructions
    $ g++ -std=c++11 main.cc -o my_server -lwaiter
    $ ./my_server 3000

## Documentation
    generate documentation:
    $ make documentation

    remove documentation:
    $ make clean
