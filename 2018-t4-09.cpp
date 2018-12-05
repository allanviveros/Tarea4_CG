// Universidad Tecnica Federico Santa Maria
// Departamento de Informatica
// INF342 Computación Grafica
// Hubert Hoffmann
// T4
// Juan Pablo Leon (201473047-2)
// Allan Viveros (201473056-k)
// Numero de Grupo: 9
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
using namespace std;

int light0 = 0;
int light1 = 0;

GLfloat x = 1.0f;
GLfloat y = 1.0f;
GLfloat z = 1.0f;
GLfloat a = -1.0f;
GLfloat b = -1.0f;
GLfloat c = 1.0f;

typedef float point[4];

// Initial tetrahedron

point v[] = { {0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
		  {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333} };

int n = 4, mode;


void OnOffLight();
void myinit();
void Controls(unsigned char key, int xx, int yy) {
	float diff = 1.0;
	switch (key) {
	case '0':
		light0 = 0;
		break;
	case '1':
		light0 = 1;
		break;
	case '2':
		light1 = 0;
		break;
	case '3':
		light1 = 1;
		break;
	case 'x':
		x += diff;
		break;
	case 'X':
		x -= diff;
		break;
	case 'y':
		y += diff;
		break;
	case 'Y':
		y -= diff;
		break;
	case 'z':
		z += diff;
		break;
	case 'Z':
		z -= diff;
		break;
	case 'a':
		a += diff;
		break;
	case 'A':
		a -= diff;
		break;
	case 'b':
		b += diff;
		break;
	case 'B':
		b -= diff;
		break;
	case 'c':
		c += diff;
		break;
	case 'C':
		c -= diff;
		break;
	case 27:
		exit(0);
	}
	glutPostRedisplay();
}


// Display one triangle using a line loop for wire frame, a single
// normal for constant shading, or three normals for interpolative shading
void triangle(point a, point b, point c)
{
	if (mode == 0)	glBegin(GL_LINE_LOOP);
	else		glBegin(GL_POLYGON);
	if (mode == 1) glNormal3fv(a);
	if (mode == 2) glNormal3fv(a);
	glVertex3fv(a);
	if (mode == 2) glNormal3fv(b);
	glVertex3fv(b);
	if (mode == 2) glNormal3fv(c);
	glVertex3fv(c);
	glEnd();
}

// Normalize a vector
void normal(point p)
{
	//double sqrt();
	float d = 0.0;
	int i;
	for (i = 0; i < 3; i++) d += p[i] * p[i];
	d = sqrt(d);
	if (d > 0.0) for (i = 0; i < 3; i++) p[i] /= d;
}

// Triangle subdivision using vertex numbers
// righthand rule applied to create outward pointing faces
void divide_triangle(point a, point b, point c, int m)
{
	point v1, v2, v3;
	int j;
	if (m > 0)
	{
		for (j = 0; j < 3; j++) v1[j] = a[j] + b[j];
		normal(v1);
		for (j = 0; j < 3; j++) v2[j] = a[j] + c[j];
		normal(v2);
		for (j = 0; j < 3; j++) v3[j] = b[j] + c[j];
		normal(v3);
		divide_triangle(a, v1, v2, m - 1);
		divide_triangle(c, v2, v3, m - 1);
		divide_triangle(b, v3, v1, m - 1);
		divide_triangle(v1, v3, v2, m - 1);
	}
	else(triangle(a, b, c)); /* draw triangle at end of recursion */
}

/* Apply triangle subdivision to faces of tetrahedron */
void tetrahedron(int m)
{
	divide_triangle(v[0], v[1], v[2], m);
	divide_triangle(v[3], v[2], v[1], m);
	divide_triangle(v[0], v[3], v[1], m);
	divide_triangle(v[0], v[2], v[3], m);
}

/* Displays all three modes, side by side */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	myinit();
	mode = 2;
	tetrahedron(n);
	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-4.0, 4.0, -4.0*(GLfloat)h / (GLfloat)w,
			+4.0*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-4.0*(GLfloat)w / (GLfloat)h,
			+4.0*(GLfloat)w / (GLfloat)h, -4.0, 4.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	display();
}

void myinit(void){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 100.0 };

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { x, y, z, 0.0 };

	GLfloat light_diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_specular1[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_position1[] = { a, b, c, 0.0 };

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	/* define material proerties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_SMOOTH);		/*enable smooth shading */
	glEnable(GL_LIGHTING);		/* enable lighting */
	OnOffLight();				/* enable/disable light 0 and light 1*/
	glEnable(GL_DEPTH_TEST);	/* enable z buffer */
	glClearColor(0.0, 0.4, 0.4, 1.0);
	glColor3f(0.0, 0.0, 0.0);
}

void OnOffLight() {
	if (light0 == 1) {
		glEnable(GL_LIGHT0);
	}
	else if (light0 == 0) {
		glDisable(GL_LIGHT0);
	}
	if (light1 == 1) {
		glEnable(GL_LIGHT1);
	}
	else if (light1 == 0) {
		glDisable(GL_LIGHT1);
	}
}

void Info() {
	cout << "USO DE TECLAS" << endl << endl;

	cout << "Fuentes de Luz" << endl << endl;

	cout << "0: Apagar Fuente de Luz 0 (roja)" << endl;
	cout << "1: Encender Fuente de Luz 0 (roja)" << endl;
	cout << "2: Apagar Fuente de Luz 1 (azul)" << endl;
	cout << "3: Encender Fuente de Luz 1 (azul)" << endl << endl;

	cout << "Movimientos de las Fuentes de Luz" << endl;
	cout << "x: Mover Luz 0 -x  y  X: Mover Luz 0 +x" << endl;
	cout << "y: Mover Luz 0 -y  y  Y: Mover Luz 0 +y" << endl;
	cout << "z: Mover Luz 0 -z  y  Z: Mover Luz 0 +z" << endl << endl;

	cout << "a: Mover Luz 1 -x  y  A: Mover Luz 1 +x" << endl;
	cout << "b: Mover Luz 1 -y  y  B: Mover Luz 1 +y" << endl;
	cout << "c: Mover Luz 1 -z  y  C: Mover Luz 1 +z" << endl << endl;

	cout << "ESC: Quit" << endl;
}

void main(int argc, char **argv)
{
	Info();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Grupo 09 - T4");
	myinit();
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(Controls);
	glutDisplayFunc(display);
	glutMainLoop();
	//return 0;
}