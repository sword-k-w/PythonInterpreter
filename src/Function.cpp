#include <Function.h>
#include <Scope.h>
#include <Evalvisitor.h>

std::map<std::string, Function> FunctionSet::map;

Python3Parser::SuiteContext *FunctionSet::Create(const std::string &name, std::vector<std::any> val) {
  const Function &func = map[name];
  Scope::CreatScope();
  size_t val_size = val.size();
  std::set<std::string> visit;
  for (size_t i = 0; i < val_size; ++i) {
    if (val[i].type() == typeid(std::any)) {
      TryRestore(val[i]);
      visit.emplace(func.parameter[i].first);
      Scope::InitValue(func.parameter[i].first, val[i]);
    } else {
      MyAssert(val[i].type() == typeid(std::pair<std::any, std::any>));
      std::pair<std::any, std::any> tmp = std::any_cast<std::pair<std::any, std::any>>(val[i]);
      std::string name = std::any_cast<std::pair<std::string, bool>>(tmp.first).first;
      visit.emplace(name);
      Scope::InitValue(name, tmp.second);
    }
  }
  size_t parameter_size = func.parameter.size();
  for (size_t i = 0; i < parameter_size; ++i) {
    if (visit.find(func.parameter[i].first) == visit.end()) {
      Scope::InitValue(func.parameter[i].first, func.parameter[i].second);
    }
  }
  return func.suite;
}

void FunctionSet::AddFunction(const std::string &name, const Function &func) {
  map[name] = func;
}
