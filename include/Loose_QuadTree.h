#ifndef LOOSE_QUAD_TREE_H
#define LOOSE_QUAD_TREE_H

#include "SpatialTypes.h"
#include <vector>

struct LooseQuadTreeNode{
    AABB _boundary;
    std::vector<Entity*> _entities;
    /*
        Dear C++,
            I have etched you upon my heart, beyond just my vision; you have become a reason for my existence.
        Yours Truly,
        Harry Chauhan
    */

    bool _subdivided = false;
    int _firstChild = -1;
};

class LooseQuadTree{
public:
    LooseQuadTree(AABB boundary, int maxNodes = 10000);
    ~LooseQuadTree() = default;

    void Clear();
    bool Insert(Entity *e);
    void QueryRange(AABB range, std::vector<Entity*>& found, int& checksDone) const;

    void GetActiveBounds(std::vector<AABB>& bounds) const;
    
private:
    static const int _CAPACITY = 4;
    // This is the looseness factor
    // 1.0 looseness factor means a regular QuadTree
    // 2.0 means children can overlap and share space
    const double _k = 2.0;

    AABB _rootBoundary;
    std::vector<LooseQuadTreeNode> _nodes;
    int _nextNodeIdx = 0;

    void SubDivide(int nodeIdx);
    bool InsertAtNode(int nodeIdx, Entity* e);
    void QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksPerformed) const;
    void GetActiveBoundsAtNode(int nodeIdx, std::vector<AABB>& bounds) const;
};

#endif