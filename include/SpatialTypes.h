#ifndef SPATIALTYPES_H
#define SPATIALTYPES_H

#include <cmath>
#include <cstdint>

// Basic Vector2 Class
struct Vec2{
    double x;
    double y;
    Vec2() = default;
    Vec2(double x, double y) : x(x), y(y) {}
    double Length() const { return  sqrt(pow(x, 2.0) + pow(y, 2.0)); }
};

// Axis Aligned Bounding Box class
struct AABB {
    Vec2 _center;
    Vec2 _halfDimension;

    AABB() = default;

    bool containsPoint(const Vec2& P) const {
        return (P.x >= _center.x - _halfDimension.x && P.x <= _center.x + _halfDimension.x &&
                P.y >= _center.y - _halfDimension.y && P.y <= _center.y + _halfDimension.y);
    }

    bool intersects(const AABB& other) const {
        if(abs_operator(_center.x - other._center.x) > (_halfDimension.x + other._halfDimension.x)) return false;
        if(abs_operator(_center.y - other._center.y) > (_halfDimension.y + other._halfDimension.y)) return false;
        return true;
    }
private:
    static double abs_operator(double v) { return v < 0 ? -v : v; }
};

// This is the entity class
struct Entity{
    uint32_t _id;
    Vec2 _position;
    Vec2 _velocity;
    double _radius;

    bool _colliding = false;
};
#endif