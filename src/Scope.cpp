#include <Scope.h>
#include <cassert>

std::map<std::string, std::any> Scope::global_;
std::stack<std::map<std::string, std::any>> Scope::local_;

std::any Scope::GetValue(const std::string &key) {
  if (!local_.empty() && local_.top().find(key) != local_.top().end()) {
    return local_.top()[key];
  } else {
    return global_[key];
  }
}

void Scope::SetValue(const std::string &key, const std::any &val) {
  if (!local_.empty() && local_.top().find(key) != local_.top().end()) {
    local_.top()[key] = val;
  } else if (global_.find(key) != global_.end()) {
    global_[key] = val;
  } else {
    local_.top()[key] = val;
  }
}

void Scope::InitValue(const std::string &key, const std::any &val) {
  local_.top()[key] = val;
}

void Scope::CreatScope() {
  local_.emplace();
}

void Scope::DeleteScope() {
  assert(!local_.empty());
  local_.pop();
}
