#ifndef NAIVE_ALGO_H
#define NAIVE_ALGO_H
#include "SpatialTypes.h"
#include <vector>

// Unnecessarily sleepy right now. Took me 10+ minutes to write this simple function. That's how sleepy I am right now.
int RunBruteForceCollisionCheck(const std::vector<Entity>& entities, int &outCheckCount){
    int collisions = 0;
    outCheckCount = 0;

    size_t n = entities.size();
    for(size_t i = 0; i < n; i++){
        for(size_t j = i + 1; j < n; j++){
            outCheckCount++;
            float dx = entities[i]._position.x - entities[j]._position.x;
            float dy = entities[i]._position.y - entities[j]._position.y;
            float distSq = dx*dx + dy*dy;
            float rsum = entities[i]._radius + entities[j]._radius;
            if(distSq <= rsum*rsum){
                collisions++;
            }
        }
    }
    return collisions;
}

#endif