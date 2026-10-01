## MISCELLANEOUS INFO (Not Really)

Alright so I added this little MD file to store info on the design choices I'm making.
The QuadTree kinda just has to be cleared and rebuilt every frame according to my research. Surprisingly enough, this is also the industry standard in physics systems. (Or at least for game engine physics.)
But! The caveat in my current QuadTree implementation is that if I clear it, I'm calling the following block every frame:

```
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
```

But what's the Issue with that you might ask... the answer to that lies in what I do when I need to rebuild the tree every frame. <br />This is the block that is called when I'm trying to subdivide the tree further:
```
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
```
__Notice something?__<br />
Calling ``` new QuadTree({...}) ``` every frame is not a good idea for an application running at like 60 frames per second.
<br />

__But Why is it bad?__<br />
It's bad because everytime I call ```new``` I'm asking the system to find new memory on the heap which is not a fast process. At around 2000 entities, there will be at least hundreds (insert a thinking emoji here) of ```new``` and ```delete``` calls every single frame. That will lead to a major drop in performance because of just bad memory usage practices. 