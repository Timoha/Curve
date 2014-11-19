// Curve.cpp : Defines the entry point for the console application.
//
//andrey, leave stdafx.h out of your code. this is a stupid windows thing

#include <vector>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <vector>
#include <time.h>
#include <limits>


#include "Shape.h"



#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


class Viewport {
public:
	int w, h; // width and height
};


class Patch{
public:
	vector <Vector3d> q;
	Patch(vector <Vector3d> q);
};

Patch::Patch(vector <Vector3d> q1){
	q = q1;
}

//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;
int numPatches;
vector <Vector3d> coordinates;
vector <Patch> bezpatches;

int rotateX = 0;
int rotateY = 0;
double translateX = 0;
double translateY = 0;
double scale = 1;

const double TRANSLATE_DELTA = 0.04;
const int ROTATE_DELTA = 3;
const double SCALE_DELTA = 0.5;



void mySpecialKeys (int key, int x, int y) {



    int mod = glutGetModifiers();
    bool isShift = false;

    switch (mod) {
        case GLUT_ACTIVE_SHIFT:
            isShift = true;
            break;
    }


    switch (key) {

        case GLUT_KEY_LEFT:
            if (isShift) {
                translateX -= TRANSLATE_DELTA;
            } else {
                rotateY -= ROTATE_DELTA;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (isShift) {
                translateX += TRANSLATE_DELTA;
            } else {
                rotateY += ROTATE_DELTA;
            }
            break;
        case GLUT_KEY_UP:
            if (isShift) {
                translateY += TRANSLATE_DELTA;
            } else {
                rotateX -= ROTATE_DELTA;
            }
            break;
        case GLUT_KEY_DOWN:
            if (isShift) {
                translateY -= TRANSLATE_DELTA;
            } else {
                rotateX += ROTATE_DELTA;
            }
            break;
        default:
            cout << "Special key " << key << endl;
    }

    glutPostRedisplay();
}

//---+----3----+----2----+----1----+---<>---+----1----+----2----+----3----+----4

void myKeyboard (unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            scale /= SCALE_DELTA;
            break;
        case '-':
            scale *= SCALE_DELTA;
            break;
        default:
            cout << "myKeyboard " << key << endl;
    }

    glutPostRedisplay();
}



//****************************************************
// reshape viewport if the window is resized
//****************************************************



void myReshape(int w, int h) {
	viewport.w = w;
	viewport.h = h;

	glViewport(0, 0, viewport.w, viewport.h);// sets the rectangle that will be the window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                // loading the identity matrix for the screen

	//----------- setting the projection -------------------------
	// glOrtho sets left, right, bottom, top, zNear, zFar of the chord system
	// glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
	 glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 2, -2); // resize type = center

	//glOrtho(-4, 4, -4, 4, 2, -2);    // resize type = stretch

	//------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

	myReshape(viewport.w, viewport.h);
}


Vector3d deCasteljau(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, double u){
	return pow((1 - u), 3)*v0 + 3 * pow((1 - u), 2)*u*v1 + 3 * (1 - u)*pow(u, 2)*v2 + pow(u, 3)*v3;
}


void glgenCurve(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2, const Vector3d& v3, double u){
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i <= (unsigned int) 1/u; ++i){
		Vector3d result = deCasteljau(v0, v1, v2, v3, (double) (u*i));
		glVertex3f(result[0], result[1], result[2]);
	}
	glEnd();
}



void display() // adapted from http://stackoverflow.com/questions/13159444/opengl-draw-polygon-with-gl-lines-and-angle-between-lines
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    // glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

	glColor3ub(255, 0, 0);
	Vector3d v0(0.0, 0.0, 0.0);
	Vector3d v1(0.33, 0.33, 1.0);
	Vector3d v2(0.66, 0.33, 1.0);
	Vector3d v3(1.0, 0.0, 0.0);
	// glgenCurve(v0, v1, v2, v3, .01);

    Quad q(v0, v1, v2, v3);

    glPushMatrix();
        glTranslated(translateX, translateY, 0);
        glRotated(rotateX, 1, 0, 0);  // Up and down arrow keys 'tip' view.
        glRotated(rotateY, 0, 1, 0);  // Right/left arrow keys 'turn' view.
        glScaled(scale, scale, scale);
        q.draw();
    glPopMatrix();

    glFlush();
	glutSwapBuffers();
}

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
	//nothing here for now
#ifdef _WIN32
	Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
	glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************

void parseLine(const string& line) {
	istringstream iss(line);
	vector<string> tokens((istream_iterator<string>(iss)), istream_iterator<string>());


	if (tokens.size() == 0) {
		return;
	} else if (tokens.size() == 1) {
		numPatches = atoi(tokens[0].c_str());
	} else if (tokens.size() == 12) {
		for (int i = 0; i < 4; i++){
			coordinates.push_back(Vector3d(atof(tokens[3 * i+0].c_str()), atof(tokens[3 * i + 1].c_str()), atof(tokens[3 * i + 2].c_str())));
		}
	}
}



int main(int argc, char *argv[]) {

	ifstream fin;
	string line;

    if (argc >= 2) {
        fin.open(argv[1]);


        if (!fin.good()) {
            cout << "File \"" << argv[1] << "\" does not exists" << endl;
            return 1;
        }

        //after parsing is done, transfer data from buffer into numPatches, and then clumb all numPatches together in "bezpatches" vector
        while (getline(fin , line)) {

            try {
                parseLine(line);
            } catch (invalid_argument& e) {
                cerr << e.what() << endl;
            }
        }

        fin.close();
    } else {
        cout << "Invalid argument." << endl;
    }


	cout << numPatches << endl;
	for (int j = 0; j < numPatches; j++){
		vector <Vector3d> curve;
		for (int i = 0; i < 16; i++){
			curve.push_back(coordinates[j * 16 + i]);
		}
		Patch next(curve);
		//just for testing
		//cout << "new patch" << endl;
		//for (int k = 0; k < 16;  k++){
		//	cout << curve[k] << endl;
		//}
		bezpatches.push_back(next);
	}




	glutInit(&argc, argv);

	//This tells glut to use a double-buffered window with red, green, and blue channels
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Initalize theviewport size
	viewport.w = 400;
	viewport.h = 400;

	//The size and position of the window
	glutInitWindowSize(viewport.w, viewport.h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CS184!");

	initScene();                                 // quick function to set up scene

	glutDisplayFunc(display);                  // function to run when its time to draw something
	glutReshapeFunc(myReshape);                  // function to run when the window gets resized
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);
	glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
	glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

	return 0;
}


//for some reason, this function is shitty and juut doesn't work
//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

	glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
	glLoadIdentity();                            // make sure transformation is "zero'd"


	Vector3d v0(0.0, 0.0, 0.0);
	Vector3d v1(0.33, 0.33, 0.0);
	Vector3d v2(0.66, 0.33, 0.0);
	Vector3d v3(1.0, 0.0, 0.0);

	glgenCurve(v0, v1, v2, v3, .2);


	//----------------------- code to draw objects --------------------------
	// Rectangle Code
	//glColor3f(red component, green component, blue component);
	/*
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);                         // draw rectangle
	//glVertex3f(x val, y val, z val (won't change the point because of the projection type));

	glVertex3f(0.0f, 0.0f, 0.0f);               // bottom left corner of rectangle
	glVertex3f(0.33f, 0.33f, 0.0f);
	glVertex3f(0.66f, 0.33f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	*/

	/*
	glColor3f(1.0f, 0.0f, 0.0f);                   // setting the color to pure red 90% for the rect
	glBegin(GL_POLYGON);                         // draw rectangle
	//glVertex3f(x val, y val, z val (won't change the point because of the projection type));
	float farl =1.1f;
	glVertex3f(-0.8f, 0.0f, farl);               // bottom left corner of rectangle
	glVertex3f(-0.8f, 0.5f, farl);               // top left corner of rectangle
	glVertex3f(0.0f, 0.5f, farl);               // top right corner of rectangle
	glVertex3f(0.0f, 0.0f, farl);               // bottom right corner of rectangle
	glEnd();
	// Triangle Code
	glColor3f(1.0f, 0.5f, 0.0f);                   // setting the color to orange for the triangle

	float basey = -sqrt(0.48f);                  // height of triangle = sqrt(.8^2-.4^2)
	glBegin(GL_POLYGON);
	glVertex3f(.5f, 0.0f, 0.0f);                // top tip of triangle
	glVertex3f(0.1f, basey, 0.0f);               // lower left corner of triangle
	glVertex3f(0.9f, basey, 0.0f);               // lower right corner of triangle
	glEnd();
	//-----------------------------------------------------------------------
	*/
	glFlush();
	glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}
