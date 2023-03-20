/*
 * Computacao Grafica
 * Codigo Exemplo: Rasterizacao de Segmentos de Reta com GLUT/OpenGL
 * Autor: Prof. Laurindo de Sousa Britto Neto
 */

// Bibliotecas utilizadas pelo OpenGL
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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <forward_list>
#include <bits/stdc++.h>
#include "glut_text.h"
#include "CG_ops.hpp"

using namespace std;

// Variaveis Globais
#define ESC 27

//Enumeracao com os tipos de formas geometricas
enum tipo_forma{LIN = 1, TRI, RET, POL, CIR }; // Linha, Triangulo, Retangulo Poligono, Circulo

//Verifica se foi realizado o primeiro clique do mouse
bool click1 = false;

//Verifica se foi realizado o segundo clique do mouse
bool click2 = false;

//Coordenadas da posicao atual do mouse
int m_x, m_y;

//Coordenadas do primeiro clique e do segundo clique do mouse
int x_1, y_1, x_2, y_2;

//Indica o tipo de forma geometrica ativa para desenhar
int modo = LIN;

//Largura e altura da janela
int width = 512, height = 512;

// Definicao das formas geometricas
struct forma{
    int tipo;
    forward_list<vertice> v; //lista encadeada de vertices
};

// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo){
    forma f;
    f.tipo = tipo;
    formas.push_front(f);
}

vector<vertice> verticesAll;

// Funcao para armazenar um vertice na forma do inicio da lista de formas geometricas
// Armazena sempre no inicio da lista
void pushVertice(int x, int y){
    vertice v;
    v.x = x;
    v.y = y;
    formas.front().v.push_front(v);
}

//Fucao para armazenar uma Linha na lista de formas geometricas
void pushLinha(int x1, int y1, int x2, int y2){
    pushForma(LIN);
    pushVertice(x1, y1);
    pushVertice(x2, y2);
}

// Adiciona triangulos à lista
void pushTri(vertice v1, vertice v2, vertice v3){
    pushForma(TRI);
    pushVertice(v1.x, v1.y);
    pushVertice(v2.x, v2.y);
    pushVertice(v3.x, v3.y);
}

// Adiciona quadriláteros à lista
void pushRET(vertice v1, vertice v2){
    pushForma(RET);
    pushVertice(v1.x, v1.y);
    pushVertice(v2.x, v2.y);
}

// Adiciona quadriláteros à lista
void pushCIR(vertice v1, vertice v2){
    pushForma(CIR);
    pushVertice(v1.x, v1.y);
    pushVertice(v2.x, v2.y);
}

void pushPOL(vector<vertice> v){
    pushForma(POL);
    for(auto i: v){
        pushVertice(i.x, i.y);
    }
}

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas das funcoes)
 */
void init(void);
void reshape(int w, int h);
void display(void);
void menu_popup(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mousePassiveMotion(int x, int y);
void drawPixel(int x, int y);
// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();
// Funcao que implementa o Algoritmo Imediato para rasterizacao de segmentos de retas
void retaImediata(double x1,double y1,double x2,double y2);
//------------------------------------------------------------------------------------


/*
 * Funcao principal
 */
int main(int argc, char** argv){
    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //Selecao do Modo do Display e do Sistema de cor
    glutInitWindowSize (width, height);  // Tamanho da janela do OpenGL
    glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow ("Computacao Grafica: Paint"); // Da nome para uma janela OpenGL
    init(); // Chama funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutMouseFunc(mouse); //funcao callback do mouse
    glutPassiveMotionFunc(mousePassiveMotion); //fucao callback do movimento passivo do mouse
    glutDisplayFunc(display); //funcao callback de desenho
    
    // Define o menu pop-up
    glutCreateMenu(menu_popup);
    glutAddMenuEntry("Linha", LIN);
    glutAddMenuEntry("Triangulo", TRI);
    glutAddMenuEntry("Retangulo", RET);
    glutAddMenuEntry("Circulo", CIR);
    glutAddMenuEntry("Poligono", POL);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    
    glutMainLoop(); // executa o loop do OpenGL
    return EXIT_SUCCESS; // retorna 0 para o tipo inteiro da funcao main();
}

/*
 * Inicializa alguns parametros do GLUT
 */
void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
	// Muda para o modo de projecao e reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);
	
	width = w;
	height = h;
    glOrtho (0, w, 0, h, -1 ,1);  

   // muda para o modo de desenho
	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();

}

/*
 * Controla os desenhos na tela
 */
void display(void){
    glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer de cores e reinicia a matriz
    glColor3f (0.0, 0.0, 0.0); // Seleciona a cor default como preto
    drawFormas(); // Desenha as formas geometricas da lista
    //Desenha texto com as coordenadas da posicao do mouse
    draw_text_stroke(0, 0, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.2);
    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas

}

/*
 * Controla o menu pop-up
 */
void menu_popup(int value){
    if (value == 0) exit(EXIT_SUCCESS);
    modo = value;
}


/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y){
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
        case ESC: exit(EXIT_SUCCESS); break;
    }
}

/*
 * Controle dos botoes do mouse
 */
void mouse(int button, int state, int x, int y){
    switch (button) {
        case GLUT_LEFT_BUTTON:
            switch(modo){
                case LIN:
                {

                
                    if (state == GLUT_DOWN) {
                        if(click1){
                            x_2 = x;
                            y_2 = height - y - 1;
                            printf("Clique 2(%d, %d)\n",x_2,y_2);
                            pushLinha(x_1, y_1, x_2, y_2);
                            click1 = false;
                            glutPostRedisplay();
                        }else{
                            click1 = true;
                            x_1 = x;
                            y_1 = height - y - 1;
                            printf("Clique 1(%d, %d)\n",x_1,y_1);
                        }
                    }
                }
                break;
                case TRI:
                {
                    if (state == GLUT_DOWN) {
                        if(click1 && click2){
                            click1 = false;
                            click2 = false;
                            vertice vtc3;
                            vtc3.x = x;
                            vtc3.y = height - y - 1;
                            printf("Clique 3(%d, %d)\n",vtc3.x,vtc3.y);
                            verticesAll.push_back(vtc3);
                            pushTri(verticesAll[0], verticesAll[1], verticesAll[2]);
                            verticesAll.clear();
                            glutPostRedisplay();
                        }
                        else if(click1){
                            vertice vtc2;
                            vtc2.x = x;
                            vtc2.y = height - y - 1;
                            printf("Clique 2(%d, %d)\n",vtc2.x,vtc2.y);
                            verticesAll.push_back(vtc2);
                            click2 = true;
                        }else{
                            vertice vtc1;
                            vtc1.x = x;
                            vtc1.y = height - y - 1;
                            printf("Clique 1(%d, %d)\n",vtc1.x,vtc1.y);
                            verticesAll.push_back(vtc1);
                            click1 = true;
                        }
                    }
                }
                break;
                case RET:
                {
                    if (state == GLUT_DOWN) {
                        if(click1){
                            click1 = false;
                            vertice vtc2;
                            vtc2.x = x;
                            vtc2.y = height - y - 1;
                            printf("Clique 1(%d, %d)\n",x_1,y_1);
                            verticesAll.push_back(vtc2);
                            pushRET(verticesAll[0], verticesAll[1]);
                            verticesAll.clear();
                        }else{
                            vertice vtc1;
                            vtc1.x = x;
                            vtc1.y = height - y - 1;
                            printf("Clique 1(%d, %d)\n",x_1,y_1);
                            verticesAll.push_back(vtc1);
                            click1 = true;
                        }
                    }
                }
                break;
                case CIR:
                {
                    if (state == GLUT_DOWN) {
                        if(click1){
                            click1 = false;
                            vertice vtc2;
                            vtc2.x = x;
                            vtc2.y = height - y - 1;
                            printf("Clique 2(%d, %d)\n",x_1,y_1);
                            verticesAll.push_back(vtc2);
                            pushCIR(verticesAll[1], verticesAll[0]);
                            verticesAll.clear();
                        }else{
                            vertice vtc1;
                            vtc1.x = x;
                            vtc1.y = height - y - 1;
                            printf("Clique 1(%d, %d)\n",x_1,y_1);
                            verticesAll.push_back(vtc1);
                            click1 = true;
                        }
                    }
                }
                break;
                case POL:
                {
                    if (state == GLUT_DOWN) {
                        vertice vtc;
                        vtc.x = x;
                        vtc.y = height - y - 1;
                        printf("Clique (%d, %d)\n",x_1,y_1);
                        verticesAll.push_back(vtc);
                        click1 = true;
                        
                    }
                }
                break;
            }
        break;

//        case GLUT_MIDDLE_BUTTON:
//            if (state == GLUT_DOWN) {
//                glutPostRedisplay();
//            }
//        break;
//
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
                pushPOL(verticesAll);
                verticesAll.clear();
            }
        break;
            
    }
}

/*
 * Controle da posicao do cursor do mouse
 */
void mousePassiveMotion(int x, int y){
    m_x = x; m_y = height - y - 1;
    glutPostRedisplay();
}

/*
 * Funcao para desenhar apenas um pixel na tela
 */
void drawPixel(int x, int y){
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
        glVertex2i(x, y);
    glEnd();  // indica o fim do ponto
}

/*
 *Funcao que desenha a lista de formas geometricas
 */
void drawFormas(){
    //Apos o primeiro clique, desenha a reta com a posicao atual do mouse
    //if(click1) retaImediata(x_1, y_1, m_x, m_y);
    if(click1){
        vertice vtc0, mousepos;
        vtc0.x = x_1;
        vtc0.y = y_1;
        mousepos.x = m_x;
        mousepos.y = m_y;
        vector<vertice> lineMouse = bresenham(vtc0, mousepos);
        for(auto i: lineMouse){
            drawPixel(i.x,i.y);
        }
    }
    
    //Percorre a lista de formas geometricas para desenhar
    for(forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++){
        switch (f->tipo) {
            case LIN:
            {
                int i = 0, x[2], y[2];
                //Percorre a lista de vertices da forma linha para desenhar
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                    x[i] = v->x;
                    y[i] = v->y;
                }
                //Desenha o segmento de reta apos dois cliques
                //retaImediata(x[0], y[0], x[1], y[1]);

                // Bresenham
                vertice vtc1, vtc2;
                vtc1.x = x[0];
                vtc1.y = y[0];
                vtc2.x = x[1];
                vtc2.y = y[1];
                vector<vertice> line = bresenham(vtc1, vtc2);
                for(auto i: line){
                    drawPixel(i.x,i.y);
                }
            }
            break;
            case TRI:
            {
                vector<vertice> auxl;
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                    vertice auxv;
                    auxv.x = v->x;
                    auxv.y = v->y;
                    auxl.push_back(auxv);
                }

                vector<vertice> line1 = bresenham(auxl[0], auxl[1]);
                vector<vertice> line2 = bresenham(auxl[1], auxl[2]);
                vector<vertice> line3 = bresenham(auxl[2], auxl[0]);

                for(auto i: line1){
                    drawPixel(i.x,i.y);
                }
                for(auto i: line2){
                    drawPixel(i.x,i.y);
                }
                for(auto i: line3){
                    drawPixel(i.x,i.y);
                }
            }
            break;
            case RET:
            {
                vector<vertice> auxl;
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                    vertice auxv;
                    auxv.x = v->x;
                    auxv.y = v->y;
                    auxl.push_back(auxv);
                }

                vertice vertice0 = {auxl[0].x, auxl[0].y};
                vertice vertice1 = {auxl[0].x, auxl[1].y};
                vertice vertice2 = {auxl[1].x, auxl[1].y};
                vertice vertice3 = {auxl[1].x, auxl[0].y};

                vector<vertice> line0 = bresenham(vertice0, vertice1);
                vector<vertice> line1 = bresenham(vertice1, vertice2);
                vector<vertice> line2 = bresenham(vertice2, vertice3);
                vector<vertice> line3 = bresenham(vertice3, vertice0);

                for(auto i: line0){
                    drawPixel(i.x,i.y);
                }
                for(auto i: line1){
                    drawPixel(i.x,i.y);
                }
                for(auto i: line2){
                    drawPixel(i.x,i.y);
                }
                for(auto i: line3){
                    drawPixel(i.x,i.y);
                }
            }
            break;
            case CIR:
            {
                vector<vertice> auxl;
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                    vertice auxv;
                    auxv.x = v->x;
                    auxv.y = v->y;
                    auxl.push_back(auxv);
                }

                vertice center = {auxl[0].x, auxl[0].y};
                int radius = abs(auxl[1].x - auxl[0].x);
                
                vector<vertice> circ = bresenhamCirc(center,radius);

                for(auto i: circ){
                    drawPixel(i.x,i.y);
                }
                
            }
            break;
            case POL:
            {
                vector<vertice> auxl;
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                    vertice auxv;
                    auxv.x = v->x;
                    auxv.y = v->y;
                    auxl.push_back(auxv);
                }
                
                for(int i=1 ; i<auxl.size() ; i++){
                    if(i == (auxl.size() -1)){
                        vector<vertice> line = bresenham(auxl[i],auxl[0]);
                        for(auto i: line){
                            drawPixel(i.x,i.y);
                        }
                        line = bresenham(auxl[i-1],auxl[i]);
                        for(auto i: line){
                            drawPixel(i.x,i.y);
                        }
                    }else{
                        vector<vertice> line = bresenham(auxl[i-1],auxl[i]);
                        for(auto i: line){
                            drawPixel(i.x,i.y);
                        }
                    }
                }
            }
            break;
        }
    }
}

/*
 * Funcao que implementa o Algoritmo de Rasterizacao da Reta Imediata
 */
void retaImediata(double x1, double y1, double x2, double y2){
    double m, b, yd, xd;
    double xmin, xmax,ymin,ymax;
    
    drawPixel((int)x1,(int)y1);
    if(x2-x1 != 0){ //Evita a divisao por zero
        m = (y2-y1)/(x2-x1);
        b = y1 - (m*x1);

        if(m>=-1 && m <= 1){ // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
            xmin = (x1 < x2)? x1 : x2;
            xmax = (x1 > x2)? x1 : x2;

            for(int x = (int)xmin+1; x < xmax; x++){
                yd = (m*x)+b;
                yd = floor(0.5+yd);
                drawPixel(x,(int)yd);
            }
        }else{ // Se tg menor que -1 ou maior que 1, calcula incrementado os valores de y
            ymin = (y1 < y2)? y1 : y2;
            ymax = (y1 > y2)? y1 : y2;

            for(int y = (int)ymin + 1; y < ymax; y++){
                xd = (y - b)/m;
                xd = floor(0.5+xd);
                drawPixel((int)xd,y);
            }
        }

    }else{ // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2)? y1 : y2;
        ymax = (y1 > y2)? y1 : y2;
        for(int y = (int)ymin + 1; y < ymax; y++){
            drawPixel((int)x1,y);
        }
    }
    drawPixel((int)x2,(int)y2);
}
