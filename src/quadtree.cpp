#include "../include/quadtree.h"

//QuadTree
//{

int MAX_NODE_CAPACITY = 4, MIN_NODE_CAPACITY = 5, MAX_DEPTH = 4;

void QuadTree::cleanUp() {
    count = 0;
    if(!obj.empty())
        obj.clear();
    if(has) {
        for(int i=0; i<4; i++) {
            children[i]->cleanUp();
            delete children[i];
        }
    }
}

QuadTree::QuadTree() {
    count=0;
}

QuadTree::QuadTree(AABB2 b, int d) {
    has=0;
    boundary = b;
    depth = d;
    count=0;
}

std::vector<short> QuadTree::whereTo(AABB2 p) {
    std::vector<short> rez;
    glm::vec2 a = p.a, b = p.b, c=boundary.c;
    if(a.x< c.x) {
        if(a.y< c.y)
            rez.push_back(0);
        if(b.y>= c.y)
            rez.push_back(2);
    }
    if(b.x>= c.x) {
        if(a.y< c.y)
            rez.push_back(1);
        if(b.y>= c.y)
            rez.push_back(3);
    }
    return rez;
}

void QuadTree::insert(int a ,AABB2 p) {
    count++;
    if(!has && count > MAX_NODE_CAPACITY && depth< MAX_DEPTH) {
        obj.insert(a);
        subdivide();
        return;
    }
    if(has) {
        std::vector<short> rez = whereTo(p);
        for(int i=0; i<rez.size(); i++) {
            children[rez[i]]->insert(a, p);
        }
    } else {
        obj.insert(a);
    }
}

void QuadTree::subdivide() {
    has=true;
    children[0]=new QuadTree(AABB2(boundary.a,boundary.c),depth+1);
    children[1]=new QuadTree(AABB2(glm::vec2(boundary.c.x,boundary.a.y),glm::vec2(boundary.b.x,boundary.c.y)),depth+1);
    children[2]=new QuadTree(AABB2(glm::vec2(boundary.a.x,boundary.c.y),glm::vec2(boundary.c.x,boundary.b.y)),depth+1);
    children[3]=new QuadTree(AABB2(boundary.c,boundary.b),depth+1);
    for(std::set<int >::iterator it=obj.begin(); it!=obj.end(); it++) {
        std::vector<short> rez = whereTo(cars[(*it)].box);
        for(int i=0; i<rez.size(); i++) {
            children[rez[i]]->insert(*it,cars[(*it)].box);
        }
    }
    obj.clear();
}

void QuadTree::colect(std::set<int > &here) {
    for(std::set<int >::iterator it=obj.begin(); it!=obj.end(); it++)
        here.insert(*it);
    obj.clear();
}

void QuadTree::kill() {
    for(int i=0; i<4; i++) {
        if(children[i]->has)
            children[i]->kill();
        children[i]->colect(obj);
        children[i]->cleanUp();
    }
    has=false;
}

void QuadTree::remove(int  a, AABB2 rem) {
    count--;
    if(has && count < MIN_NODE_CAPACITY) {
        kill();
    }
    if(has) {
        std::vector<short> rez = whereTo(rem);
        for(int i=0; i<rez.size(); i++) {
            children[rez[i]]->remove(a, rem);
        }
    } else {
        obj.erase(a);
        return;
    }
}

void QuadTree::findMultiple() {
    if (has && count) {
        for(int i=0; i<4; i++)
            children[i]->findMultiple();
    } else if(count) {
        for(std::set<int >::iterator it=obj.begin(); it!=obj.end(); it++) {
            for(std::set<int >::iterator jt=it; jt!=obj.end(); jt++) {
                if(it!=jt) {
                    collide(cars[*it].cc,cars[*jt].cc);
                }
            }
        }
    }
}

void QuadTree::findWall(int w) {
    if (has && count) {
        if(w==-1) {
            children[0]->findWall(0);
            children[0]->findWall(3);
            children[1]->findWall(0);
            children[1]->findWall(1);
            children[2]->findWall(3);
            children[2]->findWall(2);
            children[3]->findWall(1);
            children[3]->findWall(2);
        }
        if(w==0) {
            children[0]->findWall(0);
            children[0]->findWall(3);
            children[1]->findWall(0);
            children[1]->findWall(1);
        }
        if(w==1) {
            children[1]->findWall(0);
            children[1]->findWall(1);
            children[3]->findWall(1);
            children[3]->findWall(2);
        }
        if(w==2) {
            children[2]->findWall(3);
            children[2]->findWall(2);
            children[3]->findWall(1);
            children[3]->findWall(2);
        }
        if(w==3) {
            children[0]->findWall(0);
            children[0]->findWall(3);
            children[2]->findWall(3);
            children[2]->findWall(2);
        }
    } else if(count) {
        for(std::set<int>::iterator it=obj.begin(); it!=obj.end(); it++) {
            collideWall(cars[*it].cc, w);
        }
    }
}

void QuadTree::draw() {
    glm::vec2 a= glm::vec2(boundary.a.x+1,boundary.a.y+1), b=glm::vec2(boundary.b.x-1,boundary.b.y-1);
    if(true) {
        float v[4*3];
        v[0] = a.x;
        v[1]=0;
        v[2] = a.y;
        v[3] = a.x;
        v[4]=0;
        v[5] = b.y;
        v[6] = b.x;
        v[7]=0;
        v[8] = b.y;
        v[9] = b.x;
        v[10]=0;
        v[11] = a.y;
        GLuint vbo;
        glEnableVertexAttribArray(0);
        glGenBuffers(1, &vbo);
        //vertexes
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), v, GL_STATIC_DRAW);

        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glDrawArrays( GL_LINE_LOOP, 0,4);
        glBindVertexArray(0);
    }
    if(has) {
        for(int i=0; i<4; i++)
            children[i]->draw();
    }
}
//}

