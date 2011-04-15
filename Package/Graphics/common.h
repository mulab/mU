#pragma once

#include <vector>
#include <cmath>
#include <iostream>

#include <mU/Kernel.h>
#include <mU/Number.h>
//#include <mU/Function.h>

#define HUE_HUEBASIC 67
#define HUE_SBASIC 0.6
#define HUE_BBASIC 0.7
#define HUE_SLIGHT 0.2
#define HUE_BLIGHT 1.0
#define HUE_HUEADD 23

using namespace std;

struct Color {
    int r, g, b;
    Color(int red, int green, int blue)
            : r(red), g(green), b(blue) {}
};

struct Hue {
    double h, s, b;
    Hue(double hue, double saturation, double brightness)
            : h(hue), s(saturation), b(brightness) {}
};

struct Point2d {
    double x, y;
    Point2d() : x(0), y(0) {}
    Point2d(double _x, double _y)
            : x(_x), y(_y) {}
    Point2d(Point2d& p)
            : x(p.x), y(p.y) {}
};

struct Point3d {
    double x, y, z;
    Point3d() : x(0), y(0), z(0) {}
    Point3d(double _x, double _y, double _z)
            : x(_x), y(_y), z(_z) {}
    Point3d(Point3d& p)
            : x(p.x), y(p.y), z(p.z) {}
};

struct Line {
    double x1, y1, x2, y2;
    Line() : x1(0), y1(0), x2(0), y2(0) {}
    Line(double _x1, double _y1, double _x2, double _y2)
            : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

template <class T>
class ArrayList : public std::vector<T> {
public:
    void add(const T &x) {
        std::vector<T>::push_back(x);
    }
    void add(unsigned int x, const T &y) {
        std::vector<T>::insert(std::vector<T>::begin() + x, y);
    }
    void remove(unsigned int x) {
        std::vector<T>::erase(std::vector<T>::begin() + x);
    }
    void clear() {
        this->resize(0);
    }
};


class Vector3D {
public:
    double x, y, z;

public:
    Vector3D(double xx = 0, double yy = 0, double zz = 0) {
        x = xx;
        y = yy;
        z = zz;
    }

    Vector3D(const Vector3D& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3D operator + (const Vector3D& v) {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    Vector3D operator - (const Vector3D& v) {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    double operator * (const Vector3D& v) {
        return (x*v.x + y*v.y + z*v.z);
    }
};
