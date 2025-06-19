#ifndef GESTOR_JUEGO_H
#define GESTOR_JUEGO_H

#include "ConfiguracionJuego.h" 
#include <stack>
#include <string>
#include "Tablero.h"
#include "IA.h" 

class GestorJuego {
public:
    Tablero tablero;
    char jugador_actual;
    IA agente_ia; 
    std::stack<Tablero> historial;
    EstadoJuego estado_juego;
    ModoJugador modo_actual; 

    bool jugador_x_puede_intercambiar_color;
    bool jugador_o_puede_intercambiar_color;

    int puntos_esquina_jugador_x;
    int puntos_esquina_jugador_o;

    int energia_jugador_x;
    int energia_jugador_o;

    GestorJuego(int profundidad_ia = 5);

    void establecer_modo_jugador(ModoJugador modo); 
    ModoJugador obtener_modo_jugador() const;

    bool intentar_mover_jugador(int fila, int columna);
    void procesar_turno_ia();
    void actualizar_estado_juego();

    char obtener_jugador_actual() const;
    EstadoJuego obtener_estado_juego() const;

    void reiniciar_juego();
    std::string obtener_nombre_jugador(char caracter_jugador) const;

    void cambiar_turno();

    bool puede_intercambiar_color(char jugador) const;
    void usar_intercambio_color(char jugador);
    bool realizar_intercambio_color(int fila, int columna, char jugador);

    int obtener_puntos_esquina_jugador_x() const { return puntos_esquina_jugador_x; }
    int obtener_puntos_esquina_jugador_o() const { return puntos_esquina_jugador_o; }

    int obtener_energia(char jugador) const;
    bool puede_comprar_habilidad(char jugador, int costo) const;
    void gastar_energia(char jugador, int costo);
    void ganar_energia_por_volteo(char jugador, int fichas_volteadas);
    bool realizar_volteo_forzado(int fila, int columna, char jugador);

    void verificar_dominacion_esquinas_publica(char jugador); 

private:
    void verificar_dominacion_esquinas(char jugador);
};

#endif