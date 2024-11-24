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
  std::cerr << "Arglist!\n";
  std::vector<Python3Parser::ArgumentContext *> arg_array = ctx->argument();
  std::vector<std::any> res;
  for (auto &arg : arg_array) {
    res.emplace_back(visit(arg));
  }
  return res;
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  std::cerr << "Argument!\n";
  std::vector<Python3Parser::TestContext *> test_array = ctx->test();
  std::any tmp = visit(test_array[0]);
  if (tmp.type() == typeid(std::pair<std::string, bool>)) {
    std::string name = std::any_cast<std::pair<std::string, bool>>(tmp).first;
    if (name == "None") {
      return name;
    }
    if (test_array.size() == 1) {
      return Scope::GetValue(name);
    } else {
      std::any val = visit(test_array[1]);
      Scope::SetValue(name, val);
      return val;
    }
  } else {
    assert(test_array.size() == 1);
    return tmp;
  }
}


