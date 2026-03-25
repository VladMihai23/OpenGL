#include <GLUT/glut.h>
#include <cmath>
#include <vector>

static GLuint gGrassTex = 0;
static GLuint gSkyTex   = 0;
static GLuint gRockTex  = 0;
static GLuint gRoadTex  = 0;

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

void createRoadTexture() {
    const int w = 256, h = 256;
    std::vector<unsigned char> data(w * h * 3);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int i = (y * w + x) * 3;

            unsigned char r = 65;
            unsigned char g = 65;
            unsigned char b = 70;

            float noise = 0.5f + 0.5f * std::sin(x * 0.31f) * std::cos(y * 0.27f);
            r = (unsigned char)(r + noise * 18);
            g = (unsigned char)(g + noise * 18);
            b = (unsigned char)(b + noise * 18);

            
            if (x > 118 && x < 138 && ((y / 20) % 2 == 0)) {
                r = 235; g = 235; b = 110;
            }

            data[i + 0] = r;
            data[i + 1] = g;
            data[i + 2] = b;
        }
    }

    glGenTextures(1, &gRoadTex);
    glBindTexture(GL_TEXTURE_2D, gRoadTex);
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
    createRoadTexture();
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

void drawRoad() {
    glBindTexture(GL_TEXTURE_2D, gRoadTex);
    glColor3f(1.0f, 1.0f, 1.0f);

    const float y = -4.98f;
    const int segments = 160;
    const float outerA = 7.0f;
    const float outerB = 4.2f;
    const float innerA = 4.8f;
    const float innerB = 2.4f;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float t = 2.0f * 3.1415926f * (float)i / (float)segments;
        float ct = std::cos(t);
        float st = std::sin(t);

        float xOuter = outerA * ct;
        float zOuter = outerB * st;
        float xInner = innerA * ct;
        float zInner = innerB * st;

        float u = 8.0f * (float)i / (float)segments;

        glTexCoord2f(u, 0.0f); glVertex3f(xOuter, y, zOuter);
        glTexCoord2f(u, 1.0f); glVertex3f(xInner, y, zInner);
    }
    glEnd();
}

void drawBuilding(float x, float z, float sx, float sy, float sz) {
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, -5.0f + sy / 2.0f, z);
    glScalef(sx, sy, sz);

    glColor3f(0.75f, 0.55f, 0.40f);
    glutSolidCube(1.0f);

    glTranslatef(0.0f, 0.55f, 0.0f);
    glScalef(1.05f, 0.25f, 1.05f);
    glColor3f(0.55f, 0.15f, 0.15f);
    glutSolidCube(1.0f);

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

void drawTree(float x, float z) {
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(x, -4.2f, z);

    glColor3f(0.45f, 0.25f, 0.10f);
    glPushMatrix();
    glScalef(0.35f, 1.6f, 0.35f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(0.10f, 0.55f, 0.15f);
    glTranslatef(0.0f, 1.2f, 0.0f);
    glutSolidSphere(0.85f, 16, 16);

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

void drawStaticObjects() {
    // 6 cladiri
    drawBuilding(-8.0f, -7.5f, 1.4f, 3.0f, 1.4f);
    drawBuilding(-5.8f, -7.2f, 1.5f, 4.0f, 1.5f);
    drawBuilding(-3.3f, -7.0f, 1.3f, 2.6f, 1.3f);

    drawBuilding( 8.0f,  7.3f, 1.5f, 3.6f, 1.5f);
    drawBuilding( 5.6f,  7.0f, 1.7f, 4.4f, 1.7f);
    drawBuilding( 3.2f,  7.2f, 1.4f, 3.0f, 1.4f);

    // 6 pomi
    drawTree(-8.3f,  5.0f);
    drawTree(-6.5f,  6.8f);
    drawTree(-4.5f,  5.5f);

    drawTree( 8.2f, -5.2f);
    drawTree( 6.3f, -6.7f);
    drawTree( 4.2f, -5.6f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(9.0, 8.0, 17.0,
              0.0, -3.5, 0.0,
              0.0,  1.0, 0.0);

    drawGround(10.0f);
    drawSkyWalls(10.0f);

    // P1
    drawRelief();

    // P2
    drawRoad();
    drawStaticObjects();

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
    glutCreateWindow("P1 + P2 - Scena in cub");

    initScene();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}