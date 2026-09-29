#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SpatialTypes.h"
#include <vector>
#include <cstdlib>

inline void SpawnEntities(std::vector<Entity>& entities, uint32_t count, float boundWidth, float boundHeight){
    entities.clear();
    for(int i = 0; i < count; i++){
        Entity e = {
            ._id = i,
            ._position = { (float)(rand() % (int)boundWidth), (float)(rand() % (int)boundHeight) },
            ._velocity = { ((float)(rand() % 200) - 100.0F),  ((float)(rand() % 200) - 100.0F) },
            ._radius = 10.0F,
        };

        entities.push_back(e);
    }
}

inline void UpdateEntityPositions(std::vector<Entity>& entities, float boundWidth, float boundHeight, float dt){
    for(auto &e : entities){
        e._position.x += e._velocity.x * dt;
        e._position.y += e._velocity.y * dt;
        if(e._position.x < 0 || e._position.x > boundWidth) e._velocity.x *= -1;
        if(e._position.y < 0 || e._position.y > boundHeight) e._velocity.y *= -1;

        e._colliding = false;
    }
}

#endif