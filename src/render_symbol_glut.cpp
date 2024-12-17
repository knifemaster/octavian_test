#include <stdio.h>
#include <GL/glut.h>
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

void initTexture() {
    if (!get_character_bitmap('A', 100, &bitmap)) {
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("FreeType2 and GLUT");

    initTexture();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}