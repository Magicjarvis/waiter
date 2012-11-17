#include <functional>
#include <unordered_map>

#include "http.h"

namespace waiter {
class server {
  public:
    
    /**
     * TODO: COMMENT
     */
    server();

    /**
     * TODO: COMMENT
     */
    ~server();

    /**
     * TODO: COMMENT
     */
    bool bound(std::string route);

    /**
     * TODO: COMMENT
     */
    void bind(std::string route,
        std::function<std::string(http::request)> const& func);
    /**
     * TODO: COMMENT
     */
    void listen(int portNumber);

    /**
     * TODO: COMMENT
     */
    std::string handle(http::request& req, std::string route);
  private:

    /**
     * TODO: COMMENT
     */
    std::unordered_map<std::string,
      std::function<std::string(http::request)>> routes_;
};
}
