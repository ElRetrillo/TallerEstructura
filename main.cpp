#include "GestorJuego.h"
#include "GUIReversi.h"

int main() {

    GestorJuego juego; 
    
    GUIReversi gui(juego); 

    gui.ejecutar(); 

    return 0;
}