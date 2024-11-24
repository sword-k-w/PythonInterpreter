// 你需要提交这份代码
#pragma once

#include "visitor.h"

struct calculator : visitor {
    /// TODO: 完成所有需求
    std::any visit_num(num_node *) override;
    std::any visit_add(add_node *) override;
    std::any visit_sub(sub_node *) override;
    std::any visit_mul(mul_node *) override;
    std::any visit_div(div_node *) override;
    ~calculator() override = default;
};

std::any calculator::visit_num(num_node *cur) {
  return cur->number;
}

std::any calculator::visit_add(add_node *cur) {
  std::any l_val = cur->lnode->accept(this);
  std::any r_val = cur->rnode->accept(this);
  if (l_val.type() == typeid(double)) {
    double &l_double_val = std::any_cast<double &> (l_val);
    double &r_double_val = std::any_cast<double &> (r_val);
    return l_double_val + r_double_val;
  } else {
    long long &l_ll_val = std::any_cast<long long &> (l_val);
    long long &r_ll_val = std::any_cast<long long &> (r_val);
    return l_ll_val + r_ll_val;
  }
}

std::any calculator::visit_sub(sub_node *cur) {
  std::any l_val = cur->lnode->accept(this);
  std::any r_val = cur->rnode->accept(this);
  if (l_val.type() == typeid(double)) {
    double &l_double_val = std::any_cast<double &> (l_val);
    double &r_double_val = std::any_cast<double &> (r_val);
    return l_double_val - r_double_val;
  } else {
    long long &l_ll_val = std::any_cast<long long &> (l_val);
    long long &r_ll_val = std::any_cast<long long &> (r_val);
    return l_ll_val - r_ll_val;
  }
}

std::any calculator::visit_mul(mul_node *cur) {
  std::any l_val = cur->lnode->accept(this);
  std::any r_val = cur->rnode->accept(this);
  if (l_val.type() == typeid(double)) {
    double &l_double_val = std::any_cast<double &> (l_val);
    double &r_double_val = std::any_cast<double &> (r_val);
    return l_double_val * r_double_val;
  } else {
    long long &l_ll_val = std::any_cast<long long &> (l_val);
    long long &r_ll_val = std::any_cast<long long &> (r_val);
    return l_ll_val * r_ll_val;
  }
}

std::any calculator::visit_div(div_node *cur) {
  std::any l_val = cur->lnode->accept(this);
  std::any r_val = cur->rnode->accept(this);
  if (l_val.type() == typeid(double)) {
    double &l_double_val = std::any_cast<double &> (l_val);
    double &r_double_val = std::any_cast<double &> (r_val);
    return l_double_val / r_double_val;
  } else {
    long long &l_ll_val = std::any_cast<long long &> (l_val);
    long long &r_ll_val = std::any_cast<long long &> (r_val);
    return l_ll_val / r_ll_val;
  }
}
