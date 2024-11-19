#include <Evalvisitor.h>

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  std::vector<Python3Parser::TestContext *> test_array = ctx->test();
  std::vector<Python3Parser::SuiteContext *> suite_array = ctx->suite();
  size_t size = test_array.size();
  for (size_t i = 0; i < size; ++i) {
    if (std::any_cast<bool>(visit(test_array[i]))) {
      std::any val = visit(suite_array[i]);
      if (val.type() != typeid(std::string)) {
        return val;
      } else {
        std::string info = std::any_cast<std::string>(val);
        if (info != kNothingStmt) {
          return info;
        }
      }
    }
  }
  if (suite_array.size() > size) {
    return visit(suite_array[size]);
  } else {
    return kNothingStmt;
  }
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  while (std::any_cast<bool>(visit(ctx->test()))) {
    std::any val = visit(ctx->suite());
    if (val.type() != typeid(std::string)) {
      return val;
    } else {
      std::string info = std::any_cast<std::string>(val);
      if (info == kContinueStmt) {
        continue;
      } else if (info == kBreakStmt) {
        break;
      } else if (info != kNothingStmt) {
        return info;
      }
    }
  }
  return kNothingStmt;
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    std::vector<Python3Parser::StmtContext *> stmt_array = ctx->stmt();
    for (auto &stmt : stmt_array) {
      std::any val = visit(stmt);
      if (val.type() != typeid(std::string)) {
        return val;
      } else {
        std::string info = std::any_cast<std::string>(val);
        if (info != kNothingStmt) {
          return info;
        }
      }
    }
    return kNothingStmt;
  }
}
