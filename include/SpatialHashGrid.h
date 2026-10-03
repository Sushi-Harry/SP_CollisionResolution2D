#ifndef SPATIAL_HASH_GRID_H
#define SPATIAL_HASH_GRID_H

#include <cstdint>
#include <vector>
#include "SpatialTypes.h"
// We'll need like a 1D array of BUCKETS!
/*
What's a bucket?:
    A bucket just holds pointers to the entities that are currently in the cell
    So it works like that bucket array we use in Bucket Sort (Not exactly but kinda. At least that's how I'm visualizing it right now)
*/

class SpatialHashmap{
public:
    SpatialHashmap(uint32_t width, uint32_t height, uint32_t cellSize);
    void Clear();
    void Insert(Entity* e);
    void QueryRange(AABB range, std::vector<Entity*>& found, int& checksDone) const;
    
private:
    uint32_t _cellSize;
    uint32_t _columns, _rows;
    std::vector<std::vector<Entity*>> _cells;

    uint32_t getCellIndex(double x, double y) const;
};

#endif