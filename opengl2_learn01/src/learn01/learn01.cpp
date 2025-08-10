#include "learn01.hpp"

void message(){
    std::cout << "hello message\n";
}

void helloWorld(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    auto result = glutCreateWindow("FreeGLUT 3.6.0 Hello World");
    std::cout << "result: " << result << "\n";
    glutDisplayFunc([]() -> void {
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
    });
    glutMainLoop();
}

void helloWorld1(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    auto result = glutCreateWindow("Hello World");
    std::cout << "result: " << result << "\n";
    glutDisplayFunc([]() -> void {
        std::cout << "callback..." << "\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
    });
    glClearColor(0.60F, 0.40F, 0.70F, 1.0F);
    glutMainLoop();
}
void helloWorld2(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    auto result = glutCreateWindow("GLRect");
    std::cout << "result: " << result << "\n";
    glutDisplayFunc([]() -> void {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0F, 0.0F, 0.0F);
        glRectf(-25.0F, 25.0F, 25.0F, -25.0F);
        glutSwapBuffers();
    }); 
    glutReshapeFunc([](GLsizei width, GLsizei height) -> void {
        GLfloat aspectRadio;
        std::cout << std::format("width: {}, height: {}", width, height) << "\n";
        if(height == 0){
            height = 1;
        }
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        aspectRadio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
        if(width <= height){
            glOrtho(-100.0, 100.0, -100.0 / aspectRadio, 100.0 / aspectRadio, 1.0, -1.0);
        } else {
            glOrtho(-100.0 * aspectRadio, 100.0 * aspectRadio, -100.0, 100.0, 1.0, -1.0);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    });
    glClearColor(0.0F, 0.0F, 1.0F, 1.0F);
    glutMainLoop();
}
// ============== helloWorld3 ===========
// 方块的初始位置和大小
GLfloat x1 = 0.0F, y1 = 0.0F, rsize = 25.0F;

// 步进大小
GLfloat xStep = 1.0F, yStep = 1.0F;

// 窗口宽度和高度
GLfloat windowWidth = 0.0F, windowHeight = 0.0F;

void helloWorld3(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    auto result = glutCreateWindow("Bounce");
    std::cout << "result: " << result << "\n";
    glutDisplayFunc([]() -> void {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0F, 0.0F, 0.0F);
        glRectf(x1, y1, x1 + rsize, y1 - rsize);
        glutSwapBuffers();
    }); 
    glutReshapeFunc([](GLsizei width, GLsizei height) -> void {
        GLfloat aspectRadio;
        std::cout << std::format("width: {}, height: {}", width, height) << "\n";
        if(height == 0){
            height = 1;
        }
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        aspectRadio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
        if(width <= height){
            windowWidth = 100;
            windowHeight = 100 / aspectRadio;
            glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
        } else {
            windowWidth = 100 * aspectRadio;
            windowHeight = 100;
            glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 更新全局变量，注意这里和正交投影范围对应
        // windowWidth = (width <= height) ? 100.0f : 100.0f * aspectRadio;
        // windowHeight = (width <= height) ? 100.0f / aspectRadio : 100.0f;
    });
    void timerFunction(int value);
    glutTimerFunc(33, timerFunction, 1);
    glClearColor(0.0F, 0.0F, 1.0F, 1.0F);
    glutMainLoop();
}
// https://github.com/pseudaria/OpenGLES2.0_OBJ

void timerFunction(int value) {
    if (x1 > windowWidth - rsize || x1 < -windowWidth) {
        xStep = -xStep;
    }
    if (y1 > windowHeight || y1 < -windowHeight + rsize) {
        yStep = -yStep;
    }
    x1 += xStep;
    y1 += yStep;
    if (x1 > (windowWidth - rsize + xStep)) {
        x1 = windowWidth - rsize - 1;
    } else if (x1 < -(windowWidth + xStep)) {
        x1 = -windowWidth - 1;
    }
    if (y1 > (windowHeight + yStep)) {
        y1 = windowHeight - 1;
    } else if (y1 < -(windowHeight - rsize + yStep)) {
        y1 = -windowHeight + rsize - 1;
    }
    glutPostRedisplay();
    glutTimerFunc(33, timerFunction, 1);
};