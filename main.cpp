#include "range-tree.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

double rand_real()
{
  return (double)rand()/(double)RAND_MAX;
}

bool in_range(point p, point min, point max)
{
  return (min.x() <= p.x() && p.x() <= max.x() &&
	  min.y() <= p.y() && p.y() <= max.y() &&
	  min.z() <= p.z() && p.z() <= max.z());
}

std::string stringify(point p)
{
  std::stringstream ss;
  ss << "#<point:";
  ss << " x=";
  ss << p.x();
  ss << " y=";
  ss << p.y();
  ss << " z=";
  ss << p.z();
  ss << ">";
  return ss.str();
}

bool test(int N)
{
  // create random range
  double f[6];
  for (int i = 0; i < 6; ++i) {
    f[i] = rand_real();
  }
  for (int i = 0; i < 3; ++i) {
    if (f[i] > f[i+3]) {
      double tmp = f[i];
      f[i] = f[i+3];
      f[i+3] = tmp;
    }
  }
  point min(f[0], f[1], f[2]);
  point max(f[3], f[4], f[5]);

  std::vector<point> points;
  int count = 0;
  for (int i = 0; i < N; ++i) {
    double x = rand_real(), y = rand_real(), z = rand_real();
    point p(x, y, z);
    if (in_range(p, min, max)) {
      ++count;
    }
    points.push_back(p);
  }

  clock_t t;

  printf("| tree construction started... ");
  fflush(stdout);
  t = clock();
  range_tree rt(points);
  t = clock() - t;
  printf("done (took %f sec)\n", (double)t/CLOCKS_PER_SEC);

  printf("| *info* count = %d\n", count);

  printf("| quering stared... ");
  fflush(stdout);
  t = clock();
  rt.query(min, max);		// query!
  t = clock() - t;
  printf("done (took %f sec)\n", (double)t/CLOCKS_PER_SEC);

  if (rt.result().size() != count) {
    std::cout << "expected\tmin : ";
    std::cout << stringify(min);
    std::cout << "\n\t\tmax : ";
    std::cout << stringify(max);
    std::cout << "\n\t\tcount : ";
    std::cout << count;
    std::cout << "\n\n";
    std::cout << "but got\tcount : ";
    std::cout << rt.result().size();
    std::cout << "\n\n";

    for (std::vector<point>::const_iterator it = rt.result().begin();
	 it != rt.result().end();
	 ++it) {
      if (! in_range(*it, min, max)) {
	std::cout << "out of range: " << stringify(*it) << std::endl;
      }
    }
    return false;
  }
  return true;
}

int main()
{
  clock_t t;

  srand(100);

  for (int i = 0; i < 6; ++i) {
    int N = pow(10, i);

    printf("* test %d stated... (N=%d)\n", i, N);
    fflush(stdout);
    t = clock();
    bool success = test(N);
    t = clock() - t;

    if (! success) {
      printf("***test failure in N=%d\n", N);
      std::abort();
    }

    printf("* test %d successfull (took %f sec)\n\n", i, (double)t/CLOCKS_PER_SEC);
  }

  puts("All tests successfull!");
}
