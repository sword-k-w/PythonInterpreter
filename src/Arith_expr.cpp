#include <Evalvisitor.h>
#include <Function.h>
#include <int2048.h>
#include <iomanip>
#include <Scope.h>
#include <iostream>

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  // std::cerr << "Arith_expr!\n";
  std::vector<Python3Parser::TermContext *> term_array = ctx->term();
  std::any val = visit(term_array[0]);
  size_t size = term_array.size();
  if (size == 1) {
    return val;
  }
  std::vector<Python3Parser::Addorsub_opContext *> op_array = ctx->addorsub_op();
  TryRestore(val);
  for (size_t i = 1; i < size; ++i) {
    std::any val_prime = visit(term_array[i]);
    TryRestore(val_prime);
    if (std::any_cast<std::string>(visit(op_array[i - 1])) == "+") {
      if (val.type() == typeid(std::string)) {
        std::any_cast<std::string &>(val) += std::any_cast<std::string &>(val_prime);
      } else if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) += AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) += AnyToInt(val_prime);
      }
    } else {
      if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) -= AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) -= AnyToInt(val_prime);
      }
    }
  }
  return val;
}

std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
  return (*ctx->children.begin())->getText();
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  // std::cerr << "Term!\n";
  std::vector<Python3Parser::FactorContext *> factor_array = ctx->factor();
  std::any val = visit(factor_array[0]);
  size_t size = factor_array.size();
  if (size == 1) {
    return val;
  }
  std::vector<Python3Parser::Muldivmod_opContext *> op_array = ctx->muldivmod_op();
  TryRestore(val);
  for (size_t i = 1; i < size; ++i) {
    std::string cur_op = std::any_cast<std::string>(visit(op_array[i - 1]));
    std::any val_prime = visit(factor_array[i]);
    TryRestore(val_prime);
    if (cur_op == "*") {
      if (val.type() == typeid(std::string)) {
        std::string res = "", &cur = std::any_cast<std::string &>(val);
        int2048 time = AnyToInt(val_prime);
        while (true) {
          --time;
          res += cur;
          if (time.zero()) {
            break;
          }
        }
        val = res;
      } else if (val.type() == typeid(double)) {
        std::any_cast<double &>(val) *= AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) *= AnyToInt(val_prime);
      }
    } else if (cur_op == "/") {
      val = AnyToFloat(val);
      std::any_cast<double &>(val) /= AnyToFloat(val_prime);
    } else if (cur_op == "//") {
      val = AnyToInt(val);
      std::any_cast<int2048 &>(val) /= AnyToInt(val_prime);
    } else {
      val = AnyToInt(val);
      std::any_cast<int2048 &>(val) %= AnyToInt(val_prime);
    }
  }
  return val;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  // std::cerr << "Factor!\n";
  if (ctx->ADD() != nullptr) {
    std::any val = visit(ctx->factor());
    TryRestore(val);
    return val;
  } else if (ctx->MINUS() != nullptr) {
    std::any val = visit(ctx->factor());
    TryRestore(val);
    if (val.type() == typeid(double)) {
      return -std::any_cast<double &>(val);
    } else {
      return -AnyToInt(val);
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

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  // std::cerr << "Atom_expr!\n";
  if (ctx->trailer() == nullptr) {
    return visit(ctx->atom());
  } else {
    std::string name = std::any_cast<std::pair<std::string, bool>>(visit(ctx->atom())).first;
    std::vector<std::any> val_array = std::any_cast<std::vector<std::any>>(visit(ctx->trailer()));
    size_t size = val_array.size();
    if (name == "print") {
      for (size_t i = 0; i < size; ++i) {
        if (val_array[i].type() == typeid(std::pair<std::string, bool>)) {
          std::cout << "None";
        } else if (val_array[i].type() == typeid(std::string)) {
          std::cout << std::any_cast<std::string &>(val_array[i]);
        } else if (val_array[i].type() == typeid(bool)) {
          std::cout << (std::any_cast<bool &>(val_array[i]) ? "True" : "False");
        } else if (val_array[i].type() == typeid(double)) {
          std::cout << std::fixed << std::setprecision(6) << std::any_cast<double &>(val_array[i]);
        } else {
          assert(val_array[i].type() == typeid(int2048));
          std::cout << std::any_cast<int2048 &>(val_array[i]);
        }
        if (i == size - 1) {
          std::cout << '\n';
        } else {
          std::cout << ' ';
        }
      }
      return kNothingStmt;
    } else if (name == "int") {
      return AnyToInt(val_array[0]);
    } else if (name == "float") {
      return AnyToFloat(val_array[0]);
    } else if (name == "str") {
      return AnyToString(val_array[0]);
    } else if (name == "bool") {
      return AnyToBool(val_array[0]);
    } else {
      std::any res = visit(FunctionSet::Create(name, val_array));
      Scope::DeleteScope();
      return res;
    }
  }
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  // std::cerr << "Trailer!\n";
  if (ctx->arglist() != nullptr) {
    return visit(ctx->arglist());
  } else {
    return std::vector<std::any>({});
  }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  // std::cerr << "Atom!\n";
  if (ctx->NAME() != nullptr) {
    // std::cerr << "#" << ctx->NAME()->getText() << '\n';
    return std::make_pair(ctx->NAME()->getText(), true);
  } else if (ctx->NUMBER() != nullptr) {
    std::string val = ctx->NUMBER()->getText();
    size_t size = val.size();
    bool flag = false;
    for (size_t i = 0; i < size; ++i) {
      if (val[i] == '.') {
        flag = true;
        break;
      }
    }
    if (flag) {
      return AnyToFloat(val);
    } else {
      return AnyToInt(val);
    }
  } else if (ctx->NONE() != nullptr) {
    return std::make_pair(std::string("None"), false);
  } else if (ctx->TRUE() != nullptr) {
    return true;
  } else if (ctx->FALSE() != nullptr) {
    return false;
  } else if (ctx->test() != nullptr) {
    return visit(ctx->test());
  } else if (ctx->format_string() != nullptr) {
    return visit(ctx->format_string());
  } else {
    std::vector<antlr4::tree::TerminalNode *> string_array = ctx->STRING();
    std::string res;
    for (auto &string : string_array) {
      std::string tmp = string->getText();
      size_t size = tmp.size();
      for (size_t i = 1; i < size - 1; ++i) {
        res += tmp[i];
      }
    }
    return res;
  }
}

