#include "SpatialHashGrid.h"

SpatialHashmap::SpatialHashmap(uint32_t width, uint32_t height, uint32_t cellSize){
    _cellSize = cellSize;
    _columns = std::ceil((double)width / cellSize);
    _rows = std::ceil((double)height / cellSize);
    // Ahahahahaha
    _cells.resize(_columns * _rows);
}

void SpatialHashmap::Clear(){
    for(auto& cell : _cells){
        cell.clear();
    }
}

// Now this insertion function right here works in O(1) time
void SpatialHashmap::Insert(Entity* e){
    // Basically using the position of the entity as the data fed to the hashing function
    int idx = getCellIndex(e->_position.x, e->_position.y);
    _cells[idx].push_back(e);
}

// this function basically just fetches the entities that need to be checked for collision
void SpatialHashmap::QueryRange(AABB range, std::vector<Entity*>& found, int& checksDone) const {
    int colMin = (int)((range._center.x - range._halfDimension.x) / _cellSize);
    int colMax = (int)((range._center.x + range._halfDimension.x) / _cellSize);
    int rowMin = (int)((range._center.y - range._halfDimension.y) / _cellSize);
    int rowMax = (int)((range._center.y + range._halfDimension.y) / _cellSize);

    // Clamp the values to the boundary of the grid we're working with here
    // Totally unrelated but man spotify's music suggestions suck so bad. How is ASAP Rocky a good suggestion to listen to after Callin' U (Elyanna) ???
    if(colMin < 0) colMin = 0;
    if(colMax >= _columns) colMax = _columns - 1;
    if(rowMin < 0) rowMin = 0;
    if(rowMax >= _rows) rowMax = _rows - 1;

    // Looping over the overlapped cells and basically just pushing the entities found in the cells to the "found" vector
    for(int row = rowMin; row <= rowMax; row++){
        for(int col = colMin; col <= colMax; col++){
            checksDone++;
            int idx = row * _columns + col;
            for(Entity *e : _cells[idx]){
                found.push_back(e);
            }
        }
    }
}

uint32_t SpatialHashmap::getCellIndex(double x, double y) const {
    int col = (int)(x / _cellSize);
    int row = (int)(y / _cellSize);

    if(col < 0) col = 0;
    else if(col >= _columns) col = _columns - 1;
    if(row < 0) row = 0;
    else if(row >= _rows) row = _rows - 1;

    return row * _columns + col;
}