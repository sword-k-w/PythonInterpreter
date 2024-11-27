#include "Evalvisitor.h"
#include <int2048.h>
#include <iomanip>

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
  int w = 1;
  size_t first = 0;
  if (val[0] == '-') {
    ++first;
    w = -1;
  }
  for (size_t i = first; i < size; ++i) {
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
  return res ? res * w : res;
}

bool AnyToBool(std::any val) {
  TryRestore(val);
  if (val.type() == typeid(std::string)) {
    return std::any_cast<std::string &>(val) != "";
  } else if (val.type() == typeid(bool)) {
    return std::any_cast<bool &>(val);
  } else if (val.type() == typeid(double)) {
    return std::any_cast<double &>(val) ? true : false;
  } else {
    assert(val.type() == typeid(int2048));
    return !std::any_cast<int2048 &>(val).zero();
  }
}

std::string AnyToString(std::any val) {
  TryRestore(val);
  if (val.type() == typeid(std::pair<std::string, bool>)) {
    return std::string("None");
  } else if (val.type() == typeid(std::string)) {
    return std::any_cast<std::string>(val);
  } else if (val.type() == typeid(bool)) {
    return std::any_cast<bool &>(val) ? "True" : "False";
  } else if (val.type() == typeid(double)) {
    std::stringstream tmp;
    tmp << std::fixed << std::setprecision(6) << std::any_cast<double &>(val);
    return tmp.str();
  } else {
    assert(val.type() == typeid(int2048));
    return std::string(std::any_cast<int2048 &>(val));
  }
}

int2048 AnyToInt(std::any val) {
  TryRestore(val);
  if (val.type() == typeid(std::string)) {
    return int2048(std::any_cast<std::string &>(val));
  } else if (val.type() == typeid(bool)) {
    return int2048(std::any_cast<bool &>(val) ? 1ll : 0ll);
  } else if (val.type() == typeid(double)) {
    return int2048(std::any_cast<double &>(val));
  } else {
    assert(val.type() == typeid(int2048));
    return std::any_cast<int2048 &>(val);
  }
}

double AnyToFloat(std::any val) {
  TryRestore(val);
  if (val.type() == typeid(std::string)) {
    return StringToDouble(std::any_cast<std::string &>(val));
  } else if (val.type() == typeid(bool)) {
    return std::any_cast<bool &>(val) ? 1.0 : 0.0;
  } else if (val.type() == typeid(double)) {
    return std::any_cast<double &>(val);
  } else {
    assert(val.type() == typeid(int2048));
    return double(std::any_cast<int2048 &>(val));
  }
}

void MyAssert(bool x) {
  if (!x) {
    exit(1);
  }
}
