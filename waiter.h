#include <functional>
#include <unordered_map>

#include "http.h"

namespace waiter {
class server {
  public:
    server();
    ~server();
    bool bound(std::string route);
    void bind(std::string route,
        std::function<std::string(http::request)> const& func);
    void listen(int portNumber);
    std::string handle(http::request& req, std::string route);
  private:
    std::unordered_map<std::string,
      std::function<std::string(http::request)>> routes_;
};
}
