#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include "math.h"

const int MAX_NODE_CAPACITY = 3, MIN_NODE_CAPACITY = 6, MAX_DEPTH = 3;

//QuadTree
//{

struct QuadTree {
    AABB2 boundary;
    std::vector<boost::shared_ptr<object> > obj;
    iter it;
    QuadTree* children[4];
    bool has;
    int depth, count;

    void cleanUp();

    QuadTree();

    QuadTree(AABB2 b, int d);

    std::vector<short> whereTo(AABB2 p);

    void insert(boost::shared_ptr<object> a ,AABB2 p);

    void subdivide();

    void colect(std::set<boost::shared_ptr<object> > &here);

    void kill();

    void remove(boost::shared_ptr<object>  a, AABB2 rem);

    void findMultiple();

    void findWall(int w);

    void draw();
};

void collide(car c1, car c2);
void collideWall(car c, int w);

//}

#endif // QUADTREE_H
