#include <glm/glm.hpp>
//#include "texture.h"
#include <GL/gl.h>
#include <string.h>
#include <fstream>
#include <cstdlib>

#include "../include/modelobj.h"

using namespace std;

vector<glm::vec3> normals, vertices;
vector<glm::vec2> texcoords;
model obj;

void matload(const char* path) {
    ifstream fi(path);

    if(!fi) {
        printf("Cannot open %s\n", path);
        exit(1);
    }

    char matname[100], filename[100];
    float diff[4], amb[4], spec[4], emm[4], alpha;
    bool ismat = false;
    string line;
    material material;

    strcpy(filename, "notexture");

    while(getline(fi, line)) {
        switch(line[0]) {
        case '#':
            continue;
        case 'n':
            if(ismat) {
                if(strcmp(filename, "notexture")) {
                    material.texture = Texture::loadFromFile(filename);
                    strcpy(filename, "notexture");
                } else
                    material.texture = -1;

                material.alpha = alpha;
                material.name = matname;
                memcpy(material.amb, amb, sizeof(amb));
                memcpy(material.diff, diff, sizeof(diff));
                memcpy(material.spec, spec, sizeof(spec));
                memcpy(material.emm, emm, sizeof(emm));

                material.diff[3] = alpha;

                obj.materials.push_back(material);
            }

            ismat = false;
            sscanf(line.c_str(), "newmtl %s", matname);
            continue;
        case 'K':
            switch(line[1]) {
            case 'a':
                sscanf(line.c_str(), "Ka %f %f %f", amb, amb+1, amb+2);
                amb[3] = 1;
                break;
            case 'd':
                sscanf(line.c_str(), "Kd %f %f %f", diff, diff+1, diff+2);
                diff[3] = 1;
                break;
            case 's':
                sscanf(line.c_str(), "Ks %f %f %f", spec, spec+1, spec+2);
                spec[3] = 1;
                break;
            }
            ismat = true;
            continue;
        case 'm':
            sscanf(line.c_str(), "map_Kd %s", filename);
            ismat = true;
            continue;
        case 'd':
            sscanf(line.c_str(), "d %f", &alpha);
            ismat = true;
            continue;
        }
    }

    if(ismat) {
        if(strcmp(filename, "notexture")) {
            material.texture = loadtexture(filename);
            strcpy(filename, "notexture");
        } else
            material.texture = -1;

        material.alpha = alpha;
        material.name = matname;
        memcpy(material.amb, amb, sizeof(amb));
        memcpy(material.diff, diff, sizeof(diff));
        memcpy(material.spec, spec, sizeof(spec));
        memcpy(material.emm, emm, sizeof(emm));

        material.diff[3] = alpha;

        obj.materials.push_back(material);
    }
}

void cleanup() {
    vertices.clear();
    normals.clear();
    texcoords.clear();
    obj.materials.clear();
    obj.faces.clear();
}

void comun(const char* path) {
    ifstream fi(path);
    if(!fi) {
        printf("Cannot open %s\n", path);
        exit(1);
    }
    string line;

    normals.clear();
    vertices.clear();
    texcoords.clear();

    normals.push_back(glm::vec3(0));
    vertices.push_back(glm::vec3(0));
    texcoords.push_back(glm::vec2(0));

    int crmat = -1, i;

    while(getline(fi, line)) {
        if(line[0] == '#') continue;

        if(line[0] == 'v') {
            float x, y, z;

            switch(line[1]) {
            case ' ':
                sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                vertices.push_back(glm::vec3(x, y, z));
                break;
            case 'n':
                sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                normals.push_back(glm::vec3(x, y, z));
                break;
            case 't':
                sscanf(line.c_str(), "vt %f %f", &x, &y);
                texcoords.push_back(glm::vec2(x, 1-y));
                break;
            }

            continue;
        }

        if(line[0] == 'f') {
            int nrv = count(line.begin(), line.end(), ' ');
            int idv, idt, idn;

            face face;

            face.np = nrv;

            const char *ptr = line.c_str() + 2;

            int found = 1, last = 2;

            if(line.find("//") != string::npos) {
                for(i = 0; i < nrv; i++) {
                    sscanf(ptr, "%d//%d", &idv, &idn);
                    found = line.find(" ", found + 1);
                    if(found != -1) {
                        ptr += found - last;
                        last = found;
                    }

                    face.vertices[i] = vertices[idv];
                }

                face.normal = normals[idn];
            } else {
                int nrs = count(line.begin(), line.end(), '/');

                if(nrs == nrv * 2) {
                    for(i = 0; i < nrv; i++) {
                        sscanf(ptr, "%d/%d/%d", &idv, &idt, &idn);

                        found = line.find(" ", found + 1);
                        if(found != -1) {
                            ptr += found - last;
                            last = found;
                        }
                        face.vertices[i] = vertices[idv];
                        face.texcoords[i] = texcoords[idt];
                    }

                    face.normal = normals[idn];
                } else if(nrs == nrv) {
                    for(i = 0; i < nrv; i++) {
                        sscanf(ptr, "%d/%d", &idv, &idt);

                        found = line.find(" ", found + 1);

                        if(found != -1) {
                            ptr += found - last;
                            last = found;
                        }

                        face.vertices[i] = vertices[idv];
                        face.texcoords[i] = texcoords[idt];
                    }
                } else if(nrs == 0) {
                    for(i = 0; i < nrv; i++) {
                        sscanf(ptr, "%d", &idv);
                        found = line.find(" ", found + 1);

                        if(found != -1) {
                            ptr += found - last;
                            last = found;
                        }

                        face.vertices[i] = vertices[idv];
                    }
                }
            }

            face.mat = crmat;

            face.normal.norm();

            obj.faces.push_back(face);

            continue;
        }

        if(line.substr(0, 6) == "usemtl") {
            char t[100];
            sscanf(line.c_str(), "usemtl %s", t);

            for(i = 0; i < (signed)obj.materials.size(); i++)
                if(obj.materials[i].name == t) {
                    crmat = i;
                    break;
                }

            continue;
        }

        if(line.substr(0, 6) == "mtllib") {
            char matname[100];
            sscanf(line.c_str(), "mtllib %s", matname);
            matload(matname);
        }
    }
}

int loadobj(const char* path) {
    comun(path);

    int ret = glGenLists(1);

    glNewList(ret, GL_COMPILE);
    obj.draw();
    glEndList();

    cleanup();

    return ret;
}

void loadobj(const char* path, model& model) {
    comun(path);
    model.faces = obj.faces;
    model.materials = obj.materials;
    cleanup();
}
