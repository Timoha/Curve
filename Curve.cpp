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
// #include "Shape.cpp"



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
vector <Shape*> faces;

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



Vector3d curveInterp(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d v3, double u){
    Vector3d A = v0*(1.0 - u) + v1*u;
    Vector3d B = v1*(1.0 - u) + v2*u;
    Vector3d C = v2*(1.0 - u) + v3*u;

    Vector3d D = A*(1 - u) + B*u;
    Vector3d E = B*(1 - u) + C*u;
    Vector3d p = D*(1 - u) + E*u;

    return p;
}

Vector3d patchInterp(Patch patch, double u, double v){

    Vector3d vcurve0 = curveInterp(patch.q[0], patch.q[1], patch.q[2], patch.q[3], u);
    Vector3d vcurve1 = curveInterp(patch.q[4], patch.q[5], patch.q[6], patch.q[7], u);
    Vector3d vcurve2 = curveInterp(patch.q[8], patch.q[9], patch.q[10], patch.q[11], u);
    Vector3d vcurve3 = curveInterp(patch.q[12], patch.q[13], patch.q[14], patch.q[15], u);

    Vector3d ucurve0 = curveInterp(patch.q[0], patch.q[4], patch.q[8], patch.q[12], v);
    Vector3d ucurve1 = curveInterp(patch.q[1], patch.q[5], patch.q[9], patch.q[13], v);
    Vector3d ucurve2 = curveInterp(patch.q[2], patch.q[6], patch.q[10], patch.q[14], v);
    Vector3d ucurve3 = curveInterp(patch.q[3], patch.q[7], patch.q[11], patch.q[15], v);

    Vector3d p = curveInterp(vcurve0, vcurve1, vcurve2, vcurve3, v);
    return p;
}


void uniformTesselate(Patch patch, unsigned int numdiv){
    double step = numdiv;
    double stepsize = (double)(1 / step);
    for (unsigned int u = 0; u<numdiv; ++u){
        for (unsigned int v = 0; v < numdiv; ++v){
            Vector3d A = patchInterp(patch, u*stepsize, v*stepsize);
            Vector3d B = patchInterp(patch, (1+u)*(stepsize), v*stepsize);
            Vector3d C = patchInterp(patch, (1+u)*(stepsize), (1+v)*(stepsize));
            Vector3d D = patchInterp(patch, u*stepsize, (1+v)*(stepsize));

            Quad* q = new Quad(A, B, C, D);
            faces.push_back(q);

            // glBegin(GL_LINE_STRIP);
            // glVertex3d(A[0], A[1], A[2]);
            // glVertex3d(B[0], B[1], B[2]);
            // glVertex3d(C[0], C[1], C[2]);
            // glVertex3d(D[0], D[1], D[2]);
            // glEnd();
        }
    }
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
        for (int i = 0; i < faces.size(); i++) {
            faces[i]->draw();
        }
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

    for (int i = 0; i < numPatches; i++) {
        uniformTesselate(bezpatches[i], 20);
    }


	glutInit(&argc, argv);

	//This tells glut to use a double-buffered window with red, green, and blue channels
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Initalize theviewport size
	viewport.w = 600;
	viewport.h = 600;

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
