#include "BVH.h"
#include <algorithm>

BoundingVolumeHierarchy::BoundingVolumeHierarchy(int maxNodes) : _maxNodes(maxNodes){
    _nodes.resize(maxNodes);
}

void BoundingVolumeHierarchy::Build(std::vector<Entity*>& entities){
    _nextNodeIdx = 0;
    if(entities.empty()) return;

    RecursiveBuild(entities, 0, entities.size());
}

AABB BoundingVolumeHierarchy::CalculateBounds(const std::vector<Entity*>& entities, int start, int end) const{
    float minX = 999999.0F, minY = 999999.0F;
    float maxX = -999999.0F, maxY = -999999.0F;

    for(int i = start; i < end; i++){
        Entity* e = entities[i];
        if(e->_position.x - e->_radius < minX) minX = e->_position.x - e->_radius;
        if(e->_position.x + e->_radius > maxX) maxX = e->_position.x + e->_radius;
        if(e->_position.y - e->_radius < minY) minY = e->_position.y - e->_radius;
        if(e->_position.y + e->_radius > maxY) maxY = e->_position.y + e->_radius;
    }

    Vec2 center = {
        (minX + maxX) / 2.0,
        (minY + maxY) / 2.0
    };
    Vec2 halfDim = {
        (maxX - minX) / 2.0,
        (maxY - minY) / 2.0
    };

    return { center, halfDim};
}

int BoundingVolumeHierarchy::RecursiveBuild(std::vector<Entity*>& entities, int start, int end){
    int nodeIdx = _nextNodeIdx++;
    if(_nextNodeIdx > _nodes.size()){
        _nodes.resize(_nodes.size() * 2);
    }

    BVHNode& node = _nodes[nodeIdx];
    node._boundary = CalculateBounds(entities, start, end);

    int count = end - start;

    // WHen there's just 1 node we'll just make it a leaf node
    if(count == 1){
        node._entity = entities[start];
        node._lChildIdx = -1;
        node._rChildIdx = -1;
        return nodeIdx;
    }
    // the longest axis to split along
    float sizeX = node._boundary._halfDimension.x * 2.0F;
    float sizeY = node._boundary._halfDimension.y * 2.0F;
    int axis = (sizeX > sizeY) ? 0 : 1;
    // axis = 0 for X axis and 1 for Y axis

    int mid = start + count / 2;
    std::nth_element(entities.begin() + start, entities.begin() + mid, entities.begin() + end, [axis](Entity* a, Entity* b){
        if(axis == 0) return a->_position.x < b->_position.x;
        else return a->_position.y < b->_position.y;
    });

    int left = RecursiveBuild(entities, start, mid);
    int right = RecursiveBuild(entities, mid, end);

    _nodes[nodeIdx]._lChildIdx = left;
    _nodes[nodeIdx]._rChildIdx = right;
    _nodes[nodeIdx]._entity = nullptr;

    return nodeIdx;
}

void BoundingVolumeHierarchy::QueryRange(AABB range, std::vector<Entity*>& found, int& checksPerformed) const{
    if(_nextNodeIdx > 0){
        QueryAtNode(0, range, found, checksPerformed);
    }
}

void BoundingVolumeHierarchy::QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksPerformed) const{
    const BVHNode& node = _nodes[nodeIdx];

    checksPerformed++;
    if(!node._boundary.intersects(range)) return;

    if(node.isLeaf()){
        checksPerformed++;
        if(range.containsPoint(node._entity->_position)){
            found.push_back(node._entity);
        }
    }else{
        QueryAtNode(node._lChildIdx, range, found, checksPerformed);
        QueryAtNode(node._rChildIdx, range, found, checksPerformed);
    }
}

void BoundingVolumeHierarchy::GetActiveBounds(std::vector<AABB>& bounds) const {
    if(_nextNodeIdx > 0){
        GetActiveBoundsAtNode(0, bounds);
    }
}

void BoundingVolumeHierarchy::GetActiveBoundsAtNode(int nodeIdx, std::vector<AABB>& bounds) const {
    const BVHNode& node = _nodes[nodeIdx];

    bounds.push_back(node._boundary);
    if(!node.isLeaf()){
        GetActiveBoundsAtNode(node._lChildIdx, bounds);
        GetActiveBoundsAtNode(node._rChildIdx, bounds);
    }
}