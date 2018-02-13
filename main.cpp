
/*
 Name= Swati Kajal
 Student Id=109891879
 Course=272
 Project Name=wavefront model
 key m,n is given zoom in and out
 */

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//#include<SOIL.h>

#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <fstream>
#include <sstream>

#define PI 3.14159

using namespace std;

bool WireFrame = false;
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

typedef struct {
    float x;
    float y;
    float z;
} vec3;

typedef struct {
    int v1;
    int n1;
    int v2;
    int n2;
    int v3;
    int n3;
} GLindices;


vec3 *ver,  *nor;
GLindices* ind;
int count_ver = 0;
int count_nor = 0;
int count_ind = 0;
int factor = 200;
double zoom = -0.5;

double rotatex = 0;
double rotatey = 0;
GLuint tex;


/* GLUT callback Handlers */

static void resize(int width, int height)
{
    
    double Ratio;
    
    if (width<=height) {
        glViewport(0, (GLsizei) (height-width)/2, (GLsizei) width, (GLsizei) width);
        Ratio= height/width;
    }
    else {
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
        Ratio = width /height;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,Ratio,0.1f, 100.0f);
}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,0.0);
    
    if (WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Toggle WIRE FRAME
    
    glTranslatef(0,0,zoom);
    glRotated(rotatex, 1, 0, 0);
    glRotated(rotatey, 0, 1, 0);
    glPushMatrix();
    glTranslatef(-0.5,-0.5,-0.5);
    for (int i = 0; i < count_ind; i++) {
        glBegin(GL_TRIANGLES);
        
        glNormal3f(nor[ind[i].n1 - 1].x, nor[ind[i].n1 - 1].y, nor[ind[i].n1 - 1].z);
        glVertex3f(ver [ind[i].v1 - 1].x, ver [ind[i].v1 - 1].y, ver [ind[i].v1 - 1].z);
        
        glNormal3f(nor[ind[i].n2 - 1].x, nor[ind[i].n2 - 1].y, nor[ind[i].n2 - 1].z);
        glVertex3f(ver [ind[i].v2 - 1].x, ver [ind[i].v2 - 1].y, ver [ind[i].v2 - 1].z);
        
        glNormal3f(nor[ind[i].n3 - 1].x, nor[ind[i].n3 - 1].y, nor[ind[i].n3 - 1].z);
        glVertex3f(ver [ind[i].v3 - 1].x, ver [ind[i].v3 - 1].y, ver [ind[i].v3 - 1].z);
        glEnd();
    }
    
    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
        case 'q':
            exit(0);
            break;
            
        case 'w':
            WireFrame = !WireFrame;
            break;
        case 'm':
            
            zoom += 0.5;     //zoom in
            break;
        case 'n':
            
            zoom -= 0.5;     //zoom out
            break;
    }
    glutPostRedisplay();
}

void Specialkeys(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT:
            
            rotatey = rotatey + 1;
            break;
            
        case GLUT_KEY_RIGHT:
            
            rotatey = rotatey - 1;
            break;
            
        case GLUT_KEY_UP:
            
            rotatex = rotatex + 1;
            break;
            
        case GLUT_KEY_DOWN:
            
            rotatex = rotatex - 1;
            break;
    }
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    ifstream File("bunny.obj");
    string line;
    string str;
       ver = (vec3*)malloc( sizeof(vec3));
    
    nor = (vec3*)malloc(sizeof(vec3));
    
    while (getline(File, line)) {
        if (line == "" || line[0] == '#')
            continue;
        istringstream LineStream(line);
        LineStream >> str;
        
        if (str == "v") {
            float arr[3];
            sscanf(line.c_str(), "%*s %f %f %f", &arr[0], &arr[1], &arr[2]);
            ver= (vec3*)realloc(ver, (count_ver + 1) * sizeof(vec3));
            ver[count_ver].x = arr[0] / factor;
            ver[count_ver].y = arr[1] / factor;
            ver[count_ver].z = arr[2] / factor;
            count_ver++;
        } else if (str == "vn") {
            float arr[3];
            sscanf(line.c_str(), "%*s %f %f %f", &arr[0], &arr[1], &arr[2]);
            nor = (vec3*)realloc(nor, (count_nor + 1) * sizeof(vec3));
            nor[count_nor].x = arr[0];
            nor[count_nor].y = arr[1];
            nor[count_nor].z = arr[2];
            count_nor++;
        }else if (str == "f") {
            int arr[9];
            sscanf(line.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d", &arr[0],
                   &arr[1], &arr[2], &arr[3], &arr[4], &arr[5], &arr[6], &arr[7], &arr[8]);
            ind = (GLindices*)realloc(ind, (count_ind + 1) * sizeof(GLindices));
            ind[count_ind].v1 = arr[0];
            ind[count_ind].n1 = arr[2];
            ind[count_ind].v2 = arr[3];
            ind[count_ind].n2 = arr[5];
            ind[count_ind].v3 = arr[6];
            ind[count_ind].n3 = arr[8];
            count_ind++;
            
        }
        }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D); //Adding texture to the bunny.
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height; // width & height for the image reader
    unsigned char* img = NULL;
    
    // img = SOIL_load_image("wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if(img == NULL){
        cout<<"Image not taken";
    }
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    // binding image data
    //    SOIL_free_image_data(img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/* Program entrotatey point */

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Project Assignment 5");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}

