#ifndef BVH_ALGO_H
#define BVH_ALGO_H

#include "SpatialTypes.h"
#include "BVH.h"
#include <vector>

int RunBVHCollisionCheck(std::vector<Entity>& entities, const BoundingVolumeHierarchy& tree, int &outCheckCount){
    int collisionCount = 0;
    outCheckCount = 0;

    for(auto &e : entities){
        AABB queryBox = {
            ._center =  e._position,
            ._halfDimension = Vec2{e._radius, e._radius}
        };

        std::vector<Entity*> candidates;
        tree.QueryRange(queryBox, candidates, outCheckCount);

        for(Entity* E : candidates){
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