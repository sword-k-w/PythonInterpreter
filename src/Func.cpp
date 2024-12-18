#include <Evalvisitor.h>
#include <Function.h>
#include <Scope.h>

std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
  FunctionSet::AddFunction(ctx->NAME()->getText(), Function(std::any_cast<std::vector<std::pair<std::string, std::any>>>(visit(ctx->parameters())), ctx->suite()));
  return kNothingStmt;
}

std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
  if (ctx->typedargslist() != nullptr) {
    return visit(ctx->typedargslist());
  } else {
    return std::vector<std::pair<std::string, std::any>>({});
  }
}

std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
  std::vector<Python3Parser::TfpdefContext *> variable = ctx->tfpdef();
  std::vector<Python3Parser::TestContext *> default_value = ctx->test();

  size_t var_size = variable.size();
  size_t def_size = default_value.size();
  size_t delta = var_size - def_size;
  std::vector<std::pair<std::string, std::any>> res;
  for (size_t i = 0; i < delta; ++i) {
    res.emplace_back(std::any_cast<std::string>(visit(variable[i])), 0);
  }
  for (size_t i = 0; i < def_size; ++i) {
    res.emplace_back(std::any_cast<std::string>(visit(variable[i + delta])), visit(default_value[i]));
  }
  return res;
}

std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
  return ctx->NAME()->getText();
}

std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  std::vector<Python3Parser::ArgumentContext *> arg_array = ctx->argument();
  std::vector<std::any> res;
  for (auto &arg : arg_array) {
    std::any val = visit(arg);
    if (val.type() == typeid(std::vector<std::any>)) {
      std::vector<std::any> tmp = std::any_cast<std::vector<std::any> &>(val);
      for (auto &x : tmp) {
        res.emplace_back(x);
      }
    } else {
      res.emplace_back(val);
    }
  }
  return res;
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  std::vector<Python3Parser::TestContext *> test_array = ctx->test();
  if (test_array.size() == 1) {
    std::any val = visit(test_array[0]);
    TryRestore(val);
    return val;
  } else {
    std::any val = visit(test_array[1]);
    TryRestore(val);
    return std::make_pair(visit(test_array[0]), val);
  }
}


