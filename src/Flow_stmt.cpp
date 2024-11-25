#include <Evalvisitor.h>

std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  if (ctx->break_stmt() != nullptr) {
    return visit(ctx->break_stmt());
  } else if (ctx->continue_stmt() != nullptr) {
    return visit(ctx->continue_stmt());
  } else {
    return visit(ctx->return_stmt());
  }
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  return kBreakStmt;
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  return kContinueStmt;
}

/**
 * @return if "return", then return kReturnVoidStmt; if "return" a single value, then return std::any; otherwise, return std::vector<std::any>
 */
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  if (ctx->testlist() != nullptr) {
    std::vector<std::any> val_array = std::any_cast<std::vector<std::any>>(visit(ctx->testlist()));
    std::vector<std::any> res;
    for (auto &val : val_array) {
      if (val.type() == typeid(std::pair<std::string, bool>)) {
        std::pair<std::string, bool> tmp = std::any_cast<std::pair<std::string, bool> &>(val);
        if (tmp.second) {
          res.emplace_back(Scope::GetValue(tmp.first));
        } else {
          res.emplace_back(val);
        }
      } else {
        res.emplace_back(val);
      }
    }
    if (res.size() == 1) {
      return res[0];
    } else {
      return res;
    }
  } else {
    return kReturnVoidStmt;
  }
}