#ifndef QUAD_TREE_ALGO_H
#define QUAD_TREE_ALGO_H

#include "SpatialTypes.h"
#include "QuadTree.h"
#include <vector>

int RunQuadTreeCollisionCheck(std::vector<Entity>& entities, const QuadTree& tree, int &outCheckCount){
    int collisionCount = 0;
    outCheckCount = 0;

    for(auto &e : entities){
        // First we'll create a bounding box / query range that perfectly wraps the current entity (Circle shaped entities for simplicity)
        AABB queryBox = {
            ._center = e._position,
            ._halfDimension = Vec2{e._radius, e._radius}
        };

        // This candidates vector will be used to store the entities that need to be checked for collision
        std::vector<Entity*> candidates;
        tree.QueryRange(queryBox, candidates, outCheckCount);

        // Now this is where it gets a bit better than the previous Naive Algo
        for(Entity* E : candidates){
            // Since IDs are generated in the order of how they're added. Entity 1 has id 1, Entity 2 has id 2 and so on
            // Prevents double checking and self checking
            if(E->_id <= e._id) continue;

            outCheckCount++;

            float dx = e._position.x - E->_position.x;
            float dy = e._position.y - E->_position.y;
            float distSq = dx*dx + dy*dy;
            float rSum = e._radius + E->_radius;
            if(distSq < rSum*rSum){
                collisionCount++;
                e._colliding = true;
                E->_colliding = true;
            }
        }
    }

    return collisionCount;
}

#endif