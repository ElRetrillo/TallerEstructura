#ifndef GUI_REVERSI_H
#define GUI_REVERSI_H

#include "ConfiguracionJuego.h" 
#include <SFML/Graphics.hpp>
#include "GestorJuego.h" 

class GUIReversi { 
public:
    GUIReversi(GestorJuego& gm);
    void ejecutar(); 

private:
    // MIEMBROS: Declarar en este ORDEN EXACTO para coincidir con el constructor y eliminar -Wreorder
    sf::RenderWindow ventana;
    GestorJuego& gestor_juego; 

    sf::Font fuente; 

    // Textura y Sprite - declarados juntos para inicialización
    sf::Texture textura_fondo_fin_juego; 
    sf::Sprite sprite_fondo_fin_juego;   

    // Todos los objetos sf::Text
    sf::Text texto_estado;
    sf::Text texto_boton_reiniciar;
    sf::Text titulo_menu;
    sf::Text texto_boton_jugar_ia; 
    sf::Text texto_boton_jugar_humano; 
    sf::Text texto_boton_volver_menu;
    sf::Text texto_boton_intercambiar_color; 
    sf::Text texto_boton_ficha_fantasma;     
    sf::Text texto_boton_volteo_forzado; 
    sf::Text texto_energia_jugador_x; 
    sf::Text texto_energia_jugador_o; 
    sf::Text texto_stats_jugador_x; 
    sf::Text texto_stats_jugador_o; 

    // Todos los objetos sf::RectangleShape
    sf::RectangleShape boton_reiniciar;
    sf::RectangleShape boton_jugar_ia; 
    sf::RectangleShape boton_jugar_humano; 
    sf::RectangleShape boton_volver_menu;
    sf::RectangleShape boton_intercambiar_color;
    sf::RectangleShape boton_ficha_fantasma;
    sf::RectangleShape boton_volteo_forzado; 
    
    // sf::CircleShape y bool para ficha fantasma
    sf::CircleShape forma_ficha_fantasma;
    bool mostrar_previsualizacion_fantasma;
    std::vector<std::pair<int, int>> celdas_a_voltear_fantasma;
    int fila_fantasma, columna_fantasma;
    
    // Enums
    EstadoGUI estado_gui; 
    EstadoHabilidad estado_habilidad_actual; 


    // Métodos de la clase
    void inicializar_graficos();
    void procesar_eventos(); 
    void actualizar();
    void renderizar();

    void dibujar_menu();
    void dibujar_pantalla_juego();
    void dibujar_tablero();
    void dibujar_fichas();
    void dibujar_texto_estado();
    void dibujar_boton_reiniciar();
    void actualizar_texto_estado();

    void dibujar_pantalla_fin_juego();

    void dibujar_ficha_fantasma();
    void dibujar_boton_intercambiar_color();
    void dibujar_boton_ficha_fantasma();
    void dibujar_boton_volteo_forzado(); 
    void dibujar_texto_energia(); 

    void manejar_clic_tablero(int fila, int columna);

    void ajustar_vista(unsigned int ancho_ventana, unsigned int alto_ventana);

    void reposicionar_elementos_gui();
    void dibujar_paneles_laterales();
};

#endif