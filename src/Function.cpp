#include <Function.h>
#include <Scope.h>

void FunctionSet::Create(const std::string &name, const std::vector<std::any> &val) {
  const Function &func = map[name];
  Scope::CreatScope();
  size_t val_size = val.size();
  for (size_t i = 0; i < val_size; ++i) {
    Scope::InitValue(func.parameter[i].first, val[i]);
  }
  size_t parameter_size = func.parameter.size();
  for (size_t i = val_size; i < parameter_size; ++i) {
    Scope::InitValue(func.parameter[i].first, func.parameter[i].second);
  }
}

