#ifndef MODEL
#define MODEL

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "SFML/Graphics.hpp"

using namespace std;

struct material
{
    string name;
    float diff[4], amb[4], spec[4], emm[4], shiny;
    float alpha;
    sf::Texture texture;
};

struct face
{
    int np, mat;
    /*normals:normalele punctelor
      normal: normala fetei*/
    glm::vec3 vertices[4], normal;
    glm::vec2 texcoords[4];
    face(){}
    face(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    face(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
        vertices[3] = d;
    }
};

struct vertex
{
    vector<int> idfaces;//fetele din care face parte
    glm::vec3 normal;
    vertex() { normal = glm::vec3(0); }
};

struct model
{
    vector<face> faces;
    vector<vertex> vertices;
    vector<material> materials;
    void draw();
};

#endif
