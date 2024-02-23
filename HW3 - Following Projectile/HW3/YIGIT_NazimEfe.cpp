/*********
   CTIS164 - Template Source Program
----------
STUDENT : Nazim Efe Yigit
SECTION : 03
HOMEWORK: 03
----------
PROBLEMS: I couldn't use vertex function to rotate the ammo. I used matrix to solve this problem. 
----------
ADDITIONAL FEATURES: There are no additional features.
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
#include "vec.h"

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define D2R 0.0174532

// Global variables
vec_t targetPos = { 0.0, 0.0 }; // Initial position of the target object
vec_t ammoPos = { 200.0, 200.0 }; // Initial position of the ammo
vec_t ammoDirection = { 1.0, 0.0 }; // Initial direction of the ammo
int isTargetHit = 0; // Flag to check if the target object is hit
int isAmmoActive = 0; // Flag to check if the ammo is active
double ammoRotation = 0.0; // Rotation angle of the ammo's head
double targetSpeed = 4.0; // To adjust the speed of the target object
double ammoSpeed = 5.0; // To adjust the speed of the ammo
vec_t targetDestination = { WINDOW_WIDTH, WINDOW_HEIGHT }; // Aims the top right corner for the target object
int isTargetMoving = 1; // Flag to check if the target object is moving

// Function prototypes
void display();
void drawAmmo(float cx, float cy, float width, float height, float rotation);
void updateFlyingObject();
void updateTargetObject();
void update(int value);
void checkCollision();
void drawCircle(float cx, float cy, float r, int numSegments);
void drawRectangle(float cx, float cy, float width, float height);

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && !isAmmoActive) {
        // Update the position of the flying object (ammo) based on mouse click
        ammoPos.x = (double)x;
        ammoPos.y = (double)(WINDOW_HEIGHT - y);
        isAmmoActive = 1; // Set the ammo as active
        isTargetHit = 0; // Reset the target hit flag
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}
void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
//
// To display onto window using OpenGL commands
//
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // background
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    // sea
    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2);
    glVertex2f(0, WINDOW_HEIGHT / 2);
    glEnd();

    // beach
    glColor3f(0.96, 0.88, 0.71);
    glBegin(GL_QUADS);
    glVertex2f(0, WINDOW_HEIGHT / 2);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    // mountain
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_TRIANGLES);
    glVertex2f(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2);
    glVertex2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 400);
    glVertex2f(WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2);
    glEnd();

    // target
    glColor3f(1.0, 0.5, 0.0);
    drawCircle(targetPos.x, targetPos.y, 40.0, 50);

    // ammo
    if (isAmmoActive) {
        drawAmmo(ammoPos.x, ammoPos.y, 30.0, 15.0, ammoRotation); // Increased ammo size
    }

    glutSwapBuffers();
}
void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Chasing the Target Game by Nazim Efe Yigit");


    glutDisplayFunc(display);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    //glutSpecialFunc(onSpecialKeyDown);
    //glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);

    //glutMotionFunc(onMoveDown);
    //glutPassiveMotionFunc(onMove);

    glutTimerFunc(0, update, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutMainLoop();
}

void drawAmmo(float cx, float cy, float width, float height, float rotation) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glTranslatef(-cx, -cy, 0.0);

    // body
    glColor3f(0.0, 0.0, 1.0);
    drawRectangle(cx, cy, width, height);

    // head
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx + width / 2.0, cy + height / 1.6); 
    glVertex2f(cx + width / 2.0, cy - height / 1.6); 
    glVertex2f(cx + width, cy);
    glEnd();

    // line
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glVertex2f(cx + width / 2.0, cy + height / 3.0);  
    glVertex2f(cx + width / 2.0, cy - height / 3.0); 
    glEnd();

    glPopMatrix();
}
void updateFlyingObject() {
    if (isAmmoActive && !isTargetHit) {
        // calculate the direction from ammo to target
        vec_t direction = subV(targetPos, ammoPos);
        ammoDirection = unitV(direction); // to update the direction of ammo
        ammoPos = addV(ammoPos, mulV(ammoSpeed, ammoDirection)); // to move ammo
        ammoRotation = atan2(ammoDirection.y, ammoDirection.x) * 180.0 / 3.1415926;
    }
}
void updateTargetObject() {
    if (isTargetMoving) {
        // calculate the direction from target to the top right corner
        vec_t direction = subV(targetDestination, targetPos);
        vec_t targetVelocity = mulV(targetSpeed, unitV(direction));
        targetPos = addV(targetPos, targetVelocity); // to move target

        if (magV(subV(targetPos, targetDestination)) < targetSpeed) {
            targetPos.x = 0.0;
            targetPos.y = 0.0;
        }
    }
}
void update(int value) {
    updateFlyingObject();
    updateTargetObject();
    checkCollision();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
void checkCollision() {
    double distance = magV(subV(ammoPos, targetPos));
    if (distance < 20.0) {
        isTargetHit = 1;
        isAmmoActive = 0; // make de ammo disappear when it hits the target
        ammoPos.x = -1000.0; // teleport the ammo outside the visible area
        ammoPos.y = -1000.0;
    }
}
//
// to draw circle, center at (cx,cy)
// radius r
void drawCircle(float cx, float cy, float r, int numSegments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
//
// to draw a rectangle, center at (cx, cy)
// 
void drawRectangle(float cx, float cy, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(cx - width / 2, cy - height / 2);
    glVertex2f(cx + width / 2, cy - height / 2);
    glVertex2f(cx + width / 2, cy + height / 2);
    glVertex2f(cx - width / 2, cy + height / 2);
    glEnd();
}