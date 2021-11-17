
#include "ClipSegments.h"
#include <iostream>
#include <vector>
#include <gl\glut.h>

using namespace std;

#define WinW 830
#define WinH 600
#define STEP 70

int sign(double a) {
    if (a > 0) return 1;
    if (a < 0) return -1;
    return 0;
}

void digitalToDecard(int x, int y, int &X, int &Y) {
    if (x > STEP / 2) X = x - STEP / 2;
    else
        X = -(STEP / 2 - x);
    if (y > STEP / 2) Y = -(y - STEP / 2);
    else
        Y = STEP / 2 - y;
}

void drawPixel(int x, int y, int Red, int Green, int Blue) {
    int X, Y;
    digitalToDecard(x, y, X, Y);
    double sizeX = (float) WinW / (float) STEP;
    double sizeY = (float) WinH / (float) STEP;
    glColor3d(Red, Green, Blue);
    glBegin(GL_QUADS);

    double _x = X * sizeX;
    double _y = Y * sizeY;

    glVertex2d(_x, _y);
    glVertex2d(_x + sizeX, _y);
    glVertex2d(_x + sizeX, _y + sizeY);
    glVertex2d(_x, _y + sizeY);
    glEnd();
}

void decardToDigital(int x, int y, int &X, int &Y) {
    X = (STEP / 2 + x);
    Y = (STEP / 2 - y);
}

void drawBrezehamLine(int x1, int x2, int y1, int y2, vector<vector<int>> &field, int color) {
    int x, y, dx, dy, sx, sy;
    int X, Y;
    x = x1, y = y1;
    dx = abs(x2 - x1), dy = abs(y2 - y1);
    sx = sign(x2 - x1), sy = sign(y2 - y1);
    bool swap = false;

    if (dx < dy) {
        std::swap(dx, dy);
        swap = true;
    }
    int eps = 2 * dy - dx;
    if (sx < 0) x += sx;
    if (sy < 0) y += sy;
    for (int i = 0; i <= dx; i++) {
        decardToDigital(x, y, X, Y);
        field[X][Y] = color;
        while (eps >= 0) {
            if (swap)
                x += sx;
            else
                y += sy;
            eps -= (2 * dx);
        }
        if (swap)
            y += sy;
        else
            x += sx;
        eps += (2 * dy);
    }
}

void fillSegments(vector<vector<int>> &field) {
    int point;
    bool f;
    for (int i = 0; i < STEP; ++i) {
        f = false;
        for (int j = 0; j < STEP; ++j) {
            if (field[i][j] == 1) {
                if (field[i][j - 1] == 1) {
                    point = j + 1;
                } else {
                    if (f) {
                        if (field[i - 1][point] != 0) {
                            for (int k = point; k < j; ++k) {
                                if (field[i][k] == 0) field[i][k] = 5;
                                if (field[i][k] == 2) field[i][k] = 3;
                            }
                        } else {
                            point = j + 1;
                        }
                    } else {
                        point = j + 1;
                        f = !f;
                    }
                }
            }

        }
    }
}

void drawField(vector<vector<int>> &field) {

    for (int i = 0; i <= STEP; ++i) {
        for (int j = 0; j <= STEP; ++j) {
            if (field[i][j] == 1) {
                drawPixel(i, j, 0, 0, 0);
            }
            if (field[i][j] == 2) {
                drawPixel(i, j, 1, 1, 0);
            }
            if (field[i][j] == 3) {
                drawPixel(i, j, 0, 1, 0);
            }
        }
    }
}

void drawPolygon(vector<vector<int>> &coords, vector<vector<int>> &field) {
    for (int i = 0; i < coords.size() - 1; ++i) {
        drawBrezehamLine(coords[i][0], coords[i + 1][0], coords[i][1], coords[i + 1][1], field, 1);
    }
    drawBrezehamLine(coords[0][0], coords[coords.size() - 1][0], coords[0][1], coords[coords.size() - 1][1], field, 1);
}

void drawSegments(vector<vector<int>> &coords, vector<vector<int>> &field) {
    for (int i = 0; i < coords.size() - 1; i+=2) {
        drawBrezehamLine(coords[i][0], coords[i + 1][0], coords[i][1], coords[i + 1][1], field, 2);
    }
}

void drawGrid() {
    glLineWidth(1);
    glColor3d(0, 0, 0);
    for (float i = -STEP; i < STEP; ++i) {
        glBegin(GL_LINES);
        glVertex2d(i * WinW / (float) STEP, -WinH);
        glVertex2d(i * WinW / (float) STEP, WinH);
        glEnd();
    }
    for (float i = -STEP; i < STEP; ++i) {
        glBegin(GL_LINES);
        glVertex2d(-WinW, i * WinH / (float) STEP);
        glVertex2d(WinW, i * WinH / (float) STEP);
        glEnd();
    }

    glLineWidth(3);
    glColor3d(256, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-WinW, 0);
    glVertex2f(WinW, 0);
    glVertex2f(0, WinH);
    glVertex2f(0, -WinH);
    glEnd();
}

vector<vector<int>> field;

void display() {

    static int start = 1;
    if (start == 1) {
        start = 0;
        field.resize(STEP + 1);
        for (int i = 0; i < STEP + 1; ++i) {
            field[i].resize(STEP + 1);
        }
    }
    vector<vector<int>> poly{{-7, 20},
                             {8,   12},
                             {9,   -3},
                             {-16, -16},
                             {-19, -2}};
    vector<vector<int>> segments{{13,  14},
                                 {13,  -14},
                                 {2,   4},
                                 {6,   4},
                                 {-17, 7},
                                 {4,   20}};
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    drawSegments(segments, field);
    drawPolygon(poly, field);
    fillSegments(field);
    drawField(field);
    drawGrid();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(GLsizei Winw, GLsizei Winh) {
    glViewport(0, 0, Winw, Winh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Winw / 2, Winw / 2, -Winh / 2, Winh / 2, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

