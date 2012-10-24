#include "util.h"


bool quit = false;
SDL_Event event;


/**********************PROJETO DE PG 2012****************************************
Autores: Ermano Arruda(eaa3), Guilherme Reis(grrbm), Antonio Marino(amng), Fernando Sotero(lfss2)

[TECLAS]

Rotacionar Câmera -> Setas direcionais, Q, E
Transladar Câmera -> W,A,S,D
Rotacionar Objeto -> G,H,J,Y,T,U
Transladar Objeto -> C,V,B,F,X,Z
Escalar Objeto -> SPACE,N
Selecionar Objeto -> TAB
Rotacionar Superfície de Bézier -> 0,1,2,3
Transladar Superfície de Bézier -> 4,5,6,7,8,9
Movimentar Pontos de Controle da Superfície -> 1,2,3,5,+,- (Teclado numérico)
Selecionar Ponto de Controle da Superfície -> Left-Ctrl
Fechar Programa -> ESC





*/


int main(int argc, char** argv)
{
    initSDL();
    configSDLGL(WIDTH, HEIGHT);
    atexit(deinit);

    while( !quit )
    {
        while( SDL_PollEvent(&event) )
        {
            quit = handle_keyboard_event(event);
            handle_mouse_event(event);
            handle_resize_event(event);
        }


        draw();


    }


    return 0;

}

