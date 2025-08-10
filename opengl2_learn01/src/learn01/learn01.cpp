#include "learn01.hpp"

void message(){
    std::cout << "hello message\n";
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0, 1, 0);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0, 0, 1);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glutSwapBuffers();
}

void helloWorld(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    auto result = glutCreateWindow("FreeGLUT 3.6.0 Hello World");
    std::cout << "result: " << result << "\n";
    glutDisplayFunc(display);
    glutMainLoop();
}