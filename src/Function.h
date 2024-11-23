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
  static std::map<std::string, Function> map;
public:
  static Python3Parser::SuiteContext *Create(const std::string &, const std::vector<std::any> &);
  static void AddFunction(const std::string &, const Function &);
};

#endif //FUNC_H
