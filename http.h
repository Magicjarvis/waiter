#include <sstream>
#include <vector>
#include <unordered_map>

namespace waiter {
namespace http {
class request {
  // better way of doing this? I don't like it;
  friend std::ostream& operator<<(std::ostream &os, const request &req);
  static std::vector<std::string> spaceSplit(std::string line);
  static std::vector<std::string> getLines(std::string req);
  public:
  static request reqFromString(std::string str);
    inline std::string method() const {
      return method_;
    }
    inline std::string path() const {
      return path_;
    }
    inline std::string version() const {
      return version_;
    }
    inline std::unordered_map<std::string, std::string> fields() const {
      return fields_;
    }
  private:
    request(std::string meth, std::string route, 
        std::string ver, std::unordered_map<std::string, std::string> fields);
    std::unordered_map<std::string, std::string> fields_;
    std::string method_;
    std::string path_;
    std::string version_;
};
class response {
  public:
    response();
    // rename? restructure? builder pattern?
    std::string generate();
    void write(std::istream& s);
  private:
    std::stringstream response_stream_;




};
}
}
