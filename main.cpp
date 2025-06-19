#include "GestorJuego.h"
#include "GUIReversi.h"
#include "TablaTransposicion.h"

int main() {
    inicializar_tabla_zobrist(); 

    GestorJuego juego; 
    GUIReversi gui(juego); 

    gui.ejecutar(); 

    return 0;
}