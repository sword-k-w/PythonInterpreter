#include "Evalvisitor.h"
#include <int2048.h>

void TryRestore(std::any &val) {
  if (val.type() == typeid(std::pair<std::string, bool>)) {
    std::pair<std::string, bool> tmp = std::any_cast<std::pair<std::string, bool> &>(val);
    if (tmp.second) {
      val = Scope::GetValue(tmp.first);
    }
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

bool AnyToBool(std::any val) {
  if (val.type() == typeid(std::pair<std::string, bool>)) {
    std::pair<std::string, bool> tmp = std::any_cast<std::pair<std::string, bool> &>(val);
    assert(tmp.second);
    val = Scope::GetValue(tmp.first);
  }
  if (val.type() == typeid(std::string)) {
    return std::any_cast<std::string &>(val) != "";
  } else if (val.type() == typeid(bool)) {
    return std::any_cast<bool &>(val);
  } else if (val.type() == typeid(double)) {
    return std::any_cast<double &>(val) ? true : false;
  } else {
    return !std::any_cast<int2048 &>(val).zero();
  }
}

std::string AnyToString(std::any val) {
  if (val.type() == typeid(std::string)) {
    return std::any_cast<std::string>(val);
  } else if (val.type() == typeid(bool)) {
    return std::any_cast<bool &>(val) ? "True" : "False";
  } else if (val.type() == typeid(double)) {
    return std::to_string(std::any_cast<double &>(val));
  } else {
    return std::string(std::any_cast<int2048 &>(val));
  }
}
