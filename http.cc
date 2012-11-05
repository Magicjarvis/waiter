#include <algorithm>
#include <iterator>
#include <iostream>
#include "http.h"

using std::vector;
using std::string;
using std::unordered_map;

namespace waiter {
namespace http {

vector<string> request::spaceSplit(std::string line) {
  vector<string> vals;
  string one, two;
  std::stringstream ss(line);
  ss >> one;
  ss >> two;
  vals.push_back(one);
  vals.push_back(two);
  std::cout << one << "," << two << "added to the map" << std::endl;
  return vals;
}

request request::reqFromString(string str_req) {
  unordered_map<string, string> field_map;
  vector<string> lines = getLines(str_req);
  std::stringstream req_line(lines[0]);
  string method, path, version;
  req_line >> method;
  req_line >> path;
  req_line >> version;

  // GET /foo HTTP/1.1
  for (unsigned int i = 1; i < lines.size(); i++) {
    vector<string> entry = spaceSplit(lines[i]);
    field_map.emplace(entry[0], entry[1]); 
  }
  return request(method, path, version, field_map);
  
}

std::ostream& operator<<(std::ostream &os, const request &req) {
  os << "Method, Path, version" << req.method() << ", " << req.path() << "," << req.version() << std::endl;
  std::unordered_map<std::string, std::string> fields = req.fields();
  for (auto it = fields.begin(); it != fields.end(); ++it) {
    os << it->first << " : " << it->second << std::endl; 
  }
  return os;
}

//likely inefficient way of doing this
vector<string> request::getLines(string str) {                              
  std::istringstream ss(str);                                                
  vector<string> lines;                                                        
  std::string line;
  while (getline(ss, line)) {
    lines.push_back(line); 
  }
  return lines;                                                                
}    

request::request(string meth, string route, string ver,
    std::unordered_map<string, string> field_map) : fields_(field_map), method_(meth), path_(route), version_(ver) {}

response::response() : response_stream_() {}
void response::write(std::istream& s) {
  string str((std::istreambuf_iterator<char>(s)),
      std::istreambuf_iterator<char>());
  response_stream_ << str;
}
string response::generate() {
  return "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\n\n"
    + response_stream_.str();
}
}
}
