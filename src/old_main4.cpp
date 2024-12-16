#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




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



//added
float buttonX1 = -0.2f, buttonX2 = 0.2f;
float buttonY1 = -0.1f, buttonY2 = 0.1f;

// Флаг состояния кнопки
bool buttonPressed = false;

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



//void drawCube(float x, float y, float z) {
//    glPushMatrix();
//    glTranslatef(x, y, z);
//    glutSolidCube(5);
//    glPopMatrix();

//}




void cylinder(double pos)
{
    const double PI = 3.14159;

    /* top triangle */
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
        /* close the loop back to 0 degrees */
        //uncomment
	glTexCoord2f( 0.5, 0.5 );
        glVertex3f(radius, height, 0);
    glEnd();

    /* bottom triangle: note: for is in reverse order */
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f( 0.5, 0.5 );
        glVertex3f(0, 0, 0);  /* center */
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
        }
    glEnd();

    /* middle tube */
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 2 * PI; i += resolution)
        {
            const float tc = ( i / (float)( 2 * PI ) );
	    glTexCoord2f( tc, 0.0 );
            glVertex3f(radius * cos(i), 0, radius * sin(i));
            glTexCoord2f( tc, 1.0 );
            glVertex3f(radius * cos(i), height, radius * sin(i));
        }
        /* close the loop back to zero degrees */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f(radius, 0, 0);
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f(radius, height, 0);

        glutSolidCube(5);

    glEnd();

    glPopMatrix();
}

GLuint tex;
void init()
{
    //unsigned char data[] =
    //{
    //    128, 128, 128, 255,
    //    255, 0, 0, 255,
    //    0, 255, 0, 255,
    //    0, 0, 255, 255,
    //};

    //unsigned char data[] =
    //{
    //    0, 0, 128, 255,
    //    255, 0, 128, 255,
    //    0, 255, 128, 0,
    //    255, 0, 255, 255,
    //};

    //added this
    int width, height, cnt;
    unsigned char *data = stbi_load("01.png", &width, &height, &cnt, 0);
    

    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );
    // instead GL_REPEAT was GL_CLAMP_TO_EDGE 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //glTexImage2D( GL_TEXTURE_2D, 0,GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data );

    stbi_image_free(data);

}

float angle = 0;
float delta = 3;
void timer( int value )
{
    /*
    if (value == 1) {
        delta = 3;
        std::cout << value << std::endl;
    }
    if (value == 2) {
        delta = 6;
        std::cout << value << std::endl;
    }
    if (value == 3) {
        delta = 12;
        std::cout << value << std::endl;
    }
    */

    angle += delta;
        
    glutPostRedisplay();
    glutTimerFunc( 16, timer, 0 );

    //glutTimerFunc( 10, timer, 0);


}

void myMouseFunc(int button, int state, int x, int y)
{
    Point pC = {x, y};	

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		p1.x = x;
		p1.y = 480 - y;
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		p2.x = x;
		p2.y = 480 - y;

        //angle = 0;
        delta = 0;
        pC.x = x;
        pC.y = p2.y;
		std::cout << p2.x << " " << p2.y << std::endl;
        //draw_ellipse(pC, 60.0f, 200.0f);
	}

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        delta += 3;
    }

}


void display_button() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawButton();
    glutSwapBuffers();
}



void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //uncomment
    //gluPerspective( 60, 1.0, 0.1, 100.0 );
    gluPerspective(60, 1.0, 0.1, 100.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0, 0, -5 );

    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );

    //uncomment
    //glRotatef( angle, 0.2, 0.3, 0.1 );
    
    //delete
    int angleX = 45.0f;
    int angleY = 30.0f;
    int angleZ = 15.0f;
    //glRotatef( 90, 1, 0, 0 );
    glTranslatef( 0, 0, -1/2 );
    glRotatef(-90, 0.0f, 0.0f, 1.0f);
    // uncomment this
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    
    //glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    //glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    //glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

/*
    if (delta == 3) {
        glRotatef(0, 0.0f, 1.0f, 0.0f);
    
    }
    if (delta == 6) {
        glRotatef(30, 0.0f, 1.0f, 0.0f);
    
    }
    if (delta == 12) {
        glRotatef(-angle, 0.0f, 1.0f, 0.0f);
    
    }
*/
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, tex );
    //added

    
    cylinder(0.3333);
    cylinder(0.6666);
    cylinder(1);
//    drawCube(1.0f, 0.0f, 0.0f);
    //drawButton();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 600, 600 );
    glutCreateWindow( "Octavian" );
    init();
    glutDisplayFunc( display );
    
    
    glutMouseFunc(myMouseFunc);
 //   glutTimerFunc( 0, timer, 0 );
    glutTimerFunc( 10, timer, 0);
//    glutTimerFunc( 10, timer, 1 );
//    glutTimerFunc( 10, timer, 2 );
    glutMainLoop();
    return 0;
}


