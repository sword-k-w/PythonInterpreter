#include "int2048.h"
#include <cmath>
#include <cassert>
#include <Evalvisitor.h>

int Polynomial::power_length;
std::vector<int> Polynomial::bit_reversal;

constexpr double kPi = std::acos(-1.0);

int2048::int2048() { // default 0
  num_ = {0};
  negative_ = false;
}

int2048::int2048(long long x) {
  num_.clear();
  negative_ = false;
  if (x == 0) {
    num_ = {0};
    return;
  } else if (x < 0) {
    negative_ = true;
    x = -x;
  }
  while (x) {
    num_.emplace_back(x % kBase_);
    x /= kBase_;
  }
}

int2048::int2048(double x) {
  num_.clear();
  negative_ = false;
  if (fabs(x) < 1e-6) {
    num_ = {0};
  } else {
    if (x < 0) {
      negative_ = true;
      x = -x;
    }
    while (x >= 1) {
      double y = floor(x / kBase_);
      int val = x - y * kBase_;
      MyAssert(val >= 0 && val < kBase_);
      num_.emplace_back(val);
      x = y;
    }
  }
}

int2048::int2048(const std::string &string) {
  read(string);
}

int2048::int2048(const int2048 &x) {
  num_ = x.num_;
  negative_ = x.negative_;
}

int2048::int2048(const Polynomial &x, bool negative) {
  num_.clear();
  long long nxt = 0, cur;
  for (int i = 0; i < Polynomial::power_length; ++i) {
    cur = nxt + x.num_[i].real() + 0.5;
    nxt = cur / kBase_;
    cur -= nxt * kBase_;
    num_.emplace_back(cur);
  }
  negative_ = negative;
  while (!num_.back()) {
    num_.pop_back();
  }
}

int2048::~int2048() {
  num_.clear();
  num_.shrink_to_fit();
}

int2048::operator double() const {
  double res = 0;
  for (auto &x : num_) {
    res *= kBase_;
    res += x;
  }
  if (negative_) {
    res = -res;
  }
  return res;
}

int2048::operator std::string() const {
  if (zero()) {
    return "0";
  }
  std::string res;
  if (negative_) {
    res = "-";
  }
  res += std::to_string(*num_.rbegin());
  int size = static_cast<int>(num_.size());
  for (int i = size - 2; i >= 0; --i) {
    std::string tmp = std::to_string(num_[i]);
    while (tmp.size() < kLogBase_) {
      tmp = '0' + tmp;
    }
    res += tmp;
  }
  return res;
}

void int2048::read(const std::string &string) {
  num_.clear();
  int string_length = static_cast<int>(string.size());
  int first = 0;
  if (string[0] == '-') {
    negative_ = true;
    ++first;
  } else if (string[0] == '+') {
    negative_ = false;
    ++first;
  } else {
    negative_ = false;
  }
  int cur = string_length - 1;
  while (cur - kLogBase_ >= first) {
    int tmp = 0;
    for (int i = kLogBase_ - 1; i >= 0; --i) {
      tmp = tmp * 10 + (string[cur - i] - '0');
    }
    cur -= kLogBase_;
    num_.emplace_back(tmp);
  }
  int tmp = 0;
  while (cur >= first) {
    tmp = tmp * 10 + (string[first] - '0');
    ++first;
  }
  num_.emplace_back(tmp);
  while (!num_.empty() && !num_.back()) {
    num_.pop_back();
  }
  if (num_.empty()) {
    num_ = {0};
    negative_ = false;
  }
}

void int2048::print() const {
  std::cout << *this;
}

bool int2048::zero() const {
  return num_.size() == 1 && num_[0] == 0;
}

int2048 &int2048::add(const int2048 &x) {
  int2048 y = x;
  if (negative_ ^ x.negative_) { // subtraction
    y.negative_ ^= 1;
    return minus(y);
  } else { // addition
    size_t size = std::max(num_.size(), y.num_.size());
    if (num_.size() < size) {
      num_.resize(size);
    } else if (y.num_.size() < size) {
      y.num_.resize(size);
    }
    int carry = 0;
    for (int i = 0; i < size; ++i) {
      num_[i] += y.num_[i] + carry;
      if (num_[i] >= kBase_) {
        num_[i] -= kBase_;
        carry = 1;
      } else {
        carry = 0;
      }
    }
    if (carry) {
      num_.emplace_back(1);
    }
  }
  return *this;
}

int2048 add(int2048 x, const int2048 &y) {
  return x.add(y);
}

int2048 &int2048::minus(const int2048 &x) {
  int2048 y = x;
  if (negative_ ^ x.negative_) { // addition
    y.negative_ ^= 1;
    return add(y);
  } else { // subtraction
    bool negative = negative_;
    negative_ = y.negative_ = false;
    size_t size = num_.size(), y_size = y.num_.size();
    int flag = 0;
    if (size ^ y_size) {
      if (size > y_size) {
        flag = 1;
      } else {
        flag = -1;
      }
    } else {
      for (int i = size - 1; i >= 0; --i) {
        if (num_[i] ^ y.num_[i]) {
          if (num_[i] > y.num_[i]) {
            flag = 1;
          } else {
            flag = -1;
          }
          break;
        }
      }
    }
    if (!flag) {
      num_ = {0};
      negative_ = false;
    } else {
      if (flag == 1) {
        y.num_.resize(size);
        for (int i = 0; i < size; ++i) {
          num_[i] -= y.num_[i];
          if (num_[i] < 0) {
            num_[i] += kBase_;
            --num_[i + 1];
          }
        }
      } else {
        num_.resize(y_size);
        for (int i = 0; i < y_size; ++i) {
          num_[i] = y.num_[i] - num_[i];
          if (num_[i] < 0) {
            num_[i] += kBase_;
            --y.num_[i + 1];
          }
        }
        negative ^= 1;
      }
      while (!num_.back()) {
        num_.pop_back();
      }
      negative_ = negative;
    }
  }
  return *this;
}

int2048 minus(int2048 x, const int2048 &y) {
  return x.minus(y);
}

int2048 int2048::operator + () const {
  return *this;
}

int2048 int2048::operator - () const {
  if (zero()) {
    return *this;
  }
  int2048 x = *this;
  x.negative_ ^= 1;
  return x;
}

int2048 int2048::operator ++ () { // ++|value|
  for (auto &val : num_) {
    ++val;
    if (val == kBase_) {
      val = 0;
    } else {
      return *this;
    }
  }
  num_.emplace_back(1);
  return *this;
}

int2048 int2048::operator -- () { // --|value|
  if (zero()) {
    exit(1);
  }
  if (num_.size() == 1 && num_[0] == 1) {
    num_ = {0};
    return *this;
  }
  for (auto &val : num_) {
    --val;
    if (val < 0) {
      val = kBase_ - 1;
    } else {
      return *this;
    }
  }
  num_.pop_back();
  return *this;
}

int2048 &int2048::operator = (const int2048 &x) {
  num_ = x.num_;
  negative_ = x.negative_;
  return *this;
}

int2048 &int2048::operator += (const int2048 &x) {
  return this->add(x);
}

int2048 operator + (int2048 x, const int2048 &y) {
  return x.add(y);
}

int2048 &int2048::operator -= (const int2048 &x) {
  return this->minus(x);
}

int2048 operator - (int2048 x, const int2048 &y) {
  return x.minus(y);
}

int2048 &int2048::operator *= (const int2048 &x) {
  if (zero()) {
    return *this;
  }
  if (x.zero()) {
    num_ = {0};
    negative_ = false;
    return *this;
  }
  negative_ ^= x.negative_;
  int length_c = num_.size() + x.num_.size(); // c = a * b;
  Polynomial::Init(length_c);
  Polynomial a(*this), b(x);
  a *= b;
  return *this = int2048(a, negative_);
}

int2048 operator * (int2048 x, const int2048 &y) {
  return x *= y;
}

int2048 &int2048::operator *= (int x) { // ensure x is small
  if (x == 0) {
    num_ = {0};
    negative_ = false;
    return *this;
  }
  if (x < 0) {
    negative_ ^= 1;
    x = -x;
  }
  if (x >= kBase_) {
    return *this *= int2048(0ll + x);
  }
  int carry = 0;
  for (auto &val : num_) {
    val = val * x + carry;
    carry = val / kBase_;
    val -= carry * kBase_;
  }
  if (carry) {
    num_.emplace_back(carry);
  }
  return *this;
}

/** x <- x * base ^ k
 *  k can be negative
 */
void int2048::MultiplyBasePower(const int &k) {
  if (!k || zero()) {
    return;
  }
  if (k > 0) {
    num_.resize(num_.size() + k);
    for (int i = static_cast<int>(num_.size()) - 1; i >= k; --i) {
      num_[i] = num_[i - k];
    }
    for (int i = 0; i < k; ++i) {
      num_[i] = 0;
    }
  } else {
    if (num_.size() <= -k) {
      num_ = {0};
      negative_ = false;
      return;
    }
    int size = static_cast<int>(num_.size()) + k;
    for (int i = 0; i < size; ++i) {
      num_[i] = num_[i - k];
    }
    num_.resize(size);
  }
}

int2048 int2048::Inverse(int m, const int2048 &y) {
  if (m <= kDivisionBaseline) {
    int2048 x(1ll);
    x.MultiplyBasePower(2 * m);
    return x /= y;
  }
  int k = (m + 5) / 2;
  int2048 nxt = y;
  nxt.MultiplyBasePower(k - m);
  int2048 tmp = y, res = Inverse(k, nxt);
  tmp *= res, tmp *= res;
  tmp.MultiplyBasePower(-2 * k);
  res *= 2;
  res.MultiplyBasePower(m - k);
  res -= tmp;

  // adjust
  // if ((res * nxt).num_.size() <= 2 * k) {
  //   return res;
  // } else {
    return ++res;
  // }
}

/** @sword
 *  ans1 = x / y (x < 10 ^ n, y < 10 ^ m)
 *  Step1 : ans2 = x * (t / y) / t (t = base ^ {n} -> base ^ {2m}), ans1 - 1 <= ans2 <= ans1   (I don't understand why my program reveals that ans2 - 2 <= ans1 <= ans2)
 *  Step2 : base ^ {2m} / y
 *          if (n > 2m) x <- x * (base ^ {n - 2m}), y <- y * (base ^ {n - 2m})
 *          then n' <= 2m'
 *  Now calculate base ^ {2m} / y and operate above steps in inverted order
 *
 *  calculate : base ^ {2m} / y
 *  let f(k) be base ^ {2k} / (y / base ^ {m - k}) (goal : f(m))
 *
 *  let k = (m + 5) / 2 (if m is small enough, calculate f(k) with brute force)
 *  calculate f(k)
 *
 *  f*(m) = 2f(k)base ^ {m - k} - y * [f(k)] ^ 2 / base ^ {2k}
 *  f*(m) <= f(m) <= f*(m) + 1 (I don't understand why my program reveals that f(m) = f*(m) + 1, either)
 *
 */
int2048 &int2048::operator /= (int2048 y) {
  if (zero()) {
    return *this;
  }
  bool negative = negative_ ^ y.negative_;
  negative_ = false, y.negative_ = false;
  if (negative) {
    *this += y;
    --(*this);
  }
  if (*this < y) {
    num_ = {0};
    negative_ = false;
    return *this;
  }
  int size = num_.size(), y_size = y.num_.size();
  if (size <= 2 * kDivisionBaseline) { // Brute force here
    const int x_size = size - y_size + 1;
    int2048 x, cur = *this;
    cur.MultiplyBasePower(1 - x_size);
    x.num_.resize(x_size);
    x.negative_ = negative;

    static std::vector<int2048> tmp;
    tmp.emplace_back(y);
    for (int i = 2; i < kBase_; i <<= 1) {
      y *= 2;
      tmp.emplace_back(y);
    }

    for (int i = x_size - 1; i >= 0; --i) {
      for (int j = 9; j >= 0; --j) { // @warning Do not modify kBase_ !!!
        if (tmp[j] <= cur) {
          cur -= tmp[j];
          x.num_[i] |= 1 << j;
        }
      }
      if (i) {
        cur.MultiplyBasePower(1);
        cur.num_[0] = num_[i - 1];
      }
    }
    tmp.clear();
    while (!x.num_.back()) {
      x.num_.pop_back();
    }
    return *this = x;
  }
  if (size > 2 * y_size) {
    const int k = size - 2 * y_size;
    MultiplyBasePower(k);
    y.MultiplyBasePower(k);
    size += k, y_size += k;
  }
  int2048 tmp = Inverse(y_size, y);
  int2048 x = *this;
  *this *= tmp;
  this->MultiplyBasePower(-2 * y_size);
  x -= y * (*this);
  if (x.negative_) {
    --(*this);
    x += y;
    if (x.negative_) {
      --(*this);
      x += y;
    }
  }
  negative_ = negative;
  return *this;
}

int2048 operator / (int2048 x, const int2048 &y) {
  return x /= y;
}

int2048 &int2048::operator %= (const int2048 &y) {
  return *this -= (*this / y) * y;
}

int2048 operator % (int2048 x, const int2048 &y) {
  return x %= y;
}

std::istream &operator >> (std::istream &is, int2048 &x) {
  std::string string;
  is >> string;
  x = int2048(string);
  return is;
}

std::ostream &operator << (std::ostream &os, const int2048 &x) {
  int size = static_cast<int>(x.num_.size());
  if (x.zero()) {
    os << '0';
    return os;
  }
  if (x.negative_) {
    os << '-';
  }
  os << x.num_[size - 1];
  for (int i = size - 2; i >= 0; --i) {
    if (x.num_[i] < 10) { // @warning Do not modify kBase_ !!!
      os << "00" << x.num_[i];
    } else if (x.num_[i] < 100) {
      os << '0' << x.num_[i];
    } else if (x.num_[i] < 1000) {
      os << x.num_[i];
    }
  }
  return os;
}

bool operator == (const int2048 &x, const int2048 &y) {
  if (x.negative_ != y.negative_ || x.num_.size() != y.num_.size()) {
    return false;
  }
  const int size = static_cast<int>(x.num_.size());
  for (int i = 0; i < size; ++i) {
    if (x.num_[i] != y.num_[i]) {
      return false;
    }
  }
  return true;
}

bool operator != (const int2048 &x, const int2048 &y) {
  return !(x == y);
}

bool operator < (const int2048 &x, const int2048 &y) {
  if (x.negative_ && !y.negative_) {
    return true;
  }
  if (!x.negative_ && y.negative_) {
    return false;
  }
  const int x_size = static_cast<int>(x.num_.size());
  const int y_size = static_cast<int>(y.num_.size());
  if (x.negative_) {
    if (x_size != y_size) {
      return x_size > y_size;
    }
    for (int i = x_size - 1; i >= 0; --i) {
      if (x.num_[i] != y.num_[i]) {
        return x.num_[i] > y.num_[i];
      }
    }
    return false;
  } else {
    if (x_size != y_size) {
      return x_size < y_size;
    }
    for (int i = x_size - 1; i >= 0; --i) {
      if (x.num_[i] != y.num_[i]) {
        return x.num_[i] < y.num_[i];
      }
    }
    return false;
  }
}

bool operator > (const int2048 &x, const int2048 &y) {
  if (x.negative_ && !y.negative_) {
    return false;
  }
  if (!x.negative_ && y.negative_) {
    return true;
  }
  const int x_size = static_cast<int>(x.num_.size());
  const int y_size = static_cast<int>(y.num_.size());
  if (x.negative_) {
    if (x_size != y_size) {
      return x_size < y_size;
    }
    for (int i = x_size - 1; i >= 0; --i) {
      if (x.num_[i] != y.num_[i]) {
        return x.num_[i] < y.num_[i];
      }
    }
    return false;
  } else {
    if (x_size != y_size) {
      return x_size > y_size;
    }
    for (int i = x_size - 1; i >= 0; --i) {
      if (x.num_[i] != y.num_[i]) {
        return x.num_[i] > y.num_[i];
      }
    }
    return false;
  }
}

bool operator <= (const int2048 &x, const int2048 &y) {
  return !(x > y);
}

bool operator >= (const int2048 &x, const int2048 &y) {
  return !(x < y);
}

double Complex::real() const {
  return x;
}

Complex &Complex::operator += (const Complex &b) {
  x += b.x, y += b.y;
  return *this;
}

Complex &Complex::operator *= (const Complex &b) {
  return *this = Complex(x * b.x - y * b.y, x * b.y + y * b.x);
}

Complex &Complex::operator /= (const int &b) {
  return *this = Complex(x / b, y / b);
}

Complex operator + (const Complex &a, const Complex &b) {
  return Complex(a.x + b.x, a.y + b.y);
}

Complex operator - (const Complex &a, const Complex &b) {
  return Complex(a.x - b.x, a.y - b.y);
}

Complex operator * (const Complex &a, const Complex &b) {
  return Complex(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

Polynomial::Polynomial() {
  num_.clear();
  num_.resize(power_length);
}

Polynomial::~Polynomial() {
  num_.clear();
  num_.shrink_to_fit();
}

Polynomial::Polynomial(const int2048 &x) {
  num_.clear();
  num_.resize(power_length);
  int size = static_cast<int> (x.num_.size());
  for (int i = 0; i < size; ++i) {
    num_[i] = x.num_[i];
  }
}

void Polynomial::Init(int length) {
  power_length = 1;
  while (power_length < length) {
    power_length <<= 1;
  }
  // if (power_length > 1e4) { // @sword magic
  //   power_length <<= 1;
  // }
  bit_reversal.resize(power_length);
  for (int i = 1; i < power_length; ++i) {
    bit_reversal[i] = bit_reversal[i >> 1] >> 1;
    if (i & 1) {
      bit_reversal[i] |= power_length >> 1;
    }
  }
}

void Polynomial::FFT(int type) {
  for (int i = 0; i < power_length; ++i) {
    if (i < bit_reversal[i]) {
      std::swap(num_[i], num_[bit_reversal[i]]);
    }
  }
  for (int length = 1; length < power_length; length <<= 1) {
    Complex w_i(std::cos(kPi / length), type * std::sin(kPi / length));
    for (int i = 0; i < power_length; i += (length << 1)) {
      Complex w(1, 0);
      for (int j = i; j < i + length; ++j, w *= w_i) {
        auto x = num_[j], y = w * num_[j + length];
        num_[j] += y;
        num_[j + length] = x - y;
      }
    }
  }
  if (type < 0) {
    for (int i = 0; i < power_length; ++i) {
      num_[i] /= power_length;
    }
  }
}

Polynomial &Polynomial::operator *= (Polynomial &b) {
  FFT(1), b.FFT(1);
  for (int i = 0; i < power_length; ++i) {
    num_[i] *= b.num_[i];
  }
  FFT(-1);
  return *this;
}