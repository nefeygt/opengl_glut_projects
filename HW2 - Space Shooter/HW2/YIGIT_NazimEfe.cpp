/*********
   CTIS164 - Template Source Program
----------
STUDENT : Nazim Efe YIGIT
SECTION : 3
HOMEWORK: 2
----------
PROBLEMS: No problems hocam this code is so close to being perfect.
----------
ADDITIONAL FEATURES: When you successfully hit a target countdown timer's duration extends by one second. I added a cool starry background on the last minute to make the game look more aesthetic.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

// Important constants
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

// Other constants
#define TARGET_SPAWN_MIN_DELAY 150
#define TARGET_SPAWN_MAX_DELAY 500
#define GAME_TIME 20
#define TARGET_COUNT 5
#define MAX_BULLETS 5

#define D2R 0.0174532
#define M_PI 3.14285

// Structures
typedef struct {
    float x, y;
    bool active;
    float speed;
} Target;
typedef struct {
    float x, y;
    bool active;
} Bullet;

// Global Variables
static int windowWidth = 800;
static int windowHeight = 600;
static int gameTime = GAME_TIME;
static bool paused = false;
static char keysText[] = "Press Space to fire, F1 to pause/play, Esc to quit";
static char remainingTimeText[50];
static char titleText[] = "Pushing the Empire Back by Nazým Efe Yiðit";
static Target targets[TARGET_COUNT];
static Bullet bullet = { 0.0f, 0.0f, false };
static Bullet bullets[MAX_BULLETS] = { 0 };
static float weaponY = 0.0f; // Add weaponY variable to store the weapon's Y position
static int targetSpawnDelay = TARGET_SPAWN_MIN_DELAY;
static int score = 0;
static bool gameOver = false;
static char scoreText[50];

/* Global Variables for Template File 
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height */

// Function list
void display();
void onSpecialKeyDown(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void resize(int width, int height);
void timer(int value);
void init();
void drawText(float x, float y, const char* text, void* font);
void drawWeapon();
void drawBullet();
void drawTargets();
void special(int key, int x, int y); // Add special function prototype
void addTarget();
void countdownTimer(int value);
void drawGameOver();
void restartGame();


//
// To display onto window using OpenGL commands
//
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw background
    srand(time(NULL));
    glColor3f(0.0f, 1.0f, 1.0f);
    for (int i = 0; i < 150; i++) { 
        float x = rand() % windowWidth;
        float y = rand() % windowHeight;
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }

    if (!gameOver) {
        // Draw weapon
        drawWeapon();

        // Draw targets
        drawTargets();

        // Draw bullet
        drawBullet();

        // Draw remaining time
        sprintf(remainingTimeText, "Remaining Time: %d", gameTime);
        drawText(10, 10, remainingTimeText, GLUT_BITMAP_9_BY_15);

        // Draw score
        sprintf(scoreText, "Score: %d", score);
        drawText(windowWidth - 100, 10, scoreText, GLUT_BITMAP_9_BY_15);

        // Draw keys
        drawText(10, windowHeight - 25, keysText, GLUT_BITMAP_9_BY_15);
    }
    else {
        // Draw game over screen
        drawGameOver();
    }

    glutSwapBuffers();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        weaponY += 10.0f;
        if (weaponY > windowHeight - 40.0f) {
            weaponY = windowHeight - 40.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        weaponY -= 10.0f;
        if (weaponY < 0.0f) {
            weaponY = 0.0f;
        }
        break;
    case GLUT_KEY_F1:
        paused = !paused;
        if (!paused) {
            timer(0);
            countdownTimer(0);
        }
        if (gameOver) {
            restartGame();
            glutPostRedisplay();
            //paused = !paused;
        }
        break;
    default:
        break;
    }
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
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
        exit(0);
        break;
    case ' ':
        if (!gameOver) {
            // Fire weapon
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].x = 40.0f;
                    bullets[i].y = weaponY + 20.0f;
                    bullets[i].active = true;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}

//
// This function is called when the window size changes.
//
void resize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif
*/
//
// This is what keeps the program running
//
void timer(int value) {
    if (!paused && !gameOver) {

        if (gameTime <= 0) {
            gameTime = GAME_TIME;
            // Reset game
        }

        // Update target positions
        for (int i = 0; i < TARGET_COUNT; i++) {
            if (targets[i].active) {
                targets[i].y += targets[i].speed; // Change this line to make targets move
                if (targets[i].y < 0.0f || targets[i].y > windowHeight) {
                    targets[i].active = false;
                }
            }
            targetSpawnDelay--;

            // Check if it's time to spawn a new target
            if (targetSpawnDelay <= 0) {
                addTarget();
                // Set a new random delay before spawning the next target
                targetSpawnDelay = rand() % (TARGET_SPAWN_MAX_DELAY - TARGET_SPAWN_MIN_DELAY) + TARGET_SPAWN_MIN_DELAY;
            }

        }

        // Move the bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].x += 20.0f;
                if (bullets[i].x > windowWidth) {
                    bullets[i].active = false;
                }
            }
        }

        // Check for bullet-target collisions
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                for (int j = 0; j < TARGET_COUNT; j++) {
                    // This sets the hitbox, increased it becuase it was too hard to hit before
                    if (targets[j].active && sqrt(pow(bullets[i].x - targets[j].x, 2) + pow(bullets[i].y - targets[j].y, 2)) < 20.0f) {
                        targets[j].active = false;
                        bullets[i].active = false;
                        // Increase score and remaining time
                        score++;
                        gameTime++;
                        break;
                    }
                }
            }
        }

        glutTimerFunc(1000 / 60, timer, 0);
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize targets
    for (int i = 0; i < TARGET_COUNT; i++) {
        addTarget();
    }

    // Set weapon's starting position
    weaponY = (windowHeight - 40.0f) / 2.0f;
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(titleText);

    // Register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(resize);
    glutTimerFunc(1000, timer, 0);
    glutSpecialFunc(special); // Add special callback
    glutSpecialFunc(onSpecialKeyDown);

    init();
    glutTimerFunc(1000, countdownTimer, 0); // Add the new countdownTimer

    // Start the main loop
    glutMainLoop();
    return 0;
}

// Long versions of the functions
// 1
void drawText(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

// 2
void drawWeapon() {
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw the bow
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = 2 * M_PI * i / 100;
        float x = 30.0f + 10.0f * cos(angle);
        float y = weaponY + 20.0f + 20.0f * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Supposed to be bowstring
    glBegin(GL_LINES);
    glVertex2f(30.0f, weaponY);
    glVertex2f(30.0f, weaponY + 40.0f);
    glEnd();
}

// 3
void drawBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            glColor3f(0.8f, 0.0f, 1.0f);

            // Draw arrow shaft
            glBegin(GL_QUADS);
            glVertex2f(bullets[i].x - 1.0f, bullets[i].y - 1.0f);
            glVertex2f(bullets[i].x - 1.0f, bullets[i].y + 1.0f);
            glVertex2f(bullets[i].x + 19.0f, bullets[i].y + 1.0f); // Make the arrow shaft longer
            glVertex2f(bullets[i].x + 19.0f, bullets[i].y - 1.0f); // Make the arrow shaft longer
            glEnd();

            // Draw arrowhead
            glBegin(GL_TRIANGLES);
            glVertex2f(bullets[i].x + 19.0f, bullets[i].y);
            glVertex2f(bullets[i].x + 28.0f, bullets[i].y - 4.0f); // Make the arrowhead sharper
            glVertex2f(bullets[i].x + 28.0f, bullets[i].y + 4.0f); // Make the arrowhead sharper
            glEnd();

            // Draw weird two short lines at the end of the arrow
            glBegin(GL_LINES);
            glVertex2f(bullets[i].x - 1.0f, bullets[i].y - 1.0f);
            glVertex2f(bullets[i].x - 6.0f, bullets[i].y - 4.0f);
            glVertex2f(bullets[i].x - 1.0f, bullets[i].y + 1.0f);
            glVertex2f(bullets[i].x - 6.0f, bullets[i].y + 4.0f);
            glEnd();
        }
    }

}

// 4
void drawTargets() {
    glColor3f(1.0f, 0.5f, 0.4f);
    for (int i = 0; i < TARGET_COUNT; i++) {
        if (targets[i].active) {
            glPushMatrix();
            glTranslatef(targets[i].x, targets[i].y, 0.0f);
            // Draw circular body
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            for (int j = 0; j <= 100; j++) {
                float angle = 2 * M_PI * j / 100;
                float x = 20.0f * cos(angle); // Change radius to 20.0f
                float y = 20.0f * sin(angle); // Change radius to 20.0f
                glVertex2f(x, y);
            }
            glEnd();

            // Draw left wing
            glBegin(GL_TRIANGLES);
            glVertex2f(-20.0f, 0.0f); // Change starting point to -20.0f
            glVertex2f(-40.0f, 20.0f); // Change end points to -40.0f and 20.0f
            glVertex2f(-40.0f, -20.0f); // Change end points to -40.0f and -20.0f
            glEnd();

            // Draw right wing
            glBegin(GL_TRIANGLES);
            glVertex2f(20.0f, 0.0f); // Change starting point to 20.0f
            glVertex2f(40.0f, 20.0f); // Change end points to 40.0f and 20.0f
            glVertex2f(40.0f, -20.0f); // Change end points to 40.0f and -20.0f
            glEnd();

            glPopMatrix();
        }
    }
}

// 5 
void special(int key, int x, int y) {
    // Move the weapon on the Y-axis
    switch (key) {
    case GLUT_KEY_UP:
        if (weaponY + 10.0f < windowHeight - 40.0f) {
            weaponY += 10.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        if (weaponY - 10.0f > 0.0f) {
            weaponY -= 10.0f;
        }
        break;
    }
    glutPostRedisplay();
}

// 6
void addTarget() {
    // Count the number of active targets
    int activeTargets = 0;
    for (int i = 0; i < TARGET_COUNT; i++) {
        if (targets[i].active) {
            activeTargets++;
        }
    }

    // If there are fewer than TARGET_COUNT active targets, spawn a new one
    if (activeTargets < TARGET_COUNT) {
        for (int i = 0; i < TARGET_COUNT; i++) {
            if (!targets[i].active) {
                targets[i].x = (float)((windowWidth - 40) - (rand() % 201));
                targets[i].y = 0.0f;
                // Target speed 
                targets[i].speed = (float(rand() % 1800 + 1800)) / 1000.0f;
                targets[i].active = true;
                break;
            }
        }
    }
}

// 7
//
// The function for countdown on bottom left
//
void countdownTimer(int value) {
    if (!paused && !gameOver) {
        gameTime--;

        if (gameTime == 0) {
            gameOver = true;
        }

        glutTimerFunc(1000, countdownTimer, 0);
    }
}

// 8
void drawGameOver() {
    glColor3f(1.0f, 1.0f, 1.0f);
    char gameOverText[] = "Game Over!";
    char scoreText[50];
    sprintf(scoreText, "Your Score: %d", score);
    char restartText[] = "Press 'F1' to restart or Esc to quit";

    drawText((windowWidth / 2) - 40, windowHeight / 2, gameOverText, GLUT_BITMAP_9_BY_15);
    drawText((windowWidth / 2) - 40, (windowHeight / 2) - 20, scoreText, GLUT_BITMAP_9_BY_15);
    drawText((windowWidth / 2) - 100, (windowHeight / 2) - 40, restartText, GLUT_BITMAP_9_BY_15);
}

// 9
void restartGame() {
    // Reset game state variables
    gameOver = false;
    score = 0;
    gameTime = GAME_TIME;

    // Reset targets
    for (int i = 0; i < TARGET_COUNT; i++) {
        addTarget();
    }

    // Reset bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }

    // Set paused to false (fix)
    paused = false;

    // Set weapon's starting position
    weaponY = (windowHeight - 40.0f) / 2.0f;

    // Start the timers
    timer(0);
    countdownTimer(0);

    // Redraw the game screen
    glutPostRedisplay();
}