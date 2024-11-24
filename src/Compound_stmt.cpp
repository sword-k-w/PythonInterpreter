#include <Evalvisitor.h>

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  // std::cerr << "If_stmt!\n";
  std::vector<Python3Parser::TestContext *> test_array = ctx->test();
  std::vector<Python3Parser::SuiteContext *> suite_array = ctx->suite();
  size_t size = test_array.size();
  for (size_t i = 0; i < size; ++i) {
    if (AnyToBool(visit(test_array[i]))) {
      return visit(suite_array[i]);
    }
  }
  if (suite_array.size() > size) {
    return visit(suite_array[size]);
  } else {
    return kNothingStmt;
  }
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  // std::cerr << "While_stmt!\n";
  while (AnyToBool(visit(ctx->test()))) {
    std::any val = visit(ctx->suite());
    if (val.type() != typeid(std::string_view)) {
      return val;
    } else if (std::any_cast<std::string_view>(val) == kBreakStmt) {
      break;
    }
  }
  return kNothingStmt;
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  // std::cerr << "Suite!\n";
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    std::vector<Python3Parser::StmtContext *> stmt_array = ctx->stmt();
    for (auto &stmt : stmt_array) {
      std::any val = visit(stmt);
      if (val.type() != typeid(std::string_view)) {
        return val;
      } else {
        std::string_view info = std::any_cast<std::string_view>(val);
        if (info != kNothingStmt) {
          return info;
        }
      }
    }
    return kNothingStmt;
  }
}
