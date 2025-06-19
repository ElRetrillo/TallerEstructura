#include "IA.h"
#include "ConfiguracionJuego.h" 
#include "GestorJuego.h" 
#include <chrono> 

IA::IA(int profundidad) : profundidad_maxima(profundidad) {}

Movimiento IA::encontrar_mejor_movimiento(GestorJuego& gestor_juego_actual, char jugador_ia) {
    profundidad_maxima = 4; 

    int mejor_puntuacion = std::numeric_limits<int>::min();
    Movimiento mejor_movimiento = {-1, -1};

    char jugador_humano = (jugador_ia == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;

    tabla_transposicion.clear();

    std::vector<Movimiento> movimientos_posibles = gestor_juego_actual.tablero.obtener_movimientos_posibles(jugador_ia);
    if (movimientos_posibles.empty()) {
        return mejor_movimiento; 
    }

    for (const auto& movimiento : movimientos_posibles) {
        GestorJuego nuevo_gestor_juego = gestor_juego_actual; 
        
        std::vector<std::pair<int, int>> fichas_volteadas_simuladas = nuevo_gestor_juego.tablero.obtener_fichas_a_voltear(movimiento.fila, movimiento.columna, jugador_ia);
        nuevo_gestor_juego.tablero.realizar_movimiento(movimiento.fila, movimiento.columna, jugador_ia);
        nuevo_gestor_juego.ganar_energia_por_volteo(jugador_ia, fichas_volteadas_simuladas.size());
        nuevo_gestor_juego.verificar_dominacion_esquinas_publica(jugador_ia); 

        int puntuacion = minimax(nuevo_gestor_juego, profundidad_maxima - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), jugador_humano);

        if (puntuacion > mejor_puntuacion) {
            mejor_puntuacion = puntuacion;
            mejor_movimiento = movimiento;
        }
    }
    return mejor_movimiento;
}

int IA::evaluar_tablero(const GestorJuego& gestor_juego_actual, char jugador_ia) const {
    char jugador_humano = (jugador_ia == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;
    int puntuacion = 0;

    puntuacion += (gestor_juego_actual.tablero.contar_fichas(jugador_ia) - gestor_juego_actual.tablero.contar_fichas(jugador_humano)) * 10;

    int valor_esquina = 500;
    if (gestor_juego_actual.tablero.tablero[0][0] == jugador_ia) puntuacion += valor_esquina;
    if (gestor_juego_actual.tablero.tablero[0][TAMANO_TABLERO - 1] == jugador_ia) puntuacion += valor_esquina;
    if (gestor_juego_actual.tablero.tablero[TAMANO_TABLERO - 1][0] == jugador_ia) puntuacion += valor_esquina;
    if (gestor_juego_actual.tablero.tablero[TAMANO_TABLERO - 1][TAMANO_TABLERO - 1] == jugador_ia) puntuacion += valor_esquina;

    if (gestor_juego_actual.tablero.tablero[0][0] == jugador_humano) puntuacion -= valor_esquina;
    if (gestor_juego_actual.tablero.tablero[0][TAMANO_TABLERO - 1] == jugador_humano) puntuacion -= valor_esquina;
    if (gestor_juego_actual.tablero.tablero[TAMANO_TABLERO - 1][0] == jugador_humano) puntuacion -= valor_esquina;
    if (gestor_juego_actual.tablero.tablero[TAMANO_TABLERO - 1][TAMANO_TABLERO - 1] == jugador_humano) puntuacion -= valor_esquina;

    puntuacion += gestor_juego_actual.tablero.obtener_movimientos_posibles(jugador_ia).size() * 50;
    puntuacion -= gestor_juego_actual.tablero.obtener_movimientos_posibles(jugador_humano).size() * 50;

    puntuacion += gestor_juego_actual.obtener_energia(jugador_ia) * 5;

    return puntuacion;
}

int IA::minimax(GestorJuego gestor_juego_estado, int profundidad, bool es_maximizador, int alfa, int beta, char jugador_actual) {
    unsigned long long hash_actual = gestor_juego_estado.tablero.hash_actual; 
    if (tabla_transposicion.count(hash_actual)) {
        EntradaTransposicion& entrada = tabla_transposicion[hash_actual];
        if (entrada.profundidad >= profundidad) {
            if (entrada.bandera == EXACTO) return entrada.valor;
            if (entrada.bandera == LIMITE_INFERIOR && entrada.valor > alfa) alfa = entrada.valor;
            if (entrada.bandera == LIMITE_SUPERIOR && entrada.valor < beta) beta = entrada.valor;
            if (alfa >= beta) return entrada.valor;
        }
    }

    char jugador_oponente = (jugador_actual == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;

    if (profundidad == 0 || gestor_juego_estado.tablero.es_juego_terminado(JUGADOR_X, JUGADOR_O)) {
        if (gestor_juego_estado.tablero.es_juego_terminado(JUGADOR_X, JUGADOR_O)) {
            int puntuacion_X = gestor_juego_estado.tablero.contar_fichas(JUGADOR_X);
            int puntuacion_O = gestor_juego_estado.tablero.contar_fichas(JUGADOR_O);
            if (puntuacion_X > puntuacion_O) {
                return (JUGADOR_O == JUGADOR_X) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
            } else if (puntuacion_O > puntuacion_X) {
                return (JUGADOR_O == JUGADOR_O) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
            } else {
                return 0; 
            }
        }
        return evaluar_tablero(gestor_juego_estado, JUGADOR_O); 
    }

    std::vector<Movimiento> movimientos_posibles = gestor_juego_estado.tablero.obtener_movimientos_posibles(jugador_actual);

    if (movimientos_posibles.empty()) {
        return minimax(gestor_juego_estado, profundidad - 1, !es_maximizador, alfa, beta, jugador_oponente);
    }

    int valor;
    BanderaTransposicion bandera = EXACTO;

    if (es_maximizador) {
        valor = std::numeric_limits<int>::min();
        for (const auto& movimiento : movimientos_posibles) {
            GestorJuego nuevo_gestor_juego = gestor_juego_estado; 
            
            std::vector<std::pair<int, int>> fichas_volteadas_simuladas = nuevo_gestor_juego.tablero.obtener_fichas_a_voltear(movimiento.fila, movimiento.columna, jugador_actual);
            nuevo_gestor_juego.tablero.realizar_movimiento(movimiento.fila, movimiento.columna, jugador_actual);
            nuevo_gestor_juego.ganar_energia_por_volteo(jugador_actual, fichas_volteadas_simuladas.size());
            nuevo_gestor_juego.verificar_dominacion_esquinas_publica(jugador_actual); 

            valor = std::max(valor, minimax(nuevo_gestor_juego, profundidad - 1, false, alfa, beta, jugador_oponente));
            alfa = std::max(alfa, valor);
            if (alfa >= beta) {
                bandera = LIMITE_INFERIOR;
                break;
            }
        }
    } else { 
        valor = std::numeric_limits<int>::max();
        for (const auto& movimiento : movimientos_posibles) {
            GestorJuego nuevo_gestor_juego = gestor_juego_estado; 
            
            std::vector<std::pair<int, int>> fichas_volteadas_simuladas = nuevo_gestor_juego.tablero.obtener_fichas_a_voltear(movimiento.fila, movimiento.columna, jugador_actual);
            nuevo_gestor_juego.tablero.realizar_movimiento(movimiento.fila, movimiento.columna, jugador_actual);
            nuevo_gestor_juego.ganar_energia_por_volteo(jugador_actual, fichas_volteadas_simuladas.size());
            nuevo_gestor_juego.verificar_dominacion_esquinas_publica(jugador_actual); 

            valor = std::min(valor, minimax(nuevo_gestor_juego, profundidad - 1, true, alfa, beta, jugador_oponente));
            beta = std::min(beta, valor);
            if (alfa >= beta) { 
                bandera = LIMITE_SUPERIOR; 
                break;
            }
        }
    }

    tabla_transposicion[hash_actual] = {valor, profundidad, bandera};
    return valor;
}