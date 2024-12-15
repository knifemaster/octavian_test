
#include <GL/glut.h>
#include <stdbool.h>

// Координаты кнопки
float buttonX1 = -0.2f, buttonX2 = 0.2f;
float buttonY1 = -0.1f, buttonY2 = 0.1f;

// Флаг состояния кнопки
bool buttonPressed = false;

// Рисование кнопки
void drawButton() {
    // Цвет кнопки зависит от ее состояния
    if (buttonPressed) {
        glColor3f(0.0f, 1.0f, 0.0f); // Зеленая кнопка
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // Красная кнопка
    }

    // Рисуем прямоугольник (кнопку)
    glBegin(GL_QUADS);
        glVertex2f(buttonX1, buttonY1);
        glVertex2f(buttonX2, buttonY1);
        glVertex2f(buttonX2, buttonY2);
        glVertex2f(buttonX1, buttonY2);
    glEnd();
}

// Функция для проверки клика по кнопке
bool isInsideButton(float x, float y) {
    return x >= buttonX1 && x <= buttonX2 && y >= buttonY1 && y <= buttonY2;
}

// Обработчик событий мыши
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Преобразуем координаты окна в координаты OpenGL
        float glX = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0f - 1.0f;
        float glY = 1.0f - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0f;

        // Проверяем, попал ли клик по кнопке
        if (isInsideButton(glX, glY)) {
            buttonPressed = !buttonPressed; // Изменяем состояние кнопки
            glutPostRedisplay();           // Обновляем экран
        }
    }
}

// Отображение сцены
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawButton();
    glutSwapBuffers();
}

// Инициализация OpenGL
void initOpenGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Белый фон
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Устанавливаем ортографическую проекцию
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Button Example");

    initOpenGL();

    glutDisplayFunc(display);
    glutMouseFunc(mouseClick); // Регистрируем обработчик мыши

    glutMainLoop();
    return 0;
}

