/*
 * Computacao Grafica
 * Codigo Exemplo: Primeira Janela GLUT/OpenGL
 * Autor: Prof. Laurindo de Sousa Britto Neto
 */
 
// Bibliotecas utilizadas pelo OpenGL
#ifdef __APPLE__ // MacOS
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else // Windows e Linux
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <cstdlib>

#include "circulo.h"
#define ESC 27

//Variaveis Globais usadas para definicao de cores
float R,G,B;


// Declaracoes antecipadas (forward) de funcoes para as funcoes callback do OpenGL
void init(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void display(void);

/*
 * Funcao principal
 */
int main(int argc, char** argv){
    glutInit(&argc, argv); // Passagens de parametros C para o glut
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize (512, 512);  // Tamanho da janela do OpenGL
    glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow ("Computcao Grafica: Primeira Janela GLUT/OpenGL"); // Da nome para uma janela OpenGL
    init(); // Chama a funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutDisplayFunc(display); //funcao callback de desenho
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutSpecialFunc(keyboard_special);	//funcao callback do teclado especial
    glutMainLoop(); // executa o loop do OpenGL
    return EXIT_SUCCESS; // retorna 0 para o tipo inteiro da funcao main()
}

/*
 * Inicializa alguns parametros do GLUT
 */
void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    R = 0.0;
    G = 0.0;
    B = 0.0;
}


/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
	// Reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    // Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);

    glOrtho (0, 512, 0, 512, -1 ,1);  // Define o volume de projecao ortografica;
                                      // Nesse caso, o ponto (0,0,0) se encontra no
                                      // canto inferior esquerdo da tela e o viewport
                                      // tem 0 a 512 pixel no eixo x, 0 a 512 pixel no
                                      // eixo y. Como sera trabalhado imagens 2D definimos
                                      // o eixo z com tamanho -1 a 1;

//   glOrtho (-256, 256, -256, 256, -1 ,1); // Nesse caso, continuamos com 512 pixels, porem o
                                            // centro dos eixos passa a se localizar no centro
                                            // da tela.

//   glOrtho (-(w/2), (w/2),-(h/2),(h/2), -1 ,1);   // Nesse caso, ficamos com a quantidade de pixels variavel,
                                                    // dependente da largura e altura da janela, e o
                                                    // centro dos eixos continua a se localizar no centro
                                                    // da tela.
    
   // muda para o modo GL_MODELVIEW (nao pretendemos alterar a projecao
   // quando estivermos desenhando na tela)
	glMatrixMode(GL_MODELVIEW);
}

/*
 * Funcao usada na funcao callback para controlar as teclas comuns (1 Byte) do teclado
 */
void keyboard(unsigned char key, int x, int y){
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
        case ESC: exit(EXIT_SUCCESS); break; //encerra o programa com sucesso
    }
}

/*
 *Funcao usada na fucao callback para controlar as teclas especiais (2 Byte) do teclado
 */
void keyboard_special(int key, int x, int y){
    switch(key){
      case GLUT_KEY_F1: R=0.0; G=0.0; B=1.0; break; // F1: muda a cor para azul
      case GLUT_KEY_F2: R=0.0; G=1.0; B=0.0; break; // F2: muda a cor para verde
      case GLUT_KEY_F3: R=1.0; G=0.0; B=0.0; break; // F3: muda a cor para vermelho
      default: break;
    }
    glutPostRedisplay();
}

/*
 * Funcao usada na funcao callback para desenhar na tela
 */
void display(void){
	//Limpa o Buffer de Cores e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();

    // Transformacoes usadas no triangulo com vertice esquerdo na origem dos eixos cartesianos
    glColor3f(R, G, B);
	//glScalef(0.5, 0.5, 0);
	//glTranslatef(200,200, 0);
	//glRotatef(180, 0, 0, 1);


	/*glBegin(GL_TRIANGLES);
        // Coordenadas com vertice esquerdo do triangulo na origem dos eixos cartesianos
		glVertex2f(100, 200);
		glVertex2f(0, 0);
		glVertex2f(200, 0);

        // Coordenadas com centro do triangulo na origem dos eixo cartesianos
        //glVertex2f(0, 100);
        //glVertex2f(-100, -100);
        //glVertex2f(100, -100);

    glEnd();*/


    // Funcao criada para desenhar circulos
    // desenhaCirculo(100,30,true);

    // Cart
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(30, 100);
        glVertex2f(200, 100);
        glVertex2f(30, 160);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(200, 100);
        glVertex2f(30, 160);
        glVertex2f(200, 160);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(30, 160);
        glVertex2f(200, 160);
        glVertex2f(115, 190);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= 20; i++){
		glVertex2f(
			-0.7f + (0.2f * cos(i * (2.0f * 3.14f) / 20)),
			-0.1f + (0.2f *sin(i * (2.0f * 3.14f) / 20))
			);
	}
	glEnd();
    
    // Head
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(230, 220);
        glVertex2f(230, 160);
        glVertex2f(330, 160);
        glVertex2f(330, 220);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex2f(235, 215);
        glVertex2f(235, 165);
        glVertex2f(325, 165);
        glVertex2f(325, 215);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(230, 100);
        glVertex2f(230, 160);
        glVertex2f(330, 160);
        glVertex2f(330, 100);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(330, 100);
        glVertex2f(330, 160);
        glVertex2f(430, 160);
        glVertex2f(430, 100);        
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(225, 220);
        glVertex2f(225, 223);
        glVertex2f(335, 223);
        glVertex2f(335, 220);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(375, 160);
        glVertex2f(375, 223);
        glVertex2f(405, 223);
        glVertex2f(405, 160);
    glEnd();
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(370, 220);
        glVertex2f(370, 223);
        glVertex2f(410, 223);
        glVertex2f(410, 220);
    glEnd();



    // Cart wheels
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(50, 80, 0);
    desenhaCirculo(20,30,true);
    glTranslatef(130, 0, 0);
    desenhaCirculo(20,30,true);

    

    glFlush(); // manda o OpenGl renderizar as primitivas

}

