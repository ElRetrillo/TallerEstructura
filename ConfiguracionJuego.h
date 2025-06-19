#ifndef CONFIGURACION_JUEGO_H
#define CONFIGURACION_JUEGO_H

// --- Constantes del Juego ---
const int TAMANO_TABLERO = 8;
const char VACIO = '.';
const char JUGADOR_X = 'X'; 
const char JUGADOR_O = 'O'; 

// --- Estados del Juego ---
enum EstadoJuego {
    EN_CURSO,
    JUGADOR_X_GANO,
    JUGADOR_O_GANO,
    EMPATE
};

// --- Modos de Jugador ---
enum ModoJugador {
    HUMANO_VS_HUMANO,
    HUMANO_VS_IA
};

// --- Dimensiones de la GUI ---
const int OFFSET_GUI_TABLERO_X = 50;
const int OFFSET_GUI_TABLERO_Y = 50;
const int TAMANO_CELDA = 70;

// NUEVAS CONSTANTES PARA EL LAYOUT DE PANELES LATERALES
const int ANCHO_PANEL_LATERAL = 250; // Ancho de los paneles laterales (ajustable)
const int MARGEN_PANEL_VERTICAL = 20; // Margen superior/inferior para los paneles

// ANCHO y ALTO de la VENTANA se ajustan para incluir los paneles laterales
const int ANCHO_VENTANA = (ANCHO_PANEL_LATERAL * 2) + (TAMANO_TABLERO * TAMANO_CELDA) + (OFFSET_GUI_TABLERO_X * 2);
const int ALTO_VENTANA = OFFSET_GUI_TABLERO_Y * 2 + TAMANO_TABLERO * TAMANO_CELDA + 200; 

// Margen para botones de habilidad dentro de su panel (se usará en la izquierda)
const int MARGEN_BOTONES_HABILIDAD_X = 20; 

// --- Puntos de Recurso y Costos de Habilidades ---
const int RECURSO_POR_FICHA_VOLTEADA = 1; 
const int COSTO_FICHA_FANTASMA = 3; // Esta siempre activa     
const int COSTO_VOLTEO_FORZADO = 25;     
const int COSTO_INTERCAMBIO_COLOR = 20; 

// Offset vertical para el texto de energía y otras estadísticas en los paneles
const int OFFSET_TEXTO_ENERGIA_Y = 120; 

// --- Estados de la GUI ---
enum EstadoGUI {
    MENU_PRINCIPAL,
    JUGANDO,
    PANTALLA_FIN_JUEGO
};

// --- Estado de Habilidades ---
enum EstadoHabilidad {
    NINGUNA_HABILIDAD,
    MODO_FICHA_FANTASMA,
    MODO_INTERCAMBIO_COLOR,
    MODO_VOLTEO_FORZADO
};

#endif