#ifndef FUNC_H
#define FUNC_H

#include "Evalvisitor.h"
#include <vector>
#include <string>
#include <any>
#include <map>

struct Function {
  std::vector<std::pair<std::string, std::any>> parameter;
  Python3Parser::SuiteContext *suite;
  Function(const std::vector<std::pair<std::string, std::any>> &parameter = {}, Python3Parser::SuiteContext *suite = nullptr) : parameter(parameter), suite(suite) {}
};

class FunctionSet {
private:
  std::map<std::string, Function> map;
public:
  void Create(const std::string &, const std::vector<std::any> &);
};

#endif //FUNC_H
