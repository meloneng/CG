#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


#include <bits/stdc++.h>
#include "glm.h"

using namespace std;

// Frog position
// x=-2, y=0, z=19
float xpos = -2.0f;
float ypos = 0.0f;
float zpos = 19.0f;
// Model scale
float scale = 0.97;
// Model initial rotation
float rotx = 0.0, roty = 180.0, rotz = 0.0;


// Car positions
vector<float> xcars{0, -13, 15};
vector<float> ycars{0,0,0};
vector<float> zcars{3, -7, -20};


// Definicao dos parametros do modelo de iluminacao
GLfloat light_pos[] = {-2.0, 2.0, 2.0, 0.0};
GLfloat light_Ka[] = {0.4, 0.4, 0.4, 0.0};
GLfloat light_Kd[] = {1.0, 1.0, 1.0, 0.0};
GLfloat light_Ks[] = {1.0, 1.0, 1.0, 0.0};
// Definicao dos parametros do material para o modelo de iluminacao
// Parametros para material amarelado, cor de latao (Brass)
GLfloat material_Ka[] = {0.33, 0.22, 0.03, 1.00};
GLfloat material_Kd[] = {0.78, 0.57, 0.11, 1.00};
GLfloat material_Ks[] = {0.99, 0.94, 0.81, 1.00};
GLfloat material_Ke[] = {0.00, 0.00, 0.00, 0.00};
GLfloat material_Se = 28;

GLManimation *frogModel = NULL;
int faces=0,vertices=0;

GLManimation *carModel = NULL;
int facesCar=0,verticesCar=0;

GLuint modeShade;



void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void arrowKeys(int key, int x, int y);
void shapesToDraw();
void windowFuncs();
void callBackFuncs();
void lightFuncs();
void modelFuncs();
void drawFloor(GLuint mode);
void collision();

int main(int argc, char** argv) {
    glutInit(&argc, argv);          // initialize freeglut
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // set the display mode

    windowFuncs();

    init();

    callBackFuncs();

    lightFuncs();
    
    modelFuncs();

    glutMainLoop();                 // enter the main loop
    return 0;
}


void init(void){
    glClearColor (1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    glEnable(GL_DEPTH_TEST); // Habilita o algoritmo Z-Buffer
}

void shapesToDraw(){
    /*glPushMatrix();
    glTranslatef(xpos, ypos, zpos);                     // sphere starting position
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);             
    glutSolidSphere(1.0f, 20, 20);                       // draw the sphere
    glPopMatrix();
    glPopMatrix();*/
    
    modeShade = (GLM_SMOOTH | GLM_TEXTURE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glScaled(scale, scale, scale);
        glTranslated (xpos, ypos, zpos);
        glRotated (rotx, 1.0f, 0.0f, 0.0f);
        glRotated (roty, 0.0f, 1.0f, 0.0f);
        glRotated (rotz, 0.0f, 0.0f, 1.0f);
        glmDrawAnimation(frogModel, modeShade);
    glPopMatrix();


    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glScaled(scale, scale, scale);
        glTranslated (xcars[0], ycars[0], zcars[0]);
        glRotated (0, 1.0f, 0.0f, 0.0f);
        glRotated (90, 0.0f, 1.0f, 0.0f);
        glRotated (0, 0.0f, 0.0f, 1.0f);
        glmDrawAnimation(carModel, modeShade);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glScaled(scale, scale, scale);
        glTranslated (xcars[1], ycars[1], zcars[1]);
        glRotated (0, 1.0f, 0.0f, 0.0f);
        glRotated (90, 0.0f, 1.0f, 0.0f);
        glRotated (0, 0.0f, 0.0f, 1.0f);
        glmDrawAnimation(carModel, modeShade);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glScaled(scale, scale, scale);
        glTranslated (xcars[2], ycars[2], zcars[2]);
        glRotated (0, 1.0f, 0.0f, 0.0f);
        glRotated (90, 0.0f, 1.0f, 0.0f);
        glRotated (0, 0.0f, 0.0f, 1.0f);
        glmDrawAnimation(carModel, modeShade);
    glPopMatrix();


    drawFloor(modeShade);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the screen
    glMatrixMode(GL_MODELVIEW);                          // switch to the model-view matrix
    glLoadIdentity();                                    // reset the matrix
    shapesToDraw();
    glutSwapBuffers();                                   // swap the front and back buffers to display the image
}

void reshape (int w, int h){
    // muda para o modo GL_PROJECTION e reinicia a matriz de projecao
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    // define o tamanho da area de desenho da janela
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    // Define a forma do volume de visualizacao para termos
    // uma projecao perspectiva (3D).
    gluPerspective(60, (float)w/(float)h , 1.0, 300.0); //(angulo, aspecto, ponto_proximo, ponto distante)
    gluLookAt(0.0,30.0,15.0,  // posicao da camera (olho)
              0.0,0.0,0.0,  // direcao da camera (geralmente para centro da cena)
              0.0,1.0,0.0); // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode (GL_MODELVIEW);

}

void arrowKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            zpos -= 0.3f;
            break;
        case GLUT_KEY_DOWN:
            zpos += 0.3f;
            break;
        case GLUT_KEY_LEFT:
            xpos -= 0.3f;
            break;
        case GLUT_KEY_RIGHT:
            xpos += 0.3f;
            break;
    }
    glutPostRedisplay();  // redraw the scene with the new sphere position
    cout << "x" << xpos << endl << "y" << ypos << endl << "z" << zpos << endl;
    collision();
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
        case 27:
            exit(EXIT_SUCCESS);
            break;
	}
    glutPostRedisplay();
}

void windowFuncs(){
    glutInitWindowSize(800, 600);   // set the window size
    glutCreateWindow("Testing"); // create the window
}

void callBackFuncs(){
    glutDisplayFunc(display);       // set the display callback function
    glutReshapeFunc(reshape);
    glutSpecialFunc(arrowKeys);      // set the special key callback function
    glutKeyboardFunc(keyboard);
}

void lightFuncs(){

     // Ativacao dos parametros do modelo de iluminacao para a Luz 0
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

    // Ativacao dos parametros do material para uso do modelo de iluminacao
    // Para usar este material a iluminacao (GL_LIGHTING) deve estar ativa
    // e a cor do material (GL_COLOR_MATERIAL) desativada
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);

    glEnable(GL_LIGHTING); // Ativa o modelo de iluminacao
    glEnable(GL_LIGHT0); // Ativa a Luz 0. O OpenGL suporta pelo menos 8 pontos de luz.

}

void modelFuncs(){

    // Configurando o OpenGL para o uso de Texturas
    // Define como a textura sera aplicada ao objeto
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // Ativa a visualizacao de texturas 2D (Texturizacao 2D)
    glEnable(GL_TEXTURE_2D);
    
    // Ativa o modelo de sombreamento de "Gouraud" (Smooth
    glShadeModel(GL_SMOOTH);
    
    // Ativa o Back-face Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Ativa o z-buffering, de modo a remover as superficies escondidas
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
        
    //Define a cor de fundo (branco)
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Loading the Frog model
    cout << "Loading frog model" << endl;
    frogModel = glmLoadAnimation("Obj/Tree_frog.obj", 1, 1);
    frogModel->name = "paused";
    faces = frogModel->models[0]->numtriangles;
    vertices = frogModel->models[0]->numvertices;

    // Loading Car model
    cout << "Loading car model" << endl;
    carModel = glmLoadAnimation("cars/PD_VehiclePack_OBJ/Sedan_A_beige.obj", 1,1);
    carModel->name = "paused";
    facesCar = carModel->models[0]->numtriangles;
    verticesCar = carModel->models[0]->numvertices;
}

void drawFloor(GLuint mode){
    glEnable(GL_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    for(int x = -32; x <= 30; x++ ){
        for (int z = -32; z <= 17; z++){
            glPushMatrix();
                glTranslatef(x, 0, z);
                glBegin(GL_TRIANGLES);
                    glNormal3f( 0.0, -1.0,  0.0);
                    glVertex3f( 0.5, -1.0, -0.5);
                    if(mode & GLM_SMOOTH) glNormal3f(0.0, -1.0, 0.0);
                    glVertex3f(-0.5, -1.0, -0.5);
                    if(mode & GLM_SMOOTH) glNormal3f(0.0, 1.0, 0.0);
                    glVertex3f( 0.5, -1.0,  0.5);
                    
                    glNormal3f(0.0, 1.0, 0.0);
                    glVertex3f(-0.5, -1.0,  0.5);
                    if(mode & GLM_SMOOTH)glNormal3f(0.0, 1.0, 0.0);
                    glVertex3f( 0.5, -1.0,  0.5);
                    if(mode & GLM_SMOOTH) glNormal3f(0.0, -1.0,  0.0);
                    glVertex3f(-0.5, -1.0, -0.5);
                glEnd();
            glPopMatrix();
        }
    }
    glEnable(GL_TEXTURE_2D);
}

void collision(){
    
    for(int i=0;i<(xcars.size());i++){
        if(abs(xpos + ((-1)*xcars[i])) < 3.5){
            if(abs((zpos-4.0) + ((-1)*zcars[i])) < 2.5){
                cout << "collision with " << i << endl;
                //exit(0);
            }
        }
    }
}
