#include <Evalvisitor.h>

std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
  std::string res;
  std::vector<antlr4::tree::ParseTree *> children_array = ctx->children;
  size_t size = children_array.size();
  for (size_t i = 1; i < size - 1; ++i) {
    std::string tmp = children_array[i]->getText();
    if (tmp == "{") {
      ++i;
      std::vector<std::any> val = std::any_cast<std::vector<std::any>>(visit(children_array[i]));
      TryRestore(val[0]);
      res += AnyToString(val[0]);
      ++i;
    } else {
      size_t tmp_size = tmp.size();
      for (size_t j = 0; j < tmp_size; ++j) {
        res += tmp[j];
        if (j < tmp_size - 1 && tmp[j] == tmp[j + 1] && (tmp[j] == '{' || tmp[j] == '}')) {
          ++j;
        }
      }
    }
  }
  return res;
}
