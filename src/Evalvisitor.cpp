#include "Evalvisitor.h"

void TryRestore(std::any &val) {
  if (val.type() == typeid(std::pair<std::string, bool>)) {
    val = Scope::GetValue(std::any_cast<std::pair<std::string, bool> &>(val).first);
  }
}

double StringToDouble(const std::string &val) {
  size_t size = val.size();
  double res = 0;
  for (size_t i = 0; i < size; ++i) {
    if (val[i] == '.') {
      continue;
    }
    res = res * 10 + val[i] - '0';
  }
  size_t pos = size - 1;
  while (val[pos] != '.') {
    res /= 10;
    --pos;
  }
  return res;
}