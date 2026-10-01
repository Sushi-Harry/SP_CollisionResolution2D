#ifndef QUADTREE_H
#define QUADTREE_H

#include "SpatialTypes.h"
#include <vector>

struct QuadTreeNode{
    AABB _boundary;
    std::vector<Entity*> _entities;
    bool _subdivided = false;
    /*
        Il s'agit de l'index du premier enfant issu de la subdivision en quatre du quadtree actuel dans le pool de nœuds.
        Este es el índice del primer hijo de la subdivisión actual en cuatro partes del árbol cuaternario (quadtree) en el grupo de nodos (Node Pool).
        This is the index of the first child resulting from the current four-part subdivision of the quadtree within the node pool.
    */
    int _firstChild = -1;
};

class QuadTree{
public:
    QuadTree(AABB boundary, int maxNodes = 10000);
    ~QuadTree() = default;

    void Clear();
    bool Insert(Entity *e);
    void QueryRange(AABB range, std::vector<Entity*>& found, int &checksPerformed) const;

private:
    static const int _CAPACITY = 4;
    AABB _rootBoundary;

    std::vector<QuadTreeNode> _nodes;
    int _nextNodeIdx = 0;

    void SubDivide(int nodeIdx);
    bool InsertAtNode(int nodeIdx, Entity* e);
    void QueryAtNode(int nodeIdx, AABB range, std::vector<Entity*>& found, int& checksPerformed) const;

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