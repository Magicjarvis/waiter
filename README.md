waiter
======

A simple (HTTP) server

## About
Simple webserver using BSD Sockets. (Current C++ Wrapper based heavily on http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/practical/)

Development has only just begun! Right now, please pay this no mind.

## Build and Run

    $ git clone git@github.com:Magicjarvis/waiter.git
    $ cd waiter
    $ make

Right now it's only a "Hello World" on localhost:4000
#include "waiter.h"

## Sample main.cc

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
