#include <Evalvisitor.h>
#include <Scope.h>
#include <int2048.h>

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    return visit(ctx->compound_stmt());
  }
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  return visit(ctx->small_stmt());
}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  if (ctx->expr_stmt() != nullptr) {
    return visit(ctx->expr_stmt());
  } else {
    return visit(ctx->flow_stmt());
  }
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  if (ctx->if_stmt() != nullptr) {
    return ctx->if_stmt();
  } else if (ctx->while_stmt() != nullptr) {
    return ctx->while_stmt();
  } else {
    return ctx->funcdef();
  }
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  std::vector<Python3Parser::TestlistContext *> testlist_array = ctx->testlist();
  size_t size = testlist_array.size();
  if (size == 1) {
    return kNothingStmt;
  }

  std::vector<std::any> val_array;
  std::vector<std::any> tmp_array = std::any_cast<std::vector<std::any>>(visit(*testlist_array.rbegin()));
  for (auto &tmp : tmp_array) {
    if (tmp.type() == typeid(std::pair<std::string, bool>)) {
      val_array.emplace_back(Scope::GetValue(std::any_cast<std::pair<std::string, bool> &>(tmp).first));
    } else {
      val_array.emplace_back(tmp);
    }
  }

  size_t first = (ctx->augassign() == nullptr ? 0 : 1);
  size_t list_size = val_array.size();
  for (int i = static_cast<int>(size) - 2; i >= first; --i) {
    tmp_array = std::any_cast<std::vector<std::any>>(visit(testlist_array[i]));
    for (size_t j = 0; j < list_size; ++j) {
      Scope::SetValue(std::any_cast<std::pair<std::string, bool> &>(tmp_array[j]).first, val_array[j]);
    }
  }

  if (first) {
    tmp_array = std::any_cast<std::vector<std::any>>(visit(testlist_array[0]));
    std::string op = std::any_cast<std::string>(visit(ctx->augassign()));
    for (size_t j = 0; j < list_size; ++j) {
      std::string name = std::any_cast<std::pair<std::string, bool &>>(tmp_array[j]).first;
      std::any val = Scope::GetValue(name);

      if (op == "+=") {
        if (val.type() == typeid(std::string)) {
          std::any_cast<std::string &>(val) += std::any_cast<std::string &>(val_array[j]);
        } else if (val.type() == typeid(double)) {
          std::any_cast<double &>(val) += std::any_cast<double &>(val_array[j]);
        } else {
          std::any_cast<int2048 &>(val) += std::any_cast<int2048 &>(val_array[j]);
        }
      } else if (op == "-=") {
        if (val.type() == typeid(double)) {
          std::any_cast<double &>(val) -= std::any_cast<double &>(val_array[j]);
        } else {
          std::any_cast<int2048 &>(val) -= std::any_cast<int2048 &>(val_array[j]);
        }
      } else if (op == "*=") {
        if (val.type() == typeid(std::string)) {
          std::string res = "", &cur = std::any_cast<std::string &>(val);
          int time = std::any_cast<int>(val_array[j]);
          while (time--) {
            res += cur;
          }
          val = res;
        } else if (val.type() == typeid(double)) {
          std::any_cast<double &>(val) *= std::any_cast<double &>(val_array[j]);
        } else {
          std::any_cast<int2048 &>(val) *= std::any_cast<int2048 &>(val_array[j]);
        }
      } else if (op == "/=") {
        std::any_cast<double &>(val) /= std::any_cast<double &>(val_array[j]);
      } else if (op == "//=") {
        std::any_cast<int2048 &>(val) /= std::any_cast<int2048 &>(val_array[j]);
      } else {
        std::any_cast<int2048 &>(val) %= std::any_cast<int2048 &>(val_array[j]);
      }

      Scope::SetValue(name, val);
    }
  }
  return kNothingStmt;
}

