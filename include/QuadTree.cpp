#include "QuadTree.h"

void QuadTree::Clear(){
    _entities.clear();
    if(_subdivided){
        delete _00;
        delete _10;
        delete _11;
        delete _01;
        // Kinda crazy how this is valid syntax, huh
        _00 = _10 = _01 = _11 = nullptr;
        _subdivided = false;
    }
}

// Function to further subdivide the tree. Nothing major, really. Shouoldn't be hard to understand
void QuadTree::SubDivide(){
    Vec2 h = {
        _boundary._halfDimension.x / 2.0F,
        _boundary._halfDimension.y / 2.0F
    };
    Vec2 c = _boundary._center;
    _00 = new QuadTree({
        {c.x - h.x, c.y - h.y},
        h
    });
    _10 = new QuadTree({
        {c.x + h.x, c.y - h.y},
        h
    });
    _01 = new QuadTree({
        {c.x - h.x, c.y + h.y},
        h
    });
    _11 = new QuadTree({
        {c.x + h.x, c.y + h.y},
        h
    });
    _subdivided = true;
}

// Insertion function
bool QuadTree::Insert(Entity &E){
    // No need to insert it if the boundary already contains the position of the passed entity.
    // Because yk, it already exists
    if(!_boundary.containsPoint(E._position)) return false;

    if(_entities.size() < _CAPACITY && !_subdivided){
        _entities.push_back(E);
        return true;
    }

    if(!_subdivided)
        SubDivide();

    if(_00->Insert(E)) return true;
    if(_10->Insert(E)) return true;
    if(_01->Insert(E)) return true;
    if(_11->Insert(E)) return true;

    return false;
}

void QuadTree::QueryRange(AABB range, std::vector<Entity>& found, int &checksPerformed) const{
    checksPerformed++;
    if(!_boundary.intersects(range)) return;

    for(const auto &p : _entities){
        checksPerformed++;
        if(range.containsPoint(p._position))
            found.push_back(p);
    }
    // if the screen is further subdivided then call this function recursively to check other subdivisions
    if(!_subdivided){
        _00->QueryRange(range, found, checksPerformed);
        _10->QueryRange(range, found, checksPerformed);
        _01->QueryRange(range, found, checksPerformed);
        _11->QueryRange(range, found, checksPerformed);
    }
}