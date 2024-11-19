#include <Evalvisitor.h>
#include <int2048.h>
#include <wchar.h>

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  std::vector<Python3Parser::TermContext *> term_array = ctx->term();
  std::any val = visit(term_array[0]);
  size_t size = term_array.size();
  if (size == 1) {
    return val;
  }
  std::vector<Python3Parser::Addorsub_opContext *> op_array = ctx->addorsub_op();
  if (val.type() == typeid(bool)) {
    val = std::any_cast<int2048>(val);
  }
  for (size_t i = 1; i < size; ++i) {
    if (std::any_cast<std::string>(visit(op_array[i - 1])) == "+") {
      if (val.type() == typeid(std::string)) {
        std::any_cast<std::string &>(val) += std::any_cast<std::string &>(visit(term_array[i]));
      } else if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) += std::any_cast<double &>(visit(term_array[i]));
      } else {
        std::any_cast<int2048 &>(val) += std::any_cast<int2048 &>(visit(term_array[i]));
      }
    } else {
      if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) -= std::any_cast<double &>(visit(term_array[i]));
      } else {
        std::any_cast<int2048 &>(val) -= std::any_cast<int2048 &>(visit(term_array[i]));
      }
    }
  }
  return val;
}

std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
  return (*ctx->children.begin())->getText();
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  std::vector<Python3Parser::FactorContext *> factor_array = ctx->factor();
  std::any val = visit(factor_array[0]);
  size_t size = factor_array.size();
  if (size == 1) {
    return val;
  }
  std::vector<Python3Parser::Muldivmod_opContext *> op_array = ctx->muldivmod_op();
  for (size_t i = 1; i < size; ++i) {
    std::string cur_op = std::any_cast<std::string &>(visit(op_array[i - 1]));
    if (cur_op == "*") {
      if (val.type() == typeid(std::string)) {
        std::string res = "", &cur = std::any_cast<std::string &>(val);
        int time = std::any_cast<int>(visit(factor_array[i]));
        while (time--) {
          res += cur;
        }
        val = res;
      } else if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) *= std::any_cast<double &>(visit(factor_array[i]));
      } else {
        std::any_cast<int2048 &>(val) *= std::any_cast<int2048 &>(visit(factor_array[i]));
      }
    } else if (cur_op == "/") {
      std::any_cast<double &>(val) /= std::any_cast<double &>(visit(factor_array[i]));
    } else if (cur_op == "//") {
      std::any_cast<int2048 &>(val) /= std::any_cast<int2048 &>(visit(factor_array[i]));
    } else {
      std::any_cast<int2048 &>(val) %= std::any_cast<int2048 &>(visit(factor_array[i]));
    }
  }
  return val;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  std::string op = (*ctx->children.begin())->getText();
  if (op == "+") {
    return visit(ctx->factor());
  } else if (op == "-") {
    std::any val = visit(ctx->factor());
    if (val.type() == typeid(double)) {
      return -std::any_cast<double &>(val);
    } else {
      return -std::any_cast<int2048 &>(val);
    }
  } else {
    return visit(ctx->atom_expr());
  }
}

std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
  return (*ctx->children.begin())->getText();
}

std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
  return (*ctx->children.begin())->getText();
}