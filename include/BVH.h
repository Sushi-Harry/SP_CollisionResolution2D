#ifndef BVH_H
#define BVH_H

#include "SpatialTypes.h"
#include <vector>

struct BVHNode{
    AABB _boundary;

    // So here's the deal
    // a BVH is basically a binary tree where every internal node is a bounding box (AABB in this case)
    // Only the leaf nodes are the actual objects that need collision checking
    // And also, it is A LOT FASTER to just scrap a BVH Tree instead of rearranging the nodes in a single tree
    // So for every simulation tick we're gonna be scrapping the old tree and rebuilding a new tree
    Entity* _entity = nullptr;

    int _lChildIdx = -1;
    int _rChildIdx = -1;

    bool isLeaf() const{
        return _lChildIdx == -1 && _rChildIdx == -1;
    }
};

// Unlike a normal binary tree I don't need to store the pointers to the children in every node.
// I can make it work with just the index since I'll be using a flat array for it and just shuffling elements around while building the tree

class BoundingVolumeHierarchy{
public:
    BoundingVolumeHierarchy(int maxNodes = 10000);
    ~BoundingVolumeHierarchy() = default;

    void Build(std::vector<Entity*>& entities);
    void QueryRange(AABB range, std::vector<Entity*>& found, int& checksPerformed) const;
    void GetActiveBounds(std::vector<AABB>& bounds) const;
private:
    std::vector<BVHNode> _nodes;
    int _maxNodes;
    int _nextNodeIdx = 0;

    int RecursiveBuild(std::vector<Entity*>& entities, int start, int end);
    AABB CalculateBounds(const std::vector<Entity*>& entities, int start, int end) const;
    void QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksPerformed) const;
    void GetActiveBoundsAtNode(int nodeIdx, std::vector<AABB>& bounds) const;
};

#endif