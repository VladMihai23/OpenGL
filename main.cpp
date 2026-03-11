#include <GLUT/glut.h>
#include <cmath>
#include <vector>

static GLuint gGrassTex = 0;
static GLuint gSkyTex   = 0;
static GLuint gRockTex  = 0;

void setupProjection(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void createGrassTexture() {
    const int w = 256, h = 256;
    std::vector<unsigned char> data(w * h * 3);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = (y * w + x) * 3;

            float a = 0.5f + 0.5f * std::sin(x * 0.18f) * std::cos(y * 0.22f);
            float b = 0.5f + 0.5f * std::sin((x + y) * 0.09f);

            data[i + 0] = (unsigned char)(20 + a * 25);
            data[i + 1] = (unsigned char)(110 + a * 90);
            data[i + 2] = (unsigned char)(20 + b * 20);
        }
    }

    glGenTextures(1, &gGrassTex);
    glBindTexture(GL_TEXTURE_2D, gGrassTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, data.data());
}

void createSkyTexture() {
    const int w = 512, h = 256;
    std::vector<unsigned char> data(w * h * 3);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float t = (float)y / (float)(h - 1);

            unsigned char r = (unsigned char)((1.0f - t) * 175 + t * 110);
            unsigned char g = (unsigned char)((1.0f - t) * 225 + t * 190);
            unsigned char b = (unsigned char)((1.0f - t) * 255 + t * 245);

            float xf = (float)x / (float)(w - 1);
            float cloud = 0.5f + 0.5f * std::sin(10.0f * xf) * std::cos(15.0f * t);

            if (t < 0.35f && cloud > 0.80f) {
                r = 245; g = 245; b = 245;
            }

            int i = (y * w + x) * 3;
            data[i + 0] = r;
            data[i + 1] = g;
            data[i + 2] = b;
        }
    }

    glGenTextures(1, &gSkyTex);
    glBindTexture(GL_TEXTURE_2D, gSkyTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, data.data());
}

void createRockTexture() {
    const int w = 256, h = 256;
    std::vector<unsigned char> data(w * h * 3);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = (y * w + x) * 3;

            float a = 0.5f + 0.5f * std::sin(x * 0.24f) * std::cos(y * 0.19f);
            float b = 0.5f + 0.5f * std::sin((x + y) * 0.11f);
            float mix = 0.65f * a + 0.35f * b;

            unsigned char c = (unsigned char)(120 + mix * 100);
            data[i + 0] = c;
            data[i + 1] = (unsigned char)(c - 8);
            data[i + 2] = (unsigned char)(c - 16);
        }
    }

    glGenTextures(1, &gRockTex);
    glBindTexture(GL_TEXTURE_2D, gRockTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, data.data());
}

void initScene() {
    glClearColor(0.78f, 0.90f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    createGrassTexture();
    createSkyTexture();
    createRockTexture();
}

void drawCubeBounds(float halfSize) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    glutWireCube(halfSize * 2.0f);
    glEnable(GL_TEXTURE_2D);
}

void drawGround(float halfSize) {
    glBindTexture(GL_TEXTURE_2D, gGrassTex);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -5.0f, -halfSize);
        glTexCoord2f(6.0f, 0.0f); glVertex3f( halfSize, -5.0f, -halfSize);
        glTexCoord2f(6.0f, 6.0f); glVertex3f( halfSize, -5.0f,  halfSize);
        glTexCoord2f(0.0f, 6.0f); glVertex3f(-halfSize, -5.0f,  halfSize);
    glEnd();
}

void drawSkyWalls(float halfSize) {
    glBindTexture(GL_TEXTURE_2D, gSkyTex);
    glColor3f(1.0f, 1.0f, 1.0f);

    const float yBottom = -5.0f;
    const float yTop    =  3.0f;
    const float zBack   = -9.8f;
    const float xLeft   = -9.8f;
    const float xRight  =  9.8f;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, yBottom, zBack);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( halfSize, yBottom, zBack);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( halfSize, yTop,    zBack);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, yTop,    zBack);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(xLeft, yBottom,  halfSize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(xLeft, yBottom, -halfSize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(xLeft, yTop,    -halfSize);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(xLeft, yTop,     halfSize);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(xRight, yBottom, -halfSize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(xRight, yBottom,  halfSize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(xRight, yTop,     halfSize);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(xRight, yTop,    -halfSize);
    glEnd();
}

void drawRelief() {
    glBindTexture(GL_TEXTURE_2D, gRockTex);
    glColor3f(1.0f, 1.0f, 1.0f);

    const float baseY  = -5.0f;
    const float size   = 2.0f;
    const float height = 4.5f;
    const float cx     = 1.5f;
    const float cz     = 0.5f;

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - size, baseY, cz - size);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + size, baseY, cz - size);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(cx, baseY + height, cz);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + size, baseY, cz - size);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + size, baseY, cz + size);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(cx, baseY + height, cz);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + size, baseY, cz + size);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - size, baseY, cz + size);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(cx, baseY + height, cz);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - size, baseY, cz + size);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - size, baseY, cz - size);
        glTexCoord2f(0.5f, 0.0f); glVertex3f(cx, baseY + height, cz);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(7.5, 5.0, 15.0,
              0.0, -3.5, 0.0,
              0.0,  1.0, 0.0);

    drawGround(10.0f);
    drawSkyWalls(10.0f);
    drawRelief();
    drawCubeBounds(10.0f);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    setupProjection(w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("P1 - Scena in cub");

    initScene();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}