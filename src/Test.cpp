#include <Evalvisitor.h>
#include <int2048.h>

std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  // std::cerr << "Test!\n";
  return visit(ctx->or_test());
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
  // std::cerr << "Testlist!\n";
  std::vector<std::any> res;
  std::vector<Python3Parser::TestContext *> test_array = ctx->test();
  for (auto &son : test_array) {
    res.emplace_back(visit(son));
  }
  return res;
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  // std::cerr << "Or_test!\n";
  std::vector<Python3Parser::And_testContext *> and_array = ctx->and_test();
  std::any val = visit(and_array[0]);
  size_t size = and_array.size();
  if (size == 1) {
    return val;
  }
  TryRestore(val);
  if (!std::any_cast<bool &>(val)) {
    return false;
  }
  for (size_t i = 1; i < size; ++i) {
    val = visit(and_array[i]);
    TryRestore(val);
    if (!std::any_cast<bool &>(val)) {
      return false;
    }
  }
  return true;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  // std::cerr << "And_test!\n";
  std::vector<Python3Parser::Not_testContext *> not_array = ctx->not_test();
  std::any val = visit(not_array[0]);
  size_t size = not_array.size();
  if (size == 1) {
    return val;
  }
  TryRestore(val);
  if (!std::any_cast<bool &>(val)) {
    return false;
  }
  for (size_t i = 0; i < size; ++i) {
    val = visit(not_array[i]);
    TryRestore(val);
    if (!std::any_cast<bool &>(val)) {
      return false;
    }
  }
  return true;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  // std::cerr << "Not_test!\n";
  if (ctx->NOT() != nullptr) {
    std::any val = visit(ctx->not_test());
    TryRestore(val);
    return !std::any_cast<bool &>(val);
  } else {
    return visit(ctx->comparison());
  }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  // std::cerr << "Comparison!\n";
  std::vector<Python3Parser::Arith_exprContext *> arith_array = ctx->arith_expr();
  std::any val = visit(arith_array[0]);
  size_t size = arith_array.size();
  if (size == 1) {
    return val;
  }
  TryRestore(val);
  std::vector<Python3Parser::Comp_opContext *> op_array = ctx->comp_op();
  for (size_t i = 1; i < size; ++i) {
    std::any nxt = visit(arith_array[i]);
    TryRestore(nxt);
    std::string cur_op = std::any_cast<std::string>(visit(op_array[i - 1]));
    if (cur_op == "<") {
      if (val.type() == typeid(std::string)) {
        if (std::any_cast<std::string &>(val) >= std::any_cast<std::string &>(nxt)) {
          return false;
        }
      } else if (val.type() == typeid(double)) {
        if (std::any_cast<double &>(val) >= std::any_cast<double &>(nxt)) {
          return false;
        }
      } else {
        if (std::any_cast<int2048 &>(val) >= std::any_cast<int2048 &>(nxt)) {
          return false;
        }
      }
    } else if (cur_op == ">") {
      if (val.type() == typeid(std::string)) {
        if (std::any_cast<std::string &>(val) <= std::any_cast<std::string &>(nxt)) {
          return false;
        }
      } else if (val.type() == typeid(double)) {
        if (std::any_cast<double &>(val) <= std::any_cast<double &>(nxt)) {
          return false;
        }
      } else {
        if (std::any_cast<int2048 &>(val) <= std::any_cast<int2048 &>(nxt)) {
          return false;
        }
      }
    } else if (cur_op == ">=") {
      if (val.type() == typeid(std::string)) {
        if (std::any_cast<std::string &>(val) < std::any_cast<std::string &>(nxt)) {
          return false;
        }
      } else if (val.type() == typeid(double)) {
        if (std::any_cast<double &>(val) < std::any_cast<double &>(nxt)) {
          return false;
        }
      } else {
        if (std::any_cast<int2048 &>(val) < std::any_cast<int2048 &>(nxt)) {
          return false;
        }
      }
    } else if (cur_op == "<=") {
      if (val.type() == typeid(std::string)) {
        if (std::any_cast<std::string &>(val) > std::any_cast<std::string &>(nxt)) {
          return false;
        }
      } else if (val.type() == typeid(double)) {
        if (std::any_cast<double &>(val) > std::any_cast<double &>(nxt)) {
          return false;
        }
      } else {
        if (std::any_cast<int2048 &>(val) > std::any_cast<int2048 &>(nxt)) {
          return false;
        }
      }
    } else {
      if (val.type() == typeid(std::string) && nxt.type() != typeid(std::string)) {
        return false;
      }
      if (val.type() != typeid(std::string) && nxt.type() == typeid(std::string)) {
        return false;
      }
      if (cur_op == "==") {
        if (val.type() == typeid(std::string)) {
          return std::any_cast<std::string &>(val) == std::any_cast<std::string &>(nxt);
        } else if (val.type() == typeid(double) || nxt.type() == typeid(double)) {
          return std::any_cast<double &>(val) == std::any_cast<double &>(nxt);
        } else {
          return std::any_cast<int2048 &>(val) == std::any_cast<int2048 &>(nxt);
        }
      } else {
        if (val.type() == typeid(std::string)) {
          return std::any_cast<std::string &>(val) != std::any_cast<std::string &>(nxt);
        } else if (val.type() == typeid(double) || nxt.type() == typeid(double)) {
          return std::any_cast<double &>(val) != std::any_cast<double &>(nxt);
        } else {
          return std::any_cast<int2048 &>(val) != std::any_cast<int2048 &>(nxt);
        }
      }
    }
    val = nxt;
  }
  return true;
}

std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  return (*ctx->children.begin())->getText();
}
