#include <GL\glut.h>
#include "ClipSegments.h"

#define WinW 830
#define WinH 600

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(WinW, WinH);
    glutInitWindowPosition(300, 80);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Clipping segments");
    glClearColor(255, 255, 255, 255);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}