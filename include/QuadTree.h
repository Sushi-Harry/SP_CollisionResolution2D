#ifndef QUADTREE_H
#define QUADTREE_H

#include "SpatialTypes.h"
#include <vector>

class QuadTree{
public:
    QuadTree(AABB boundary) : _boundary(boundary) {}
    ~QuadTree() { Clear(); }

    void Clear();
    void SubDivide();
    bool Insert(Entity &e);
    void QueryRange(AABB range, std::vector<Entity>& found, int &checksPerformed) const;
private:
    static const int _CAPACITY = 4;
    AABB _boundary;
    std::vector<Entity> _entities;
    bool _subdivided = false;

    // These are the base subdivisions
    QuadTree* _00 = nullptr;
    QuadTree* _10 = nullptr;
    QuadTree* _11 = nullptr;
    QuadTree* _01 = nullptr;
};

// Here's what each subdivision refers to
/*
Look at this diagram as a screen divided into 4 parts. The number in the center denotes the variable name that's used to refer to that specific subdivision
(Why use that name? Cause they're easy to work with since they're kinda llike the coordinates in a cartesian system with a flipped Y axis (Y coord increases as we go downn. (Purely cause I'm familiiar with this already)))
|==================|==================|
|                  |                  |
|        00        |        10        |
|                  |                  |
|==================|==================|
|                  |                  |
|        01        |        11        |
|                  |                  |
|==================|==================|

*/

#endif