// -*- mode: c++ -*-

#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <limits>

struct point {
public:

  point(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z)
  {
  }

  double x() const {
    return x_;
  }
  double y() const {
    return y_;
  }
  double z() const {
    return z_;
  }

  bool operator==(const point& rhs) const {
    if (fabs(x_ - rhs.x_) >= std::numeric_limits<double>::min())
      return false;
    if (fabs(y_ - rhs.y_) >= std::numeric_limits<double>::min())
      return false;
    return fabs(z_ - rhs.z_) < std::numeric_limits<double>::min();
  }

  point operator-() const {
    return point(-x_, -y_, -z_);
  }

  point operator+(const point& rhs) const {
    return point(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
  }

  point operator-(const point& rhs) const {
    return point(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
  }

  point operator*(double mag) const {
    return point(x_ * mag, y_ * mag, z_ * mag);
  }

  point operator/(double mag) const {
    return point(x_ / mag, y_ / mag, z_ / mag);
  }

  double dot(const point& rhs) const {
    return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
  }

  point cross(const point& rhs) const {
    return point(y_ * rhs.z_ - z_ * rhs.y_,
		 z_ * rhs.x_ - x_ * rhs.z_,
		 x_ * rhs.y_ - y_ * rhs.x_);
  }

  double norm() const {
    return sqrt(norm2());
  }

  double norm2() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  double dist(const point& rhs) const {
    return sqrt(dist2(rhs));
  }

  double dist2(const point& rhs) const {
    return (*this - rhs).norm2();
  }

  point normalize() const {
    return *this / norm();
  }

private:

  double x_;
  double y_;
  double z_;

};

template<int T>
double get (point p);

template<> double get<1> (point p) { return p.x(); }
template<> double get<2> (point p) { return p.y(); }
template<> double get<3> (point p) { return p.z(); }


#endif
