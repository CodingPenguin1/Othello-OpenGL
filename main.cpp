#include <GL/glut.h>
#include <chrono>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define DISPLAY_SIZE 1000
#define BOARD_SIZE 8


// Grid setup
int points[BOARD_SIZE][BOARD_SIZE];


// Misc variables
double start_time;
bool *key_states = new bool[256];
bool *special_key_states = new bool[256];
const int framerate_average_count = 5;
double *frame_times = new double[framerate_average_count];


float map(float val, float old_start, float old_end, float new_start, float new_end) {
    return (val - old_start) / (old_end - old_start) * (new_end - new_start) + new_start;
}


double time() {
    return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void draw_string(float x, float y, std::string s) {
    glRasterPos2f(x, y);
    for (int i = 0; i < (int)s.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}

void draw_board() {
    // Clear to green
    glClearColor(0.282352941, 0.364705882, 0.247058824, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw lines between cells
    glLineWidth(3);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (float x = -1.0 + (2.0 / BOARD_SIZE); x < 1.0; x += 2.0 / (BOARD_SIZE)) {
        // Horizontal lines
        glVertex2f(-1.0, x);
        glVertex2f(1.0, x);
        // Vertical lines
        glVertex2f(x, -1.0);
        glVertex2f(x, 1.0);
    }
    glEnd();
}


void display() {
    // Setup
    double frame_start_time = time();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw board background
    draw_board();

    // Calculate framerate
    for (int i = 1; i < framerate_average_count; ++i)
        frame_times[i] = frame_times[i + 1];
    frame_times[4] = time() - frame_start_time;
    double framerate = framerate_average_count / (frame_times[0] + frame_times[1] + frame_times[2] + frame_times[3] + frame_times[4]);

    // Render settings text
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    draw_string(-0.99, 0.97, std::to_string((int)framerate) + " FPS");
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}


void init(int argc, char **argv) {
    // Set up window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(DISPLAY_SIZE, DISPLAY_SIZE);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("[floating] Othello");
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);

    // Set start time of program
    start_time = time();
    printf("Start time: %f\n", start_time);

    // Initialize framerate average array
    for (int i = 0; i < framerate_average_count; ++i)
        frame_times[i] = 0.0;
}


int main(int argc, char **argv) {
    init(argc, argv);
    glutMainLoop();

    // Exit
    delete[] frame_times;
    return 0;
}
