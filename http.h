#include <sstream>
#include <vector>
#include <unordered_map>

namespace waiter {
namespace http {
/**
 * http request object
 */
class request {
  /**
   * split a request line at a space character into two strings (field name and 
   * field value)
   * @param line the line to be split
   * @return vector<string> containing both parts of the split line
   */
  static std::vector<std::string> spaceSplit(std::string line);

  /**
   * extract each line from a request string and put them in a vector
   * @param req request string
   * @return vector<string> of each line in the request string
   */
  static std::vector<std::string> getLines(std::string req);

  /**
   * send the request object to an ostream
   * @param os ostream
   * @param req request object
   * @return the stream with the request object added
   */
  friend std::ostream& operator<<(std::ostream &os, const request &req); 

  public:
  /**
   * create a request object from a string
   * @param str request object in stream form
   * @return constructed request object
   */
  static request reqFromString(std::string str);

    /// get method
    inline std::string method() const {
      return method_;
    }

    /// get path
    inline std::string path() const {
      return path_;
    }

    /// get version
    inline std::string version() const {
      return version_;
    }

    /// get fields map
    inline std::unordered_map<std::string, std::string> fields() const {
      return fields_;
    }

  private:
    /**
     * constructs an http requests
     */
    request(std::string meth, std::string route, 
        std::string ver, std::unordered_map<std::string, std::string> fields);
    
    // request fields
    std::unordered_map<std::string, std::string> fields_;
    std::string method_;
    std::string path_;
    std::string version_;
};

/**
 * http response object
 */
class response {
  public:
    /**
     * constructor
     */
    response();

    /**
     * generate an http response stream with appropriate header and content from 
     * response_stream
     */
    std::string generate();

    /**
     *
     */
    void setContentType(std::string ext);

    /**
     * fill response stream with content from an istream
     * @param s istream with content to be added to reponse_stream
     */
    void write(std::istream& s);
  private:
    std::string content_type_;
    std::stringstream response_stream_;
};

}
}
