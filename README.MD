# 2D Spatial Partitioning and Collision Detection

This is a real-time 2D Collision Detection Benchmark sort of thing that compares the performance using different data structures.
(One of the major limitations here is that all the entities are basically just circles of the exact same dimensions)

Made using __Raylib__ for graphics and __Raygui__ for that little GUI menu.

### Implemented Data Structures
1. __Standard Quad Trees__ <br />
Recursively divides the 2d space and its subdivisions into 4 parts to minimize the checking area for the collision checking candidate entities.

2. __Loose Quad Trees__ <br />
This is basically Quad Tree but a little bit more lenient. The boundaries between the nodes isn't as strict as in a normal quad tree. In a normal quad tree, the object on a boundary line would be moved up to a higher node in the quad tree but in a loose quad tree, that doesn't happen. Instead, due to the overlapping nature of the nodes, the entity on the border doesn't have to be moved up as frequently (or moved up at all sometimes)

3. __Spatial Hash Mapping__ <br />
This is actually very simple. Just uses the screen coordinates of the entities to fill them into a hash grid. The insertion is very fast ( O(1) time cause it just takes in the integer coordinates and calculates the index of the entity in the map based on that)
```
uint32_t SpatialHashmap::getCellIndex(double x, double y) const {
    int col = (int)(x / _cellSize);
    int row = (int)(y / _cellSize);

    if(col < 0) col = 0;
    else if(col >= _columns) col = _columns - 1;
    if(row < 0) row = 0;
    else if(row >= _rows) row = _rows - 1;

    return row * _columns + col;
}
``` 

4. __BVH (Bounding Volume Hierarchy)__ <br />
It's basically a binary tree made of Shapes as internal nodes and the leaf nodes being the actual circles that could be intersecting.
Essentially, it all starts with a root shape that has all the circles and then according to the dimensions of that shape, we'll split it so that roughly half of the circles are in one new subbdivided shape and the rest in the other half  subdivided shape. Here, the shape is a rectangle so every subsequent subdivision is also a rectangle. It goes on till the subdivisions reach a point where that only one circle remains in the subdivision which becomes the root node.
__How it checks for collision?__ So let's say we're checking for a circle called 'X'. The root box essentially contains all circles so we'll go into one of its children. Now we check which one of the child boxes X's query box intersects and we go further into whichever one it intersects.
If the hitbox of X doesn't overlap a major bounding box (a higher node of the tree), then we just skip that entire subtree since it is guaranteed to not be overlapping with the hitboxs of any of the other leaf nodes in that sub tree. Hence, reducing the number of candidates we need to add to the list for the final checking process!

## Performance
Some performance numbers I gathered from the application:

| Data Structure | Collision Checks with 100 Entities | Collision Checks with 1,000 Entities |
| --- | --- | --- |
| Spatial Hash Grid | ~250 | ~4,000 |
| BVH | ~1,700 | ~32,000 |
| QuadTree | ~2,500 | ~44,000 |
| Loose QuadTree (K=2.0) | ~7,200 | ~660,000 |

## Build & Run
Requires a C++ compiler and __Raylib__
```bash
make
.\main.exe
```