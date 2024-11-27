#include <Evalvisitor.h>
#include <Scope.h>
#include <int2048.h>

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  // std::cerr << "Stmt!\n";
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    return visit(ctx->compound_stmt());
  }
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  // std::cerr << "Simple_stmt!\n";
  return visit(ctx->small_stmt());
}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  // std::cerr << "Small_stmt!\n";
  if (ctx->expr_stmt() != nullptr) {
    return visit(ctx->expr_stmt());
  } else {
    return visit(ctx->flow_stmt());
  }
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  // std::cerr << "Compound_stmt!\n";
  if (ctx->if_stmt() != nullptr) {
    return visit(ctx->if_stmt());
  } else if (ctx->while_stmt() != nullptr) {
    return visit(ctx->while_stmt());
  } else {
    return visit(ctx->funcdef());
  }
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  // std::cerr << "Expr_stmt!\n";
  std::vector<Python3Parser::TestlistContext *> testlist_array = ctx->testlist();
  size_t size = testlist_array.size();
  if (size == 1) {
    visit(testlist_array[0]);
    return kNothingStmt;
  }

  if (ctx->augassign() == nullptr) {
    std::vector<std::any> cur_array;
    std::vector<std::any> val_array = std::any_cast<std::vector<std::any>>(visit(*testlist_array.rbegin()));
    size_t list_size = val_array.size();
    for (int i = static_cast<int>(size) - 2; i >= 0; --i) {
      cur_array = std::any_cast<std::vector<std::any>>(visit(testlist_array[i]));
      for (size_t j = 0; j < list_size; ++j) {
        std::any val = val_array[j];
        TryRestore(val);
        Scope::SetValue(std::any_cast<std::pair<std::string, bool> &>(cur_array[j]).first, val);
      }
      val_array = cur_array;
    }
  } else {
    std::any val_prime = std::any_cast<std::vector<std::any>>(visit(testlist_array[1]))[0];
    TryRestore(val_prime);
    std::string op = std::any_cast<std::string>(visit(ctx->augassign()));
    std::string name = std::any_cast<std::pair<std::string, bool> &>(std::any_cast<std::vector<std::any>>(visit(testlist_array[0]))[0]).first;
    std::any val = Scope::GetValue(name);

    if (op == "+=") {
      if (val.type() == typeid(std::string)) {
        std::any_cast<std::string &>(val) += std::any_cast<std::string &>(val_prime);
      } else if (val.type() == typeid(double) || val_prime.type() == typeid(double)) {
        val = AnyToFloat(val);
        std::any_cast<double &>(val) += AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) += AnyToInt(val_prime);
      }
    } else if (op == "-=") {
      if (val.type() == typeid(double) || val_prime.type() == typeid(double)) {
        val = AnyToFloat(val);
        std::any_cast<double &>(val) -= AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) -= AnyToInt(val_prime);
      }
    } else if (op == "*=") {
      if (val.type() == typeid(std::string) && val_prime.type() != typeid(std::string)) {
        std::string res = "", &cur = std::any_cast<std::string &>(val);
        int2048 time = AnyToInt(val_prime);
        if (!time.zero() && !time.negative()) {
          while (!time.zero()) {
            res += cur;
            --time;
          }
        }
        val = res;
      } else if (val_prime.type() == typeid(std::string) && val.type() != typeid(std::string)) {
        std::string res = "", &cur = std::any_cast<std::string &>(val_prime);
        int2048 time = AnyToInt(val);
        if (!time.zero() && !time.negative()) {
          while (!time.zero()) {
            res += cur;
            --time;
          }
        }
        val = res;
      } else if (val.type() == typeid(double) || val_prime.type() == typeid(double)) {
        val = AnyToFloat(val);
        std::any_cast<double &>(val) *= AnyToFloat(val_prime);
      } else {
        val = AnyToInt(val);
        std::any_cast<int2048 &>(val) *= AnyToInt(val_prime);
      }
    } else if (op == "/=") {
      val = AnyToFloat(val);
      std::any_cast<double &>(val) /= AnyToFloat(val_prime);
    } else if (op == "//=") {
      val = AnyToInt(val);
      std::any_cast<int2048 &>(val) /= AnyToInt(val_prime);
    } else {
      val = AnyToInt(val);
      std::any_cast<int2048 &>(val) %= AnyToInt(val_prime);
    }

    Scope::SetValue(name, val);
  }
  return kNothingStmt;
}