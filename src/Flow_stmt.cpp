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

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  if (ctx->testlist() != nullptr) {
    return visit(ctx->testlist());
  } else {
    return kReturnVoidStmt;
  }
}