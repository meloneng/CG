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


#define POSY -1.04  //posicao do modelo 3D no eixo y
#define CAMZ 5.0    //posicao da camera no eixo z

bool use_texture = true; // Determina se vamos usar texturas
bool use_color_material = false; // Determina se vamos usar a glColor como parametro do material
bool use_depth_test = true; // Determina se vamos usar Z-buffering
bool use_light = true; // Determina se liga a luz
bool use_gouraud = true; // Determina o uso de Gouraud ou Flat shading
bool render_solid = true; // Determina se renderiza o objeto solido ou em wireframe

GLint apply_texture = GL_MODULATE; //Determina como a textura sera aplicada no objeto
std::string apply_texture_name = "Modulate"; //nome da fucao que sera usada para aplicar a textura no objeto

unsigned int id_texturas[1]; //Matriz para armazenar os identificadores de textura


float graus = 15.0, deslocamento = 0.2; //incrementos do angulo de graus e do deslocamento
float scale = 0.97; // fator de escala proporcional em todos os eixos do modelo 3D
float rotx = 0.0, roty = 0.0, rotz = 0.0; // angulo de graus do modelo 3D no eixo x, y e z
float posx = 0.0, posy = POSY, posz = 0.0; // deslocamento do modelo 3D no eixo x, y e z
float R = 1.0, G = 1.0, B = 1.0; //Cores do desenho controladas pelo glColor (inicial: cor branca)
float camx = 0.0, camy = 0.0, camz = CAMZ; // posicao da camera no eixo x, y e z
float cam_rotx = 0.0, cam_roty = 0.0; // angulo de graus da camera no eixo x e y
float cam_up = 1.0; //direcao do vetor para cima da camera


/*
 *  Definicao dos parametros do modelo de iluminacao
 */
GLfloat light_pos[] = {-2.0, 2.0, 2.0, 0.0};
GLfloat light_Ka[] = {0.4, 0.4, 0.4, 0.0};
GLfloat light_Kd[] = {1.0, 1.0, 1.0, 0.0};
GLfloat light_Ks[] = {1.0, 1.0, 1.0, 0.0};

/*
 *  Definicao dos parametros do material para o modelo de iluminacao
 */
GLfloat material_Ka[] = {0.83, 0.02, 0.13, 1.00};
GLfloat material_Kd[] = {0.78, 0.37, 0.11, 1.00};
GLfloat material_Ks[] = {0.39, 0.94, 0.21, 1.00};
GLfloat material_Ke[] = {0.00, 0.00, 0.00, 0.00};
GLfloat material_Se = 28;


GLManimation *frogModel = NULL;
int faces=0,vertices=0;
GLuint modeShade;


void init(void);
void display(void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void arrowKeys(int key, int x, int y);
//void shapesToDraw();
void windowFuncs();
void callBackFuncs();
void lightFuncs();
void modelFuncs();
void texFuncs();
void keyboard_special(int key, int x, int y);



int main(int argc, char** argv) {
    glutInit(&argc, argv);          // initialize freeglut
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // set the display mode
    windowFuncs();

    init();

    callBackFuncs();

    lightFuncs();

    texFuncs();
    
    modelFuncs();

    glutMainLoop();                 // enter the main loop
    return 0;
}




void init(void){
    glClearColor (1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    glEnable(GL_DEPTH_TEST); // Habilita o algoritmo Z-Buffer
}

void windowFuncs(){
    glutInitWindowSize(800, 600);   // set the window size
    glutCreateWindow("Testing"); // create the window
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
        case 27:
            exit(EXIT_SUCCESS);
            break;
	}
    glutPostRedisplay();
}

void callBackFuncs(){
    glutDisplayFunc(display);       // set the display callback function
    glutReshapeFunc(reshape);
    //glutSpecialFunc(arrowKeys);      // set the special key callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);
}

void display() {
    //glMatrixMode(GL_MODELVIEW);                          // switch to the model-view matrix
    //glLoadIdentity();                                    // reset the matrix

    // Apaga o video e o depth buffer, e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Ativa/Desativa a Texturizacao 2D (Visualizacao de Texturas 2D)
    if(use_texture) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);

    // Liga/Desliga a luz
    if(use_light) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);

    //Define se desenha usando sombreamento Gouraud ou Flat
    GLuint mode;
    if(use_gouraud) mode = (GLM_SMOOTH | GLM_TEXTURE);
    else mode = (GLM_FLAT  | GLM_TEXTURE);

    glColor3f(R, G, B);

    glPushMatrix();
        glScaled(scale, scale, scale);
        glTranslated (posx, posy, posz);
        glRotated (rotx, 1.0f, 0.0f, 0.0f);
        glRotated (roty, 0.0f, 1.0f, 0.0f);
        glRotated (rotz, 0.0f, 0.0f, 1.0f);
        glmDrawAnimation(frogModel, mode);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D); //Desativa a Texturizacao 2D para desenhar o texto
    glDisable(GL_LIGHTING); // Desliga a Luz para desenhar o texto

    //shapesToDraw();
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
    gluLookAt(0.0,5.0,5.0,  // posicao da camera (olho)
              0.0,0.8,0.0,  // direcao da camera (geralmente para centro da cena)
              0.0,1.0,0.0); // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode (GL_MODELVIEW);

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

void texFuncs(){
    /*
    glGenTextures(1, id_texturas);
    
    //Ativa a textura que sera criada ou usada no momento, por meio do seu ID.
    glBindTexture(GL_TEXTURE_2D, id_texturas[0]);
    
    //Cria a extrutura da textura na mem�ria
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    
    //Definicao dos parametros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    //Define como a textura sera aplicada ao objeto
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, apply_texture);

    //Ativa a visualizacao de texturas 2D (Texturizacao 2D)
    glEnable(GL_TEXTURE_2D);
    
    // Ativa o modelo de sombreamento de "Gouraud" (Smooth
    glShadeModel( GL_SMOOTH );
    // Ativa o z-buffering, de modo a remover as superficies escondidas
    glEnable(GL_DEPTH_TEST);
    // Define a cor de fundo (branco)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    */


    // Configurando o OpenGL para o uso de Texturas
    //Define como a textura sera aplicada ao objeto
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, apply_texture);

    //Ativa a visualizacao de texturas 2D (Texturizacao 2D)
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


}

void modelFuncs(){
    // Loading the Frog model
    cout << "Loading frog model" << endl;

    frogModel = glmLoadAnimation("models/ironman/ironman-T-pose.obj", 1, 1);
    frogModel->name = "paused";
    faces = frogModel->models[0]->numtriangles;
    vertices = frogModel->models[0]->numvertices;
    
    printf(" done.\n");
}

void keyboard_special(int key, int x, int y){
    switch(key){
        
        case GLUT_KEY_F2: //Seleciona rederizacao solida ou em wireframe
            if (render_solid) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            render_solid = !render_solid;
        break;
        case GLUT_KEY_F3: //Liga/desliga o color material
            if (use_color_material){
                glDisable(GL_COLOR_MATERIAL);
                
                //Redefine os parametros do material para os valores iniciais, que estavam rastreando o glColor
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
            }else glEnable(GL_COLOR_MATERIAL);
            use_color_material = !use_color_material;
        break;

        case GLUT_KEY_F4: //Alterar a cor de desenho
            if (R && G && B) { R = 0.0; G = 0.0; B = 1.0;} //Muda a cor para azul
            else if(B){ R = 0.0; G = 1.0; B = 0.0;} //Muda a cor para verde
            else if (G){ R = 1.0; G = 0.0; B = 0.0;} //Muda a cor para vermelho
            else { R = 1.0; G = 1.0; B = 1.0;} //Muda a cor para branco
        break;

        case GLUT_KEY_F5: use_gouraud = !use_gouraud; break; //Liga/desliga o gouraud shading
            
        case GLUT_KEY_F6: //Liga/desliga o Z-buffering
            if (use_depth_test) glDisable(GL_DEPTH_TEST);
            else glEnable(GL_DEPTH_TEST);
            use_depth_test = !use_depth_test;
        break;
            
        case GLUT_KEY_F7: use_light = !use_light; break; //Liga/desliga a luz
        
        case GLUT_KEY_F8: use_texture = !use_texture; break; //Liga/desliga a visualizacao da textura
        
        case GLUT_KEY_F9: //Troca como a textura sera aplicada no objeto
            switch (apply_texture) {
                case GL_ADD:
                    apply_texture = GL_DECAL;
                    apply_texture_name = "Decal";
                break;
                case GL_MODULATE:
                    apply_texture = GL_BLEND;
                    apply_texture_name = "Blend";
                break;
                case GL_BLEND:
                    apply_texture = GL_ADD;
                    apply_texture_name = "Add";
                break;
                case GL_DECAL:
                    apply_texture = GL_MODULATE;
                    apply_texture_name = "Modulate";
                break;
                //Funcoes usadas para combinar mais de uma texturas
//                case GL_COMBINE: //Uso sem combinar tem o mesmo efeito do GL_MODULATE
//                break;
//                case GL_REPLACE: //Uso sem combinar tem o mesmo efeito do GL_DECAL
//                break;
            }
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, apply_texture);
        break;
            
   
    }
}
