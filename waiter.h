#include <functional>
#include <unordered_map>

#include "http.h"

namespace waiter {
/**
 * class that listens for http requests on a socket and serves the appropriate 
 * page source
 */
class server {
  public:
    /**
     * constructor
     */
    server();
    /**
     * destructor
     */
    ~server();

    /**
     * true if the route is in the routes table
     * @param route route be checked
     */
    bool bound(std::string route);

    /**
     * add a route to the routes table
     * @param route path to content
     * @param func function that serves content at the given route
     */
    void bind(std::string route,
        std::function<std::string(http::request)> const& func);

    /**
     * listens for content requests at a given port number on localhost
     * @param portNumber port number
     */
    void listen(int portNumber);

    /**
     * generates the page source for the route and http request
     * @req http request
     * @route route
     * @return page source content
     */
    std::string handle(http::request& req, std::string route);

  private:

    // routes map (key - route path, val - http request handler)
    std::unordered_map<std::string,
      std::function<std::string(http::request)>> routes_;
};
}
