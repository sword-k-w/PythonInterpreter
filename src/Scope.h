#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <stack>
#include <string>
#include <any>

class Scope {
private:
  static std::map<std::string, std::any> global_;
  static std::stack<std::map<std::string, std::any>> local_;
public:
  static std::any GetValue(const std::string &);
  static void SetValue(const std::string &, const std::any &);
  static void InitValue(const std::string &, const std::any &);
  static void CreatScope();
  static void DeleteScope();
};

#endif //SCOPE_H
