#include "../include/model.h"
#include <GL/gl.h>
#include <string.h>

using namespace std;

void model::draw() {
    int f, v, texturacurenta = -1;
    float clr[4];
    material mat;
    glm::vec2 tex;

    for(f = 0; f < (signed)faces.size(); f++) {
        glm::vec3 vec = faces[f].normal;

        glNormal3f(vec.x, vec.y, vec.z);

        if(faces[f].mat != -1) {
            mat = materials[faces[f].mat];
            memcpy(clr, mat.diff, sizeof(clr));
            glColor4fv(clr);

            if(mat.texture != -1 and mat.texture != texturacurenta) {
                texturacurenta = mat.texture;
                glBindTexture(GL_TEXTURE_2D, texturacurenta);
            }
        } else
            texturacurenta = -1;

        glBegin(faces[f].np == 3 ? GL_TRIANGLES : GL_QUADS);

        for(v = 0; v < faces[f].np; v++) {
            vec = faces[f].vertices[v];

            if(texturacurenta != -1) {
                tex = faces[f].texcoords[v];
                glTexCoord2f(tex.x, tex.y);
            }

            glVertex3f(vec.x, vec.y, vec.z);
        }

        glEnd();
    }
}
