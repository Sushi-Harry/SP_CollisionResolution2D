#include "Loose_QuadTree.h"

// No notes / comments here since almost all of these functions are identical to those in QuadTree.cpp

LooseQuadTree::LooseQuadTree(AABB boundary, int maxNodes = 10000) : _rootBoundary(boundary) {
    _nodes.resize(maxNodes);
    Clear();
}

void LooseQuadTree::Clear(){
    _nextNodeIdx = 1;
    _nodes[0]._boundary = _rootBoundary;
    _nodes[0]._entities.clear();
    _nodes[0]._subdivided = false;
}

bool LooseQuadTree::Insert(Entity* e){
    return InsertAtNode(0, e);
}

bool LooseQuadTree::InsertAtNode(int nodeIdx, Entity* e){
    LooseQuadTreeNode& node = _nodes[nodeIdx];

    if(!node._boundary.containsPoint(e->_position)) return false;

    if(node._entities.size() < _CAPACITY && !node._subdivided){
        node._entities.push_back(e);
        return true;
    }

    if(!node._subdivided){
        SubDivide(nodeIdx);
    }

    LooseQuadTreeNode& current = _nodes[nodeIdx];

    if(InsertAtNode(current._firstChild + 0, e)) return true;
    if(InsertAtNode(current._firstChild + 1, e)) return true;
    if(InsertAtNode(current._firstChild + 2, e)) return true;
    if(InsertAtNode(current._firstChild + 3, e)) return true;

    return false;
}

void LooseQuadTree::SubDivide(int nodeIdx){
    int childIdx = _nextNodeIdx;
    _nextNodeIdx += 4;

    if(_nextNodeIdx > _nodes.size()){
        _nodes.resize(_nodes.size()  * 2);
    }

    LooseQuadTreeNode& node = _nodes[nodeIdx];
    node._firstChild = childIdx;
    node._subdivided = false;

    Vec2 h = {
        node._boundary._halfDimension.x / 2.0,
        node._boundary._halfDimension.y / 2.0
    };
    Vec2 c = {
        node._boundary._center.x / 2.0,
        node._boundary._center.y / 2.0
    };

    // Now this is where it differs
    // Applying the looseness factor with this line
    Vec2 looseH = {
        h.x * _k,
        h.y * _k
    };

    _nodes[childIdx + 0]._boundary = {{ c.x - h.x, c.y - h.y }, looseH};
    _nodes[childIdx + 1]._boundary = {{ c.x + h.x, c.y - h.y }, looseH};
    _nodes[childIdx + 2]._boundary = {{ c.x - h.x, c.y + h.y }, looseH};
    _nodes[childIdx + 3]._boundary = {{ c.x + h.x, c.y + h.y }, looseH};

    for(int i = 0; i < 4; i++){
        _nodes[childIdx + i]._entities.clear();
        _nodes[childIdx + i]._subdivided = false;
    }
}

void LooseQuadTree::QueryRange(AABB range, std::vector<Entity*>& found, int& checksDone) const {
    QueryAtNode(0, range, found, checksDone);
}

void LooseQuadTree::QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksDone) const {
    const LooseQuadTreeNode& node = _nodes[nodeIdx];
    checksDone++;
    if(!node._boundary.intersects(range)) return;

    for(const auto& p : node._entities){
        checksDone++;
        if(range.containsPoint(p->_position)){
            found.push_back(p);
        }
    }

    if(node._subdivided){
        QueryAtNode(node._firstChild + 0, range, found, checksDone);
        QueryAtNode(node._firstChild + 1, range, found, checksDone);
        QueryAtNode(node._firstChild + 2, range, found, checksDone);
        QueryAtNode(node._firstChild + 3, range, found, checksDone);
    }
}

void LooseQuadTree::GetActiveBounds(std::vector<AABB>& bounds) const {
    GetActiveBoundsAtNode(0, bounds);
}

void LooseQuadTree::GetActiveBoundsAtNode(int nodeIdx, std::vector<AABB>& bounds) const {
    const LooseQuadTreeNode& node = _nodes[nodeIdx];
    
    bounds.push_back(node._boundary);
    
    if (node._subdivided) {
        GetActiveBoundsAtNode(node._firstChild + 0, bounds);
        GetActiveBoundsAtNode(node._firstChild + 1, bounds);
        GetActiveBoundsAtNode(node._firstChild + 2, bounds);
        GetActiveBoundsAtNode(node._firstChild + 3, bounds);
    }
}