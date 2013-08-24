#include "../include/math.h"
#include <cmath>
#include <iostream>

using namespace std;

float EPSILON=1e-10,
       Cos[360],
       Sin[360];

///debug
//{
void V2::debug() {
    cout<<"---\n";
    cout<<"x:"<<x<<" y:"<<y;
}
void V3::debug() {
    cout<<"---\n";
    cout<<"x:"<<x<<" y:"<<y<<" z:"<<z<<'\n';
}
void M3::debug() {
    cout<<"---\n";
    cout<<"1: "<<m[0]<<" "<<m[1]<<" "<<m[2]<<'\n';
    cout<<"2: "<<m[3]<<" "<<m[4]<<" "<<m[5]<<'\n';
    cout<<"3: "<<m[6]<<" "<<m[7]<<" "<<m[8]<<'\n';
}
void M2::debug() {
    cout<<"---\n";
    cout<<"1: "<<m[0]<<" "<<m[1]<<'\n';
    cout<<"2: "<<m[2]<<" "<<m[3]<<'\n';
}
void V2inM2(V2 a, float *m) {
    *m=a.x;
    m++;
    m++;
    *m=a.y;
}
void V3inM3(V3 a, float *m) {
    *m=a.x;
    m++;
    m++;
    m++;
    *m=a.y;
    m++;
    m++;
    m++;
    *m=a.z;
}
//}
///V2
//{
V2::V2() {
    x=EPSILON;
    y=EPSILON;
}
V2::V2(float a , float b) {
    x=a;
    y=b;
}
V2 V2::operator/(float a) {
    if(a==0)a=EPSILON;
    return V2(x/a, y/a);
}
V2 V2::operator-(float a) {
    return V2(x-a, y-a);
}
V2 V2::operator+(float a) {
    return V2(x+a,y+a);
}
V2 V2::operator*(float a) {
    if(a==0)a=EPSILON;
    return V2(x*a,y*a);
}
void V2::operator/=(float a) {
    if(a==0)a=EPSILON;
    x/=a;
    y/=a;
}
void V2::operator-=(float a) {
    x-=a;
    y-=a;
}
void V2::operator+=(float a) {
    x+=a;
    y+=a;
}
void V2::operator*=(float a) {
    if(a==0)a=EPSILON;
    x*=a;
    y*=a;
}
V2 V2::operator-(V2 a) {
    return V2(x-a.x,y-a.y);
}
V2 V2::operator+(V2 a) {
    return V2(x+a.x,y+a.y);
}
void V2::operator-=(V2 a) {
    x-=a.x;
    y-=a.y;
}
void V2::operator+=(V2 a) {
    x+=a.x;
    y+=a.y;
}
bool V2::operator==(V2 a) {
    float e = EPSILON;
    if(fabs(a.x-x)<=e&& fabs(a.y-y)<=e)return 1;
    return 0;
}
bool V2::operator!=(V2 a) {
    float e = EPSILON;
    if(fabs(a.x-x)<=e&& fabs(a.y-y)<=e)return 0;
    return 1;
}
V2 V2::norm() {
    float d = sqrt(x*x +y*y);
    if(d==0)d=EPSILON;
    return V2(x/d,y/d);
}
float V2::dot(V2 a) {
    return x * a.x + y * a.y ;
}
float V2::magn() {
    float d = x*x + y*y;
    if(d==0)d=EPSILON;
    return d;
}
float V2::sqmagn() {
    float d = sqrt(x*x + y*y);
    if(d==0)d=EPSILON;
    return d;
}
float dist(V2 p1 , V2 p2) {
    return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);
}
float sqdist(V2 p1 , V2 p2) {
    return sqrt(dist(p1,p2));
}
//}

///V3
//{
V3::V3() {
    x=EPSILON;
    y=EPSILON;
    z=EPSILON;
}
V3::V3(float a , float b, float c) {
    x=a;
    y=b;
    z=c;
}
V3 V3::operator/(float a) {
    if(a==0)a=EPSILON;
    return V3(x/a, y/a, z/a);
}
V3 V3::operator-(float a) {
    return V3(x-a, y-a, z-a);
}
V3 V3::operator+(float a) {
    return V3(x+a,y+a, z+a);
}
V3 V3::operator*(float a) {
    return V3(x*a,y*a, z*a);
}

void V3::operator/=(float a) {
    if(a==0)a=EPSILON;
    x/=a;
    y/=a;
    z/=a;
}
void V3::operator-=(float a) {
    x-=a;
    y-=a;
    z-=a;
}
void V3::operator+=(float a) {
    x+=a;
    y+=a;
    z+=a;
}
void V3::operator*=(float a) {
    if(a==0)a=EPSILON;
    x*=a;
    y*=a;
    z*=a;
}


V3 V3::operator-(V3 a) {
    return V3(x-a.x,y-a.y, z-a.z);
}
V3 V3::operator+(V3 a) {
    return V3(x+a.x,y+a.y, z+a.z);
}

void V3::operator-=(V3 a) {
    x-=a.x;
    y-=a.y;
    z-=a.z;
}
void V3::operator+=(V3 a) {
    x+=a.x;
    y+=a.y;
    z+=a.z;
}

bool V3::operator==(V3 a) {
    float e = EPSILON;
    if(fabs(a.x-x)<=e&& fabs(a.y-y)<=e&& fabs(a.z-z)<=e)return 1;
    return 0;
}
bool V3::operator!=(V3 a) {
    float e = EPSILON;
    if(fabs(a.x-x)<=e&& fabs(a.y-y)<=e&& fabs(a.z-z)<=e)return 0;
    return 1;
}
float V3::dot(V3 a) {
    return x * a.x + y * a.y + z*a.z;
}
V3 V3::cross(V3 a) {
    return V3(y * a.z - z * a.y,
              z * a.x - x * a.z,
              x * a.y - y * a.x );
}
V3 V3::norm() {
    float d = sqrt(x*x +y*y +z*z);
    if(d==0)d=EPSILON;
    return V3(x/d,y/d, z/d);
}
float V3::magn() {
    float d = x*x + y*y + z*z;
    if(d==0)d=EPSILON;
    return d;
}
float V3::sqmagn() {
    float d = sqrt(x*x + y*y + z*z);
    if(d==0)d=EPSILON;
    return d;
}
float dist(V3 p1 , V3 p2) {
    return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
}
float sqdist(V3 p1 , V3 p2) {
    return sqrt(dist(p1,p2));
}
//}

///M2
//{
M2::M2() {
    m[0]=0;
    m[1]=0;
    m[2]=0;
    m[3]=0;
}
M2::M2(float a[4]) {
    m[0]=a[0];
    m[1]=a[1];
    m[2]=a[2];
    m[3]=a[3];
}
M2::M2(V2 a, V2 b) {
    V2inM2(a, &m[0]);
    V2inM2(b, &m[1]);
}
M2::M2(float a1, float a2, float a3, float a4) {
    m[0]=a1;
    m[1]=a2;
    m[2]=a3;
    m[3]=a4;
}
void Swap(float a, float b) {
    float aux;
    aux=a;
    a=b;
    b=aux;
}
M2 M2::t() {
    return M2(m[0],m[2],m[1],m[3]);
}
float M2::det() {
    return m[0]* m[3]-
           m[1]* m[2];
}
V2 M2::operator*(V2 v) {
    return V2(m[0]* v.x+ m[1]* v.y,
              m[2]* v.x+ m[3]* v.y);
}
M2 M2::operator*(M2 a) {
    int d=2;
    M2 c(0,0,0,0);
    for(int i=0; i<d; i++) {
        for(int j=0; j<d; j++)
            for(int l=0; l<d; l++)
                c.m[i*d+j]+=m[i*d+ l]* a.m[j+l*d];
    }
    return c;
}
M2 M2::operator+(M2 a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]+a.m[i];
    return c;
}
M2 M2::operator-(M2 a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]-a.m[i];
    return c;
}
M2 M2::operator/(M2 a) {
    return (*this)* a.inv();
}
M2 M2::operator+(float a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]+a;
    return c;
}
M2 M2::operator-(float a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]-a;
    return c;
}
M2 M2::operator*(float a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]*a;
    return c;
}
M2 M2::operator/(float a) {
    M2 c(0,0,0,0);
    int d=2;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]/a;
    return c;
}
M2 M2::inv() {
    float d=(*this).det();
    if(d==0)d=EPSILON;
    return M2(m[3], -m[1],
              -m[2], m[0]
             )*(1/d);
}
M2 M2::scale(V2 v) {
    return (*this)*M2(v.x,0
                      ,0,v.y);
}
M2 M2::shearx(float x) {
    return (*this)*M2(1 ,x,
                      0 ,1);
}
M2 M2::rot(float a) {
    return (*this)*M2(cos(a) ,-sin(a),
                      sin(a) ,cos(a));
}
//}

///M3
//{
M3::M3() {
    m[0]=0;
    m[1]=0;
    m[2]=0;
    m[3]=0;
    m[4]=0;
    m[5]=0;
    m[6]=0;
    m[7]=0;
    m[8]=0;
}
M3::M3(V3 a, V3 b, V3 c) {
    V3inM3(a, &m[0]);
    V3inM3(b, &m[1]);
    V3inM3(c, &m[2]);
}
M3::M3(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9) {
    m[0]=a1;
    m[1]=a2;
    m[2]=a3;
    m[3]=a4;
    m[4]=a5;
    m[5]=a6;
    m[6]=a7;
    m[7]=a8;
    m[8]=a9;
}
M3 M3::t() {
    return M3(m[0], m[3], m[6],
              m[1], m[4], m[7],
              m[2], m[5], m[8]);
}
float M3::det() {
    return m[0]* m[4]* m[8]+
           m[1]* m[5]* m[6]+
           m[2]* m[3]* m[7]-
           m[2]* m[4]* m[6]-
           m[1]* m[3]* m[8]-
           m[5]* m[7]* m[0];
}
V3 M3::operator*(V3 v) {
    return V3(m[0]* v.x+ m[1]* v.y+ m[2]* v.z,
              m[3]* v.x+ m[4]* v.y+ m[5]* v.z,
              m[6]* v.x+ m[7]* v.y+ m[8]* v.z);
}
M3 M3::operator*(M3 a) {
    int d=3;
    M3 c(0,0,0,0, 0, 0, 0, 0, 0);
    for(int i=0; i<d; i++) {
        for(int j=0; j<d; j++)
            for(int l=0; l<d; l++)
                c.m[i*d+j]+=m[i*d+ l]* a.m[j+l*d];
    }
    return c;
}
M3 M3::operator+(M3 a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]+a.m[i];
    return c;
}
M3 M3::operator-(M3 a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]-a.m[i];
    return c;
}
M3 M3::operator/(M3 a) {
    return (*this)* a.inv();
}
M3 M3::operator+(float a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]+a;
    return c;
}
M3 M3::operator-(float a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]-a;
    return c;
}
M3 M3::operator*(float a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]*a;
    return c;
}
M3 M3::operator/(float a) {
    M3 c(0,0,0,0,0,0,0,0,0);
    int d=3;
    for(int i=0; i<d*d; i++)
        c.m[i]=m[i]/a;
    return c;
}
M3 M3::rotx(float a) {
    a*=0.0174532925;
    return (*this)*M3(1, 0, 0,
                      0,cos(a),-sin(a),
                      0,sin(a),cos(a));
}
M3 M3::roty(float a) {
    a*=0.0174532925;
    return (*this)*M3(cos(a), 0, -sin(a),
                      0,      1,   0,
                      sin(a),0,cos(a));
}
M3 M3::rotz(float a) {
    a*=0.0174532925;
    return (*this)*M3(cos(a),-sin(a),0,
                      sin(a),cos(a),0,
                      0,0,1);
}
M3 M3::rot(V3 v, float a) {
    a*=0.0174532925;
    float Qx=v.x*sin(a/2),
           Qy=v.y*sin(a/2),
           Qz=v.z*sin(a/2),
           Qw=cos(a/2);
    V3 S(2* Qx, 2* Qy, 2* Qz),
    W=S*Qw;
    float xx= S.x* Qx,
           yy=S.y*Qy,
           zz=S.y*Qz,
           xy= S.y* Qx,
           yz=S.z*Qy,
           zx=S.x*Qz;
    M3 T(1 - (yy + zz),xy + W.z,zx - W.y,
         xy - W.z	,1 - (xx + zz),	yz + W.x,
         zx + W.y	,yz - W.x,	1 - (xx + yy));
    return (*this)*T;
}
M3 M3::inv() {
    float d=(*this).det();
    if(d==0)d=EPSILON;
    M3 I;
    I.m[0] = m[4] * m[8] - m[5] * m[7];
    I.m[1] = m[2] * m[7] - m[1] * m[8];
    I.m[2] = m[1] * m[5] - m[2] * m[4];
    I.m[3] = m[5] * m[6] - m[3] * m[8];
    I.m[4] = m[0] * m[8] - m[2] * m[6];
    I.m[5] = m[2] * m[3] - m[0] * m[5];
    I.m[6] = m[3] * m[7] - m[4] * m[6];
    I.m[7] = m[1] * m[6] - m[0] * m[7];
    I.m[8] = m[0] * m[4] - m[1] * m[3];
    return I*(1/d);
}
//}

///AABB2
//{
AABB2::AABB2(V2 a1, V2 b1) {
    c= a1;
    d= b1;
    a = c-d;
    b = c+d;
}
bool AABB2::containsPoint(V2 p) {
    if(a.x<p.x)
        if(b.x>p.x)
            if(a.y<p.y)
                if(b.y>p.y)
                    return true;
    return false;
}
bool AABB2::intersectsAABB2(AABB2 o) {
    return
        !(a.x > o.b.x || b.x < o.a.x ||
        a.y > o.b.y || b.y < o.a.y);
}
//}

///AABB3
//{
AABB3::AABB3(V3 a1, V3 b1) {
    c= a1;
    d= b1;
    a = c-d;
    b = c+d;
}
bool AABB3::containsPoint(V3 p) {
    if(a.x<p.x)
        if(b.x>p.x)
            if(a.y<p.y)
                if(b.y>p.y)
                    if(a.z<p.z)
                        if(b.z>p.z)
                            return true;
    return false;
}
bool AABB3::intersectsAABB3(AABB3 o) {
    return
        !(a.x > o.b.x || b.x < o.a.x ||
        a.y > o.b.y || b.y < o.a.y ||
        a.z > o.b.z || b.z < o.a.z);
}
//}

///POLYGONS
//{
POL::POL() {
    p=V2(0,0);
}
void POL::genN() {
    int vs = v.size();
    n.assign(vs, V2());
    n[0] = v[0] - v[vs-1];
    if(n[0].magn()<=EPSILON)n[0]=V2();
    n[0] = (V2(n[0].y, -n[0].x)).norm();
    for(int i=1; i<vs; i++) {
        n[i] = v[i] - v[i-1];
        if(n[i].magn()<=EPSILON)n[i]=V2();
        n[i] = V2(n[i].y, -n[i].x).norm();
    }
}

POL::POL(std::vector<V2> v1) {
    p=V2(0,0);
    v= v1;
    genN();
}

POL::POL(std::vector<V2> v1, std::vector<V2>n1) {
    p=V2(0,0);
    v = v1;
    n = n1;
}

POL::POL(V2 p1, std::vector<V2> v1) {
    p=p1;
    v= v1;
    genN();
}

POL::POL(V2 p1, std::vector<V2> v1, std::vector<V2>n1) {
    p=p1;
    v = v1;
    n = n1;
}

void POL::Centerize() {
    int vs = v.size();
    V2 center;
    for(int i=0; i<vs; i++) {
        center+=v[i];
    }
    center/=vs;
    for(int i=0; i<vs; i++) {
        v[i]-=center;
    }
    genN();
}

bool check_col(POL p1, POL p2) {
    V2 ax;
    std::vector<V2> t1= p1.v, n1=p1.n,t2=p2.v, n2=p2.n;
    float mtv =2000000000;
    int n=t1.size() , m=t2.size();
    //precalculated normals
    V2 e[n+m];
    //first poly
    for(int i=0; i<n; i++) {
        e[i]=n1[i];
    }
    //second poly
    for(int i=0; i<m; i++) {
        e[i+n]=n2[i];
    }
    V2 voffs = p1.p - p2.p;
    for(int j=0; j<n+m; j++) {
        float min1 = e[j].dot(t1[0]);
        float max1 = min1;
        float min2 = e[j].dot(t2[0]);
        float max2 = min2;
        //first shape
        for (int i = 1; i < n; i++) {
            float p = e[j].dot(t1[i]);
            if (p < min1) {
                min1 = p;
            } else if (p > max1) {
                max1 = p;
            }
        }
        //second shape
        for (int i = 1; i < m; i++) {
            float p = e[j].dot(t2[i]);
            if (p < min2) {
                min2 = p;
            } else if (p > max2) {
                max2 = p;
            }
        }
        float soffset = e[j].dot(voffs);
        min1+=soffset;
        max1+=soffset;
        float mtvl1 = max2 - min1,mtvl2= max1 - min2,mtvl=std::min(mtvl1,mtvl2);
        if(mtvl<0) {
            return false;
        } else if(mtv>mtvl) {
            mtv = mtvl;
            ax = e[j];
            if(soffset<0) {
                ax*=-1;
            }
        }
    }
    return true;
}

bool check_col(POL p1, POL p2, V2 &ax, float &mtv) {
    std::vector<V2> t1= p1.v, n1=p1.n,t2=p2.v, n2=p2.n;
    mtv =2000000000;
    int n=t1.size() , m=t2.size();
    //precalculated normals
    V2 e[n+m];
    //first poly
    for(int i=0; i<n; i++) {
        e[i]=n1[i];
    }
    //second poly
    for(int i=0; i<m; i++) {
        e[i+n]=n2[i];
    }
    V2 voffs = p1.p - p2.p;
    for(int j=0; j<n+m; j++) {
        float min1 = e[j].dot(t1[0]);
        float max1 = min1;
        float min2 = e[j].dot(t2[0]);
        float max2 = min2;
        //first shape
        for (int i = 1; i < n; i++) {
            float p = e[j].dot(t1[i]);
            if (p < min1) {
                min1 = p;
            } else if (p > max1) {
                max1 = p;
            }
        }
        //second shape
        for (int i = 1; i < m; i++) {
            float p = e[j].dot(t2[i]);
            if (p < min2) {
                min2 = p;
            } else if (p > max2) {
                max2 = p;
            }
        }
        float soffset = e[j].dot( voffs);
        min1+=soffset;
        max1+=soffset;
        float mtvl1 = max2 - min1,mtvl2= max1 - min2,mtvl=std::min(mtvl1,mtvl2);
        if(mtvl<0) {
            return false;
        } else if(mtv>mtvl) {
            mtv = mtvl;
            ax = e[j];
            if(soffset<0) {
                ax*=-1;
            }
        }
    }
    return true;
}

//BROKEN
bool check_col(POL p1, POL p2, V2 &ax, float &mtv, V2 &contact) {
    std::vector<V2> t1= p1.v, n1=p1.n,t2=p2.v, n2=p2.n;
    mtv =2000000000;
    int n=t1.size() , m=t2.size();
    //precalculated normals
    V2 e[n+m];
    //first poly
    for(int i=0; i<n; i++) {
        e[i]=n1[i];
    }
    //second poly
    for(int i=0; i<m; i++) {
        e[i+n]=n2[i];
    }
    V2 voffs = p1.p - p2.p;
    for(int j=0; j<n+m; j++) {
        float min1 = e[j].dot(t1[0]);
        float max1 = min1;
        float min2 = e[j].dot(t2[0]);
        float max2 = min2;
        //first shape
        for (int i = 1; i < n; i++) {
            float p = e[j].dot(t1[i]);
            if (p < min1) {
                min1 = p;
            } else if (p > max1) {
                max1 = p;
            }
        }
        //second shape
        for (int i = 1; i < m; i++) {
            float p = e[j].dot(t2[i]);
            if (p < min2) {
                min2 = p;
            } else if (p > max2) {
                max2 = p;
            }
        }
        float soffset = e[j].dot( voffs);
        min1+=soffset;
        max1+=soffset;
        float mtvl1 = max2 - min1,mtvl2= max1 - min2,mtvl=std::min(mtvl1,mtvl2);
        if(mtvl<0) {
            return false;
        } else if(mtv>mtvl) {
            mtv = mtvl;
            ax = e[j];
            if(soffset<0) {
                ax*=-1;
            }
        }
    }
    float max2 = ax.dot(t2[0]);
    float min1 = ax.dot(t1[0]);
    int ii = 0 , jj = 0;
    //contact point
    //first shape
    for (int i = 1; i < m; i++) {
        float p = ax.dot(t2[i]);
        if (p > max2) {
            jj = i;
            max2 = p;
        }
    }
    //second shape
    for (int i = 1; i < n; i++) {
        float p = ax.dot(t1[i]);
        if (p < min1) {
            ii = i;
            min1 = p;
        }
    }
    float slope1, slope2 ;
    int next1=ii+1, next2=ii-1;
    if(ii+1==n) {
        next1 = 0;
    }
    if(ii-1==-1) {
        next2 = n-1;
    }
    //first slope
    V2 edge = t1[next1]-t1[ii];
    edge = edge.norm();
    slope1 = ax.dot(edge);
    //second slope
    edge = t1[next2]-t1[ii];
    edge = edge.norm();
    slope2 = ax.dot(edge);
    //check
    contact = t1[ii]+p1.p;
    if(slope1<=0.00001 || slope2<=0.00001) {
        contact = t2[jj]+p2.p;
    }
    return true;
}
//}
