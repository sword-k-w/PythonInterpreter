//
// Created by sword on 2024/11/13.
//

#ifndef INT2048_H
#define INT2048_H

#include <iostream>
#include <vector>

class Polynomial;
class int2048 {
private:
  friend class Polynomial;
  constexpr static int kLogBase_ = 3;
  constexpr static int kBase_ = 1e3;
  constexpr static int kDivisionBaseline = 10;
  std::vector<int> num_;
  bool negative_;

public:
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);
  int2048(const Polynomial &, bool);
  ~int2048();

  void read(const std::string &);
  void print() const;
  bool zero() const;

  int2048 &add(const int2048 &);
  friend int2048 add(int2048, const int2048 &);

  int2048 &minus(const int2048 &);
  friend int2048 minus(int2048, const int2048 &);

  int2048 operator + () const;
  int2048 operator - () const;

  int2048 operator ++ ();
  int2048 operator -- ();

  int2048 &operator = (const int2048 &);

  int2048 &operator += (const int2048 &);
  friend int2048 operator + (int2048, const int2048 &);

  int2048 &operator -= (const int2048 &);
  friend int2048 operator - (int2048, const int2048 &);

  int2048 &operator *= (const int2048 &);
  int2048 &operator *= (int);
  friend int2048 operator * (int2048, const int2048 &);

  static int2048 Inverse(int, const int2048 &);
  void MultiplyBasePower(const int &);

  int2048 &operator /= (int2048);
  friend int2048 operator / (int2048, const int2048 &);

  int2048 &operator %= (const int2048 &);
  friend int2048 operator % (int2048, const int2048 &);

  friend std::istream &operator >> (std::istream &, int2048 &);
  friend std::ostream &operator << (std::ostream &, const int2048 &);

  friend bool operator == (const int2048 &, const int2048 &);
  friend bool operator != (const int2048 &, const int2048 &);
  friend bool operator < (const int2048 &, const int2048 &);
  friend bool operator > (const int2048 &, const int2048 &);
  friend bool operator <= (const int2048 &, const int2048 &);
  friend bool operator >= (const int2048 &, const int2048 &);
};

class Complex {
private:
  double x, y;
public:
  Complex(double x = 0, double y = 0) : x(x), y(y) {}
  double real() const;
  Complex &operator += (const Complex &);
  Complex &operator *= (const Complex &);
  Complex &operator /= (const int &);
  friend Complex operator + (const Complex &, const Complex &);
  friend Complex operator - (const Complex &, const Complex &);
  friend Complex operator * (const Complex &, const Complex &);
};

class Polynomial {
private:
  friend class int2048;
  friend class Complex;
  static int power_length;
  static std::vector<int> bit_reversal;
  std::vector<Complex> num_;
public:
  Polynomial();
  Polynomial(const int2048 &);
  ~Polynomial();
  static void Init(int);
  void FFT(int type);
  Polynomial &operator *= (Polynomial &);
};


#endif //INT2048_H
