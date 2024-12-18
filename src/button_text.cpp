#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
FT_Face face;

GLuint tex_handle;
int tex_width, tex_height;

int window_width = 640;
int window_height = 480;

int button_x = 10, button_y = 400; // Координаты кнопки (левый верхний угол)
int button_width = 240, button_height = 80; // Размер кнопки
int button_pressed = 0; // Флаг нажатия кнопки

// Инициализация FreeType и загрузка шрифта
bool initFreeType(const char* font_path) {
    if (FT_Init_FreeType(&library)) {
        printf("Couldn't init FreeType2\n");
        return false;
    }
    if (FT_New_Face(library, font_path, 0, &face)) {
        printf("Couldn't open font file %s\n", font_path);
        return false;
    }
    return true;
}

// Получение битмапа символа и загрузка его в текстуру
bool createCharacterTexture(char ch, float font_size) {
    FT_Set_Pixel_Sizes(face, 0, (int)font_size);

    FT_UInt glyph_index = FT_Get_Char_Index(face, ch);
    if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER)) {
        printf("Couldn't load glyph for character %c\n", ch);
        return false;
    }

    FT_Bitmap* bitmap = &face->glyph->bitmap;

    tex_width = bitmap->width;
    tex_height = bitmap->rows;

    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tex_width, tex_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap->buffer);

    return true;
}

// Отображение символа с учётом смещения
void renderCharacter(char ch, float font_size, int x, int y) {
    if (!createCharacterTexture(ch, font_size)) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_handle);

    glBegin(GL_QUADS);
        // Инвертируем координаты по Y (по оси Y текстуры)
        glTexCoord2f(0, 1); glVertex2i(x, y); // Нижний левый
        glTexCoord2f(1, 1); glVertex2i(x + tex_width, y); // Нижний правый
        glTexCoord2f(1, 0); glVertex2i(x + tex_width, y + tex_height); // Верхний правый
        glTexCoord2f(0, 0); glVertex2i(x, y + tex_height); // Верхний левый
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(1, &tex_handle);
}

// Рисование текста
void renderText(const char* text, float font_size, int x, int y) {
    for (int i = 0; text[i] != '\0'; i++) {
        renderCharacter(text[i], font_size, x, y);
        x += tex_width; // Смещение вправо для следующего символа
    }
}

// Проверка нажатия кнопки
int isInsideButton(int x, int y) {
    return x >= button_x && x <= (button_x + button_width) &&
           y >= (window_height - button_y - button_height) && y <= (window_height - button_y); 
}

// Отображение кнопки
void renderButton() {
    // Определяем цвет кнопки в зависимости от состояния (нажата или нет)
    if (button_pressed)
        glColor3f(0.6f, 0.0f, 0.0f); // Цвет кнопки при нажатии (тёмно-красный)
    else
        glColor3f(0.8f, 0.2f, 0.2f); // Обычный цвет кнопки (светло-красный)

    glBegin(GL_QUADS);
        glVertex2i(button_x, button_y);
        glVertex2i(button_x + button_width, button_y);
        glVertex2i(button_x + button_width, button_y - button_height);
        glVertex2i(button_x, button_y - button_height);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // Цвет текста (белый)
    renderText("Hello World", 32, button_x + 20, button_y - 50);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderButton();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

// Обработка событий мыши
void mouse(int button, int state, int x, int y) {
    // Инвертируем координату Y
    y = window_height - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isInsideButton(x, y)) {
            std::cout << "button down" << std::endl;
            button_pressed = 1; // При клике на кнопку, меняем флаг нажатия
            glutPostRedisplay(); // Перерисовываем экран
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (button_pressed) {
            std::cout << "button up" << std::endl;
            button_pressed = 0; // При отпускании кнопки, сбрасываем флаг
            glutPostRedisplay(); // Перерисовываем экран
        }
    }
}

int main(int argc, char** argv) {
    if (!initFreeType("roboto.ttf")) {
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Button with Text");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    glutMainLoop();

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;
}