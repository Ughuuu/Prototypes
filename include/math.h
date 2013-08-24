#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#define PI 3.14159265
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

extern float EPSILON,
       Cos[360],
       Sin[360];

///Vectors
//{
struct V2 : public sf::Vector2f {
    void debug();
    V2();
    V2(float a , float b);
    bool operator==(V2 a);
    bool operator!=(V2 a);
    V2 operator/(float a);
    V2 operator-(float a);
    V2 operator+(float a);
    V2 operator*(float a);

    void operator/=(float a);
    void operator-=(float a);
    void operator+=(float a);
    void operator*=(float a);

    V2 operator-(V2 a);
    V2 operator+(V2 a);

    void operator-=(V2 a);
    void operator+=(V2 a);

    float magn();
    float sqmagn();
    float dot(V2 a);
    V2 cross(V2 a);
    V2 norm();
};

struct V3 : public sf::Vector3f {
    void debug();
    V3();
    V3(float a , float b , float c);
    bool operator==(V3 a);
    bool operator!=(V3 a);
    V3 operator/(float a);
    V3 operator-(float a);
    V3 operator+(float a);
    V3 operator*(float a);

    void operator/=(float a);
    void operator-=(float a);
    void operator+=(float a);
    void operator*=(float a);

    V3 operator-(V3 a);
    V3 operator+(V3 a);

    void operator-=(V3 a);
    void operator+=(V3 a);
    float operator*(V3 a);
    float magn();
    float sqmagn();
    V3 cross(V3 a);
    float dot(V3 a);
    V3 norm();
};
//}

///Matrices
//{
struct M2 {
    void debug();
    float m[4];
    //m1
    M2();
    M2(float a[4]);
    M2(V2 a, V2 b);
    M2(float a1, float a2, float a3, float a4);
    M2 operator*(M2 a);
    M2 operator+(M2 a);
    M2 operator-(M2 a);
    M2 operator/(M2 a);
    M2 operator*(float a);
    M2 operator+(float a);
    M2 operator-(float a);
    M2 operator/(float a);
    M2 inv();
    M2 scale(V2 v);//scale
    M2 shearx(float x);//shear along x
    float det();
    M2 t();
    M2 rot(float a);
    V2 operator*(V2 v);
};

struct M3 {
    void debug();
    float m[9];
    //m1
    M3();
    M3(float a[9]);
    M3(V3 a, V3 b, V3 c);
    M3(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9);
    M3 operator*(M3 a);
    M3 operator+(M3 a);
    M3 operator-(M3 a);
    M3 operator/(M3 a);
    M3 operator*(float a);
    M3 operator+(float a);
    M3 operator-(float a);
    M3 operator/(float a);
    M3 inv();
    float det();
    M3 t();
    M3 rotx(float a);
    M3 roty(float a);
    M3 rotz(float a);
    M3 rot(V3 v, float a);
    V3 operator*(V3 v);
};
//}

///AABB2
//{
struct AABB2 {
    //a - left top
    //b - right bottom
    //c - center
    //d - dimensions/2
    V2 a, b, c, d;

    AABB2(V2 a1, V2 b1);

    AABB2() {}

    bool containsPoint(V2 p);

    bool intersectsAABB2(AABB2 o);

    //POL
};
//}

///AABB3
//{
struct AABB3 {
    V3 a, b, c, d;

    AABB3(V3 a1, V3 b1);

    AABB3() {}

    bool containsPoint(V3 p);

    bool intersectsAABB3(AABB3 o);
};

//}

///POLYGONS
//{
struct POL {
    V2 p;
    std::vector<V2>  v, n;
    void genN();
    void Centerize();
    POL();
    POL(std::vector<V2> v1);
    POL(std::vector<V2> v1, std::vector<V2>n1);
    POL(V2 p1, std::vector<V2> v1);
    POL(V2 p1, std::vector<V2> v1, std::vector<V2>n1);
};


bool check_col(POL p1, POL p2, V2 &ax, float &mtv, V2 &contact);
bool check_col(POL p1, POL p2, V2 &ax, float &mtv);
bool check_col(POL p1, POL p2);
//}

float dist(V2 p1, V2 p2);
float sqdist(V2 p1, V2 p2);
float dist(V3 p1, V3 p2);
float sqdist(V3 p1, V3 p2);

#endif
