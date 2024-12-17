#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#include <ft2build.h>
#include FT_FREETYPE_H

FT_Bitmap bitmap;
GLuint tex_handle;
int tex_width, tex_height;

bool get_character_bitmap(char ch, float font_size, FT_Bitmap* bitmap) {
    FT_Error error;
    
    FT_Library library;
    error = FT_Init_FreeType(&library);
    if (error) {
        printf("Couldn't init FreeType2\n");
        return false;
    }

    FT_Face face;
    error = FT_New_Face(library, "roboto.ttf", 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        printf("Couldn't open font file, unknown file format.\n");
        return false;
    } else if (error) {
        printf("Couldn't open font file.\n");
        return false;
    }

    error = FT_Set_Char_Size(face, 0, (int)(font_size * 16), 100, 100);
    if (error) {
        printf("Couldn't set font size.\n");
        return false;
    }

    FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_UInt32)ch);

    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        printf("Couldn't load glyph\n");
        return false;
    }

    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (error) {
        printf("Couldn't render glyph\n");
        return false;
    }

    *bitmap = face->glyph->bitmap;
    return true;
}

void initTexture(char character) {
    
    if (!get_character_bitmap(character, 100, &bitmap)) {
        printf("Rendering character failed\n");
        exit(1);
    }

    tex_width = bitmap.width;
    tex_height = bitmap.rows;

    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tex_width, tex_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap.buffer);
}


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

    glEnd();

    //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//remove this
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 640, 0, 480);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Рендер текстуры
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_handle);

        glBegin(GL_QUADS);
            glTexCoord2d(0, 1); glVertex2i((640 - tex_width) / 2, (480 - tex_height) / 2);
            glTexCoord2d(1, 1); glVertex2i((640 + tex_width) / 2, (480 - tex_height) / 2);
            glTexCoord2d(1, 0); glVertex2i((640 + tex_width) / 2, (480 + tex_height) / 2);
            glTexCoord2d(0, 0); glVertex2i((640 - tex_width) / 2, (480 + tex_height) / 2);
        glEnd();

        glDisable(GL_TEXTURE_2D);
///


     glPopMatrix();
}

GLuint tex;
void init()
{

    int width, height, channel;
    unsigned char *data = stbi_load("01.png", &width, &height, &channel, 0);
    

    glGenTextures( 1, &tex );
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
void timer( int value )
{
    angle += delta;

 //   std::cout << "angle" << angle << std::endl;

    glutPostRedisplay();
    //glutTimerFunc( 16, timer, 0 );

    glutTimerFunc(30, timer, 0);


}

void MouseEvent(int button, int state, int x, int y)
{
    Point pC = {x, y};	

    using namespace std::chrono_literals;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    
            initTexture('D');

            //if (int(angle) % 360 == 0) {

            //}
            if (isAngleInRange(angle, 350, 370)) {
                delta = 0;
            }


            std::cout << "MouseEvent" << angle << std::endl;
            //glutPostRedisplay();

        
        // нажатие кнопки мыши
    }
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // отжатие кнопки мыши
	}

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        // правая кнопка мыши нажатие
    //    delta += 3;
    }

}

// remove reshape
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}


void display()
{

    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //gluPerspective( 60, 1.0, 0.1, 100.0 );
    gluPerspective(60, 1.0, 0.1, 100.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0, 0, -5 );

    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );

    //uncomment
    //glRotatef( angle, 0.2, 0.3, 0.1 );
    

    //glRotatef( 90, 1, 0, 0 );
    glTranslatef( 0, 0, -1/2 );
    glRotatef(-90, 0.0f, 0.0f, 1.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);


    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, tex );

    cylinder(0.3333);
    cylinder(0.6666);
    cylinder(1);
 
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
			delta += 3;
			break;

		case 2:	// Stop
            		std::cout << "2" << std::endl;
			delta = 0;
			break;

		case 3:	// Exit
			exit(0);
			break;
	}

}

void draw_pixel(Point p) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(color.red, color.green, color.blue);
	glBegin(GL_POINTS);
	glVertex2i(p.x, p.y);
	glEnd();
	glFlush();
}


int main(int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 600, 600 );
    glutCreateWindow( "Octavian" );
    init();
    
    initTexture('B');
    
    glutDisplayFunc( display );
   
        
    int subMenu = glutCreateMenu(subMenuHandler);
    glutAddMenuEntry("status", 0);

    glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Status", subMenu);

    glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("Stop", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);



    
    glutMouseFunc(MouseEvent);
 //   glutTimerFunc( 0, timer, 0 );
    glutTimerFunc( 1000, timer, 0);
//    glutTimerFunc( 10, timer, 1 );
//    glutTimerFunc( 10, timer, 2 );
    glutMainLoop();
    return 0;
}


