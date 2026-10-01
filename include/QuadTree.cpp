#include "QuadTree.h"

/*
THIS IS THE 2ND TIME I'M IMPLEMENTING THIS STRUCTURE. WHY? Cause the last implementation was inefficient.
*/

QuadTree::QuadTree(AABB boundary, int maxNodes) : _rootBoundary(boundary) {
    _nodes.resize(maxNodes);
    Clear();
}
// function to clear the tree
void QuadTree::Clear(){
    // setting it to one for reserving 1 space at the beginning for the root node
    _nextNodeIdx = 1;

    // resetting the root node's data for the new frame
    _nodes[0]._boundary = _rootBoundary;
    _nodes[0]._subdivided = false;
    _nodes[0]._entities.clear();
}

// Insert function
bool QuadTree::Insert(Entity *e){
    return InsertAtNode(0, e);
}
bool QuadTree::InsertAtNode(int nodeIdx, Entity *e){
    QuadTreeNode& node = _nodes[nodeIdx];

    if(!node._boundary.containsPoint(e->_position)) return false;
    if(node._entities.size() < _CAPACITY && !node._subdivided){
        node._entities.push_back(e);
        return true;
    }

    if(!node._subdivided){
        SubDivide(nodeIdx);
    }

    // Why am I doing this again? Because subdivision has a chance of doing a vector resize. So this is justa  refresh
    QuadTreeNode& current = _nodes[nodeIdx];

    // Check the 4 children in the array
    if (InsertAtNode(current._firstChild + 0, e)) return true; // _00
    if (InsertAtNode(current._firstChild + 1, e)) return true; // _10
    if (InsertAtNode(current._firstChild + 2, e)) return true; // _01
    if (InsertAtNode(current._firstChild + 3, e)) return true; // _11

    return false;
}

void QuadTree::SubDivide(int nodeIdx){
    int childIdx = _nextNodeIdx;
    _nextNodeIdx += 4;

    if(_nextNodeIdx > _nodes.size()){
        _nodes.resize(_nodes.size() * 2);
    }

    QuadTreeNode& node = _nodes[nodeIdx];
    node._firstChild = childIdx;
    node._subdivided = true;

    Vec2 h = {
        node._boundary._halfDimension.x / 2.0,
        node._boundary._halfDimension.y / 2.0
    };

    Vec2 c = node._boundary._center;

    _nodes[childIdx + 0]._boundary = { {c.x - h.x, c.y - h.y}, h };
    _nodes[childIdx + 1]._boundary = { {c.x + h.x, c.y - h.y}, h };
    _nodes[childIdx + 2]._boundary = { {c.x - h.x, c.y + h.y}, h };
    _nodes[childIdx + 3]._boundary = { {c.x + h.x, c.y + h.y}, h };

    for(int i = 0; i < 4; i++){
        _nodes[childIdx + i]._entities.clear();
        _nodes[childIdx + i]._subdivided = false;
    }
}

void QuadTree::QueryRange(AABB range, std::vector<Entity*>& found, int& checksPerformed) const {
    QueryAtNode(0, range, found, checksPerformed);
}

void QuadTree::QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksPerformed) const {
    const QuadTreeNode& node = _nodes[nodeIdx];

    checksPerformed++;
    if(!node._boundary.intersects(range)) return;

    for(const auto& p : node._entities){
        checksPerformed++;
        if(range.containsPoint(p->_position)){
            found.push_back(p);
        }
    }
    if(node._subdivided){
        QueryAtNode(node._firstChild + 0, range, found, checksPerformed);
        QueryAtNode(node._firstChild + 1, range, found, checksPerformed);
        QueryAtNode(node._firstChild + 2, range, found, checksPerformed);
        QueryAtNode(node._firstChild + 3, range, found, checksPerformed);
    }
}

// Helper Functions
void QuadTree::GetActiveBounds(std::vector<AABB>& bounds) const {
    GetActiveBoundsAtNode(0, bounds);
}

void QuadTree::GetActiveBoundsAtNode(int nodeIdx, std::vector<AABB>& bounds) const {
    const QuadTreeNode& node = _nodes[nodeIdx];

    bounds.push_back(node._boundary);

    if(node._subdivided){
        GetActiveBoundsAtNode(node._firstChild + 0, bounds);
        GetActiveBoundsAtNode(node._firstChild + 1, bounds);
        GetActiveBoundsAtNode(node._firstChild + 2, bounds);
        GetActiveBoundsAtNode(node._firstChild + 3, bounds);
    }
}