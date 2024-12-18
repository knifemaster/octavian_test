#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <StateMachine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
FT_Face face;

GLuint tex_handle;
int tex_width, tex_height;

int window_width = 600;
int window_height = 600;

int button_x = 10, button_y = 400;
int button_width = 240, button_height = 80;
bool button_pressed = false;
bool stop_signal = false;


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
        glColor3f(0.6f, 0.0f, 0.0f);
    else
        glColor3f(0.8f, 0.2f, 0.2f);

    glBegin(GL_QUADS);
        glVertex2i(button_x, button_y);
        glVertex2i(button_x + button_width, button_y);
        glVertex2i(button_x + button_width, button_y - button_height);
        glVertex2i(button_x, button_y - button_height);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    renderText("Start", 32, button_x + 20, button_y - 50);
    glutPostRedisplay();
    
}


sm::StateMachine game_state;

sm::StateButton button_state;

float angle = 0;

struct Point {
	GLint x;
	GLint y;
};

Point p1, p2;

struct GLColor {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};

GLColor colors[6] = {
	{0.0f, 0.0f, 0.0f},	// Black
	{1.0f, 0.0f, 0.0f},	// Red
	{0.0f, 1.0f, 0.0f}, // Green
	{0.0f, 0.0f, 1.0f}, // Blue
	{1.0f, 1.0f, 0.0f}, // Yellow
	{1.0f, 0.0f, 1.0f}	// Purple
};

GLColor color = colors[0]; // Default: Black


// Функция для проверки угла
// в зависимости какое положение принимает барабан
bool isAngleInRange(double angle, double rangeStart, double rangeEnd) {
    double m_angle = fmod(angle, 360);
    if (m_angle < 0) {
        m_angle += 360.0;
    }

    rangeStart = fmod(rangeStart, 360.0);

    if (rangeStart < 0) {
        rangeStart += 360.0;
    }

    rangeEnd = fmod(rangeEnd, 360.0);
    if (rangeEnd < 0) {
        rangeEnd += 360.0;
    }

    if  (rangeStart <= rangeEnd) {
        return m_angle >= rangeStart && m_angle <= rangeEnd;
    }
    else {
        return m_angle >= rangeStart || m_angle <= rangeEnd;

    }


}

void cylinder(double pos)
{
    const double PI = 3.14159;

    // top triangle 
    double i, resolution  = 0.1;
    double height = 0.5;
    double radius = 0.5;

    glPushMatrix();
    glTranslatef(0, pos, 0);

    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, height, 0);  /* center */
        for (i = 2 * PI; i >= 0; i -= resolution)

        {
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(radius * cos(i), height, radius * sin(i));
        }

	glTexCoord2f( 0.5, 0.5 );
        glVertex3f(radius, height, 0);
    glEnd();

    // bottom triangle 
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, 0, 0);  /* center */
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
        }
    glEnd();

    // middle tube 
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            const float tc = ( i / (float)( 2 * PI ) );
	    glTexCoord2f( tc, 0.0 );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
            glTexCoord2f( tc, 1.0 );
            glVertex3f(radius * cos(i), height, radius * sin(i));
        }
        
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f(radius, 0, 0);
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f(radius, height, 0);

    glEnd();

    glPopMatrix();
}

GLuint tex;
void init()
{
    game_state.set_state(sm::StateMachine::State::SPIN);

    int width, height, channel;
    unsigned char *data = stbi_load("01.png", &width, &height, &channel, 0);
    

    glGenTextures( 3, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    // instead GL_REPEAT write GL_CLAMP_TO_EDGE 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data );

    stbi_image_free(data);

}

float delta = 3;
void update( int value )
{

   if (game_state.is_spin()) {
        delta += 3;
        std::cout << "Spin State" << std::endl;

   }

   if (game_state.is_idle()) {
        std::cout << "Idle State" << std::endl;
        
        delta -= 3;

        if (delta < 0) {
            delta = 0;
        }

   }

    angle += delta;

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);

}

void MouseEvent(int button, int state, int x, int y)
{
    Point pC = {x, y};	

    using namespace std::chrono_literals;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    
            // если наш угол между 90 и 110 градусов
            // это нужно чтобы знать когда останавливать
            // барабан
            if (isAngleInRange(angle, 90, 110)) {
                stop_signal = true;
                //delta -= 3;
                delta = 0;
            }

            std::cout << "MouseEvent" << angle << std::endl;
   
        // нажатие кнопки мыши
    }
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        button_state.set_state(sm::StateButton::State::PRESSED);

        if (button_state.is_pressed()) {
            std::cout << "Кнопка нажата" << std::endl;

            //game_state.set_state(sm::StateMachine::State::IDLE);

        }
        // отжатие кнопки мыши
	}

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        // правая кнопка мыши нажатие
        // delta += 3;
    }

}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

void display_button() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderButton();

    glutSwapBuffers();

    
}

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective(60, 1.0, 1.0, 200.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0, 0, -5 );

    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );


    glTranslatef( 0, 0, -1/2 );
    glRotatef(-90, 0.0f, 0.0f, 1.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);


    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, tex );


    cylinder(0.3333);
    cylinder(0.6666);
    cylinder(1);
 
    //renderButton();
    
    glutSwapBuffers();


}


void subMenuHandler(int choice) {
	color = colors[choice];

}

void mainMenuHandler(int choice) {
    
	Point p = {320, 240};	// draw_pixel
	Point p1 = {10, 100};	// draw_line
	Point p2 = {200, 100};	// --

	Point pC = {320, 240};	// Circle center point
	GLfloat radius = 200;	// Circle radius

    	std::cout << "choice" << std::endl;

	switch(choice) {
		case 1:	// Start
			std::cout << "1" << std::endl;
            game_state.set_state(sm::StateMachine::State::SPIN);
			//delta += 3;
			break;

		case 2:	// Stop
            std::cout << "2" << std::endl;
			game_state.set_state(sm::StateMachine::State::IDLE);
            //delta -= 3;
			break;

		case 3:	// Exit
			exit(0);
			break;
	}

}



int main(int argc, char **argv)
{
    if (!initFreeType("roboto.ttf")) {
        return 1;
    }

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 600, 600 );
    glutCreateWindow( "Octavian" );
    init();
    
   
    
    glutDisplayFunc( display );
   
        
    int subMenu = glutCreateMenu(subMenuHandler);
    glutAddMenuEntry("status", 0);

    glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Status", subMenu);

    glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("Stop", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutMouseFunc(MouseEvent);
    glutTimerFunc( 10, update, 0);

    glutMainLoop();
    return 0;
}


