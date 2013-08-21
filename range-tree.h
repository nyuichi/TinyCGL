// -*- mode: c++ -*-

#ifndef RANGE_TREE_H
#define RANGE_TREE_H

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "point.h"

template<unsigned int D>
struct node {
public:

  static node<D>* build(const std::vector<point>& points) {
    std::vector<point> ps(points);

    std::sort(ps.begin(), ps.end(), node<D>::comp_node);

    return node<D>::build_node(ps);
  }

  void query(point min, point max, std::vector<point>& out) {
    double rmin = get<D>(min);
    double rmax = get<D>(max);
    if (rmin <= min_ && max_ <= rmax) {
      assoc_->query(min, max, out);
    } else if (rmin <= max_ && min_ <= rmax) {
      left_->query(min, max, out);
      right_->query(min, max, out);
    }
  }

  ~node() {
    if (left_) delete left_;
    if (right_) delete right_;
    if (assoc_) delete assoc_;
  }

private:

  node(double min, double max, node<D>* left, node<D>* right, node<D-1>* assoc)
    : min_(min)
    , max_(max)
    , left_(left)
    , right_(right)
    , assoc_(assoc)
  {
  }

  static node<D>* build_node(const std::vector<point>& points) {
    int size = points.size();

    double min = get<D>(points[0]);
    double max = get<D>(points[size-1]);
    node<D>* left = NULL;
    node<D>* right = NULL;
    node<D-1>* assoc = node<D-1>::build(points);

    if (size > 1) {
      std::vector<point>::const_iterator
	beg = points.begin(),
	end = points.end(),
	mid = beg + size/2;

      left = node<D>::build_node(std::vector<point>(beg, mid));
      right = node<D>::build_node(std::vector<point>(mid, end));
    }

    return new node<D>(min, max, left, right, assoc);
  }

  static bool comp_node(const point& a, const point& b) {
    return get<D>(a) < get<D>(b);
  }

  double min_;
  double max_;

  node<D>* left_;
  node<D>* right_;

  node<D-1>* assoc_;

};

template<>
struct node<1> {
public:

  static node<1>* build(const std::vector<point>& points) {
    std::vector<point> ps(points);

    std::sort(ps.begin(), ps.end(), node<1>::comp_node);

    return new node<1>(ps);
  }

  void query(point min, point max, std::vector<point>& out) {
    int lower, upper;

    int size = points_.size();

    // lower bound
    int low = 0;
    int high = size - 1;
    while (low < high) {
      int mid = (low + high) / 2;
      if (get<1>(points_[mid]) < get<1>(min)) {
	low = mid + 1;
      } else {
	high = mid;
      }
    }
    if (get<1>(points_[low]) < get<1>(min)) {
      lower = size;
    } else {
      lower = low;
    }

    // upper bound
    low = 0;
    high = size - 1;
    while (low < high) {
      int mid = (low + high + 1) / 2;
      if (get<1>(max) < get<1>(points_[mid])) {
	high = mid - 1;
      } else {
	low = mid;
      }
    }
    if (get<1>(max) < get<1>(points_[low])) {
      upper = -1;
    } else {
      upper = low;
    }

    // report results
    for (int i = lower; i <= upper; ++i) {
      out.push_back(points_[i]);
    }
  }

private:

  node(const std::vector<point>& points)
    : points_(points)
  {
  }

  static bool comp_node(const point& a, const point& b) {
    return get<1>(a) < get<1>(b);
  }

  std::vector<point> points_;

};

class range_tree {
public:

  explicit range_tree(const std::vector<point>& points)
    : tree_(node<3>::build(points))
  {
  }

  ~range_tree() {
    delete tree_;
  }

  void query(point min, point max)
  {
    result_.clear();
    tree_->query(min, max, result_);
  }

  const std::vector<point>& result() const {
    return result_;
  }

private:

  node<3>* tree_;

  std::vector<point> result_;

};

#endif
