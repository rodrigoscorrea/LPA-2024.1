/*
Aluno: Rodrigo Santos Correa
Matrícula: 22251139
*/

#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

int width, height, segQuantity;

typedef struct {
    float x;
    float y;
} Point;

Point generateRandomPoint(int width, int height) {
    Point p;
    p.x = rand() % width;
    p.y = rand() % height;
    return p;
}

int ccw(Point A, Point B, Point C) {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

int intersect(Point A, Point B, Point C, Point D) {
    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}

int isIntersecting(Point A, Point B, Point segments[], int count) {
    for (int i = 0; i < count; ++i) {
        Point C = segments[i * 2];
        Point D = segments[i * 2 + 1];
        if (intersect(A, B, C, D)) {
            return 1; 
        }
    }
    return 0; 
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);

    Point segments[2 * segQuantity]; 
    
    for (int i = 0; i < segQuantity; ++i) {
        Point A, B;
        do {
            A = generateRandomPoint(width, height);
            B = generateRandomPoint(width, height);
        } while (isIntersecting(A, B, segments, i)); 

        segments[i * 2] = A;
        segments[i * 2 + 1] = B;
    }

    for (int i = 0; i < segQuantity; ++i) {
        Point A = segments[i * 2];
        Point B = segments[i * 2 + 1];
        glVertex2f(A.x, A.y);
        glVertex2f(B.x, B.y);
    }

    glEnd();
    glFlush();
}

void main(int argc, char *argv[])
{
    srand(time(NULL));

    width = atoi(argv[1]);
    height = atoi(argv[2]);
    segQuantity = atoi(argv[3]);

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(width, height);
    glutCreateWindow ("Lab 11 - Segmentos - Rodrigo Santos");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();
}