/*********
   CTIS164 - Template Source Program
----------
STUDENT : NAZIM EFE YIGIT
SECTION : 3
HOMEWORK: 1
----------
PROBLEMS: There is an overlapping problem. The cross shaped line on the window should be the closest to the viewer but the UFO, planet and meteors seem closer when they're flying across that lines.
----------
ADDITIONAL FEATURES: I added stars, meteors and a planet to make the space more occupied. 
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <cmath>
#include <string>

#define _USE_MATH_DEFINES

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define D2R 0.0174532
#define M_PI 3.14

// Function prototypes
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void timer(int);

void drawWall();
void drawScrew();
void drawUFO();
void drawSpaceBackground();
void drawPlanet();
void drawMeteor();
void drawMeteors();
void drawGrid();
void drawNameLabel();

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  width = 800, height = 600; // current Window width and height
//universal
int animation_paused = 0;
//UFO
float ufo_position_x = 100.0f;
float ufo_position_y = 100.0f;
float ufo_speed_x = 1.0f;
float ufo_speed_y = 1.0f;
//innermost window
float space_background_offset = 0.0f;
float space_background_speed = 0.15f;
//stars
const int num_stars = 500;
float star_positions_x[num_stars];
float star_positions_y[num_stars];
//planet
float planet_position_x = 0.0f;
float planet_speed_x = 0.1f;
//meteors
const int num_meteors = 10;
float meteor_positions_x[num_meteors];
float meteor_positions_y[num_meteors];
float meteor_speeds_x[num_meteors];
float meteor_speeds_y[num_meteors];


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // purple background
    //
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw scene elements
    drawWall();
    void drawScrew();
    drawSpaceBackground();
    drawPlanet();
    drawMeteors();
    drawUFO();
    drawGrid();
    drawNameLabel();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    //to stop the complex object
    if (key == 32) { // Spacebar
        animation_paused = !animation_paused;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void timer(int value) {

    // Write your codes here.
    if (!animation_paused) {
        // Update UFO position
        ufo_position_x += ufo_speed_x;
        ufo_position_y += ufo_speed_y;
        if (ufo_position_x >= (width - 100) || ufo_position_y >= (height - 100)) {
            ufo_position_x = 100.0f;
            ufo_position_y = 100.0f;
        }

        // Update space background offset
        space_background_offset += space_background_speed * 1.2f;
        if (space_background_offset > 380) {
            space_background_offset -= 380;
        }
    }

    // Update star positions
    for (int i = 0; i < num_stars; ++i) {
        star_positions_y[i] += 0.33f; // Adjust this value to change the speed of the stars
        if (star_positions_y[i] > height - 100) {
            star_positions_y[i] -= (height - 100 - 100);
        }
    }

    // Update planet position
    planet_position_x += planet_speed_x * 1.2f;
    if (planet_position_x > 380) {
        planet_position_x = -50.0f;
    }

    // Update meteors positions
    for (int i = 0; i < num_meteors; ++i) {
        meteor_positions_x[i] += meteor_speeds_x[i] * 20;
        meteor_positions_y[i] += meteor_speeds_y[i] * 20;

        if (meteor_positions_x[i] < 100 || meteor_positions_x[i] > width - 100) {
            meteor_speeds_x[i] = -meteor_speeds_x[i];
        }
        if (meteor_positions_y[i] < 100 || meteor_positions_y[i] > height - 100) {
            meteor_speeds_y[i] = -meteor_speeds_y[i];
        }
    }

    glutPostRedisplay();

    glutTimerFunc(1000 / 60, timer, 0); // 60 FPS


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void drawScrew(float x, float y) {
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + 2 * cos(angle), y + 2 * sin(angle));
    }
    glEnd();
}
void drawWall() {
    // Draw new outer gray border
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(70, 70);
    glVertex2f(width - 70, 70);
    glVertex2f(width - 70, height - 70);
    glVertex2f(70, height - 70);
    glEnd();

    // Draw screws
    drawScrew(75, 75);
    drawScrew(width - 75, 75);
    drawScrew(width - 75, height - 75);
    drawScrew(75, height - 75);

    // Draw silver colored lines connecting screws
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINES);
    glVertex2f(75, 75);
    glVertex2f(width - 75, 75);
    glVertex2f(width - 75, 75);
    glVertex2f(width - 75, height - 75);
    glVertex2f(width - 75, height - 75);
    glVertex2f(75, height - 75);
    glVertex2f(75, height - 75);
    glVertex2f(75, 75);
    glEnd();

    // Draw original red border
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(90, 90);
    glVertex2f(width - 90, 90);
    glVertex2f(width - 90, height - 90);
    glVertex2f(90, height - 90);
    glEnd();
}
void drawUFO() {
    glPushMatrix();
    glTranslatef(ufo_position_x, ufo_position_y, 0.0f);
    glRotatef(10, 0.0f, 0.0f, 1.0f); // Rotate the UFO 10 degrees to the right

    // Draw UFO body (scale down the size by 0.5)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.5 * 50 * cos(angle), 0.5 * 25 * sin(angle));
    }
    glEnd();

    // Draw UFO top (scale down the size by 0.5)
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.5 * 25 * cos(angle), 0.5 * 25 * sin(angle));
    }
    glEnd();

    // Draw UFO lights (scale down the size by 0.5)
    glColor3f(1.0f, 1.0f, 0.05);
    for (int i = 0; i < 360; i += 45) {
        glBegin(GL_QUADS);
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.5 * (50 * cos(angle) - 5), 0.5 * (25 * sin(angle) - 5));
        glVertex2f(0.5 * (50 * cos(angle) + 5), 0.5 * (25 * sin(angle) - 5));
        glVertex2f(0.5 * (50 * cos(angle) + 5), 0.5 * (25 * sin(angle) + 5));
        glVertex2f(0.5 * (50 * cos(angle) - 5), 0.5 * (25 * sin(angle) + 5));
        glEnd();
    }
    glPopMatrix();
}
void drawSpaceBackground() {
    // Draw background
    glColor3f(0.0f, 0.0f, 0.4f); // Darker blue
    glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(width - 100, 100);
    glVertex2f(width - 100, height - 100);
    glVertex2f(100, height - 100);
    glEnd();

    // Draw stars
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < num_stars; ++i) {
        glVertex2f(star_positions_x[i], star_positions_y[i]);
    }
    glEnd();


    // Draw handlebars
    glColor3f(0.75f, 0.75f, 0.75f); // Light gray
    glBegin(GL_QUADS);
    glVertex2f(100, height / 2 - 20);
    glVertex2f(120, height / 2 - 20);
    glVertex2f(120, height / 2 + 20);
    glVertex2f(100, height / 2 + 20);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(width - 120, height / 2 - 20);
    glVertex2f(width - 100, height / 2 - 20);
    glVertex2f(width - 100, height / 2 + 20);
    glVertex2f(width - 120, height / 2 + 20);
    glEnd();

    // Draw window line
    glLineWidth(12.0f); // Set line width to 3 pixels
    glColor3f(0.1f, 0.1f, 0.1f); // dark gray

    //first line    
    glBegin(GL_LINES);
    glVertex2f(100, 100);
    glVertex2f(width - 100, height - 100);
    glEnd();
    // Draw the symmetrical line
    glBegin(GL_LINES);
    glVertex2f(width - 100, 100);
    glVertex2f(100, height - 100);
    glEnd();
}
void drawPlanet() {
    glPushMatrix();
    glTranslatef(210, 110, 0.0f);
    glTranslatef(planet_position_x, 200, 0.0f);
    // Draw planet body
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(25 * cos(angle), 25 * sin(angle));
    }
    glEnd();
    glPopMatrix();
}
void drawMeteor(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glColor3f(0.6f, 0.4f, 0.2f); // Brown color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(5 * cos(angle), 5 * sin(angle));
    }
    glEnd();

    glPopMatrix();
}
void drawMeteors() {
    for (int i = 0; i < num_meteors; ++i) {
        drawMeteor(meteor_positions_x[i], meteor_positions_y[i]);
    }
}
void drawGrid() {
    int gridSize = 20;
    int borderSize = 10; // Adjust this value based on your border size

    glColor3f(0.3f, 0.0f, 0.3f); // Darker purple for the grid lines
    for (int x = borderSize; x < width - borderSize; x += gridSize) {
        glBegin(GL_LINES);
        glVertex2f(x, 0);
        glVertex2f(x, borderSize);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(x, height - borderSize);
        glVertex2f(x, height);
        glEnd();
    }

    for (int y = borderSize; y < height - borderSize; y += gridSize) {
        glBegin(GL_LINES);
        glVertex2f(0, y);
        glVertex2f(borderSize, y);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(width - borderSize, y);
        glVertex2f(width, y);
        glEnd();
    }
}
void drawNameLabel() {
    // Draw a silver rectangle
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f((width / 2) - 100, 20);
    glVertex2f((width / 2) + 100, 20);
    glVertex2f((width / 2) + 100, 60);
    glVertex2f((width / 2) - 100, 60);
    glEnd();

    // Write "NAZIM EFE YIGIT" inside the rectangle
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f((width / 2) - 80, 25);
    std::string text = "NAZIM EFE YIGIT";

    for (const char& c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void main(int argc, char* argv[]) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("HW1 UFO Animation");

    //timer event
    glutTimerFunc(1000 / 60, timer, 0);

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

    // Initialize star positions
    for (int i = 0; i < num_stars; ++i) {
        star_positions_x[i] = (rand() % (int)(width - 200)) + 100;
        star_positions_y[i] = (rand() % (int)(height - 200)) + 100;
    }

    // Initialize meteor positions and speeds
    for (int i = 0; i < num_meteors; ++i) {
        meteor_positions_x[i] = (rand() % (int)(width - 200)) + 100;
        meteor_positions_y[i] = (rand() % (int)(height - 200)) + 100;
        meteor_speeds_x[i] = (rand() % 10 - 5) / 500.0f * 2.0f;
        meteor_speeds_y[i] = (rand() % 10 - 5) / 500.0f * 2.0f;
    }

    Init();

    glutMainLoop();
}