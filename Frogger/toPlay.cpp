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

using namespace std;

// Sphere position
float xpos = 0.0f;
float ypos = 0.0f;
float zpos = 0.0f;


void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void arrowKeys(int key, int x, int y);
void shapesToDraw();


int main(int argc, char** argv) {
    glutInit(&argc, argv);          // initialize freeglut
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // set the display mode
    glutInitWindowSize(800, 600);   // set the window size
    glutCreateWindow("Sphere Example"); // create the window

    init();

    glutDisplayFunc(display);       // set the display callback function
    glutReshapeFunc(reshape);
    glutSpecialFunc(arrowKeys);      // set the special key callback function
    glutKeyboardFunc(keyboard);
    glutMainLoop();                 // enter the main loop
    return 0;
}


void init(void){
    glClearColor (1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    glEnable(GL_DEPTH_TEST); // Habilita o algoritmo Z-Buffer
}

void shapesToDraw(){
    glPushMatrix();
    glTranslatef(xpos, ypos, zpos);                     // sphere starting position
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);                         
    glutWireSphere(1.0f, 20, 20);                       // draw the sphere
    glPopMatrix();
    glPopMatrix();
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
    gluPerspective(60, (float)w/(float)h , 1.0, 30.0); //(angulo, aspecto, ponto_proximo, ponto distante)
    gluLookAt(0.0,-7.0,15.0,  // posicao da camera (olho)
              0.0,1.0,0.0,  // direcao da camera (geralmente para centro da cena)
              0.0,1.0,0.0); // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode (GL_MODELVIEW);

}

void arrowKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            ypos += 0.2f;
            break;
        case GLUT_KEY_DOWN:
            ypos -= 0.2f;
            break;
        case GLUT_KEY_LEFT:
            xpos -= 0.2f;
            break;
        case GLUT_KEY_RIGHT:
            xpos += 0.2f;
            break;
    }
    glutPostRedisplay();  // redraw the scene with the new sphere position
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
	}
    glutPostRedisplay();
}

