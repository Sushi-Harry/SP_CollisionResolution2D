#ifndef SPATIAL_HASH_GRID_ALGO_H
#define SPATIAL_HASH_GRID_ALGO_H

#include "SpatialTypes.h"
#include "SpatialHashGrid.h"
#include <vector>

// Not adding comments in this one cause thhis is essentially the same code used for the other algo implementations

int RunSpatialHashmapCollisionCheck(std::vector<Entity>& entities, const SpatialHashmap& grid, int& outChecksDone){
    int collisionCount = 0;
    outChecksDone = 0;

    for(auto& e: entities){
        AABB queryBox ={
            ._center = e._position,
            ._halfDimension = Vec2{e._radius, e._radius}
        };

        std::vector<Entity*> candidates;
        grid.QueryRange(queryBox, candidates, outChecksDone);

        for(Entity* E : candidates){
            if(E->_id <= e._id) continue;
            outChecksDone++;

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