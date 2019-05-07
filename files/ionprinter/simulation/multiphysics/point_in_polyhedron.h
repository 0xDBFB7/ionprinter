//Thanks, John!
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>

struct Vector {
  double x, y, z;

  Vector operator-(Vector p) const {
    return Vector{x - p.x, y - p.y, z - p.z};
  }

  Vector cross(Vector p) const {
    return Vector{
      y * p.z - p.y * z,
      z * p.x - p.z * x,
      x * p.y - p.x * y
    };
  }

  double dot(Vector p) const {
    return x * p.x + y * p.y + z * p.z;
  }

  double norm() const {
    return std::sqrt(x*x + y*y + z*z);
  }
};

using Point = Vector;

struct Face {
  std::vector<Point> v;

  Vector normal() const {
    assert(v.size() > 2);
    Vector dir1 = v[1] - v[0];
    Vector dir2 = v[2] - v[0];
    Vector n  = dir1.cross(dir2);
    double d = n.norm();
    return Vector{n.x / d, n.y / d, n.z / d};
  }
};

bool isInConvexPoly(Point const& p, std::vector<Face> const& fs) {
  for (Face const& f : fs) {
    Vector p2f = f.v[0] - p;         // f.v[0] is an arbitrary point on f
    double d = p2f.dot(f.normal());
    d /= p2f.norm();                 // for numeric stability

    constexpr double bound = -1e-15; // use 1e15 to exclude boundaries
    if (d < bound)
      return false;
  }

  return true;
}

int main(int argc, char* argv[]) {
  assert(argc == 3+1);
  char* end;
  Point p;
  p.x = std::strtod(argv[1], &end);
  p.y = std::strtod(argv[2], &end);
  p.z = std::strtod(argv[3], &end);

  std::vector<Face> cube{ // faces with 4 points, last point is ignored
    Face{{Point{0,0,0}, Point{1,0,0}, Point{1,0,1}, Point{0,0,1}}}, // front
    Face{{Point{0,1,0}, Point{0,1,1}, Point{1,1,1}, Point{1,1,0}}}, // back
    Face{{Point{0,0,0}, Point{0,0,1}, Point{0,1,1}, Point{0,1,0}}}, // left
    Face{{Point{1,0,0}, Point{1,1,0}, Point{1,1,1}, Point{1,0,1}}}, // right
    Face{{Point{0,0,1}, Point{1,0,1}, Point{1,1,1}, Point{0,1,1}}}, // top
    Face{{Point{0,0,0}, Point{0,1,0}, Point{1,1,0}, Point{1,0,0}}}, // bottom
  };

  std::cout << (isInConvexPoly(p, cube) ? "inside" : "outside") << std::endl;

  return 0;
}
