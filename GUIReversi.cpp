#include "GUIReversi.h"
#include <iostream>
#include <SFML/System.hpp> 
#include <string>

// Constructor
GUIReversi::GUIReversi(GestorJuego& gm) :
    // MIEMBROS: Inicializar en el ORDEN EXACTO de declaración en GUIReversi.h
    ventana(sf::VideoMode(sf::Vector2u(ANCHO_VENTANA, ALTO_VENTANA)), sf::String(L"FlipTurn")),
    gestor_juego(gm),
    
    fuente(), 
    
    textura_fondo_fin_juego(), 
    sprite_fondo_fin_juego(textura_fondo_fin_juego),   
    
    // Todos los objetos sf::Text
    texto_estado(fuente),
    texto_boton_reiniciar(fuente),
    titulo_menu(fuente),
    texto_boton_jugar_ia(fuente), 
    texto_boton_jugar_humano(fuente), 
    texto_boton_volver_menu(fuente),
    texto_boton_intercambiar_color(fuente), 
    texto_boton_ficha_fantasma(fuente),     
    texto_boton_volteo_forzado(fuente), 
    texto_energia_jugador_x(fuente), 
    texto_energia_jugador_o(fuente), 
    texto_stats_jugador_x(fuente), 
    texto_stats_jugador_o(fuente), 

    // Todos los objetos sf::RectangleShape
    boton_reiniciar(), 
    boton_jugar_ia(),  
    boton_jugar_humano(), 
    boton_volver_menu(), 
    boton_intercambiar_color(),
    boton_ficha_fantasma(),
    boton_volteo_forzado(), 
    
    // sf::CircleShape y bool para ficha fantasma
    forma_ficha_fantasma(static_cast<float>(TAMANO_CELDA / 2 - 5)), 
    mostrar_previsualizacion_fantasma(false), 
    celdas_a_voltear_fantasma(), 
    fila_fantasma(0), 
    columna_fantasma(0),
    
    // Enums
    estado_gui(MENU_PRINCIPAL), 
    estado_habilidad_actual(NINGUNA_HABILIDAD)     
{
    ventana.setFramerateLimit(60);
    inicializar_graficos();
    // La llamada a ajustar_vista se hará en el constructor, que a su vez llama a reposicionar_elementos_gui.
    ajustar_vista(ANCHO_VENTANA, ALTO_VENTANA); 
}

void GUIReversi::inicializar_graficos() {
    if (!fuente.openFromFile("arial.ttf")) {
        std::cerr << "Error cargando fuente arial.ttf" << std::endl;
    }

    if (!textura_fondo_fin_juego.loadFromFile("fondo_fin_juego.png")) {
        std::cerr << "ERROR DE CARGA: No se pudo cargar la textura del fondo de fin de juego: fondo_fin_juego.png" << std::endl;
    } else {
        sprite_fondo_fin_juego.setTexture(textura_fondo_fin_juego);
        sprite_fondo_fin_juego.setScale(
            sf::Vector2f( 
                static_cast<float>(ANCHO_VENTANA) / sprite_fondo_fin_juego.getLocalBounds().size.x, 
                static_cast<float>(ALTO_VENTANA) / sprite_fondo_fin_juego.getLocalBounds().size.y  
            )
        );
    }

    texto_estado.setCharacterSize(24);
    texto_estado.setFillColor(sf::Color::White);
    texto_estado.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 20));

    boton_reiniciar.setSize(sf::Vector2f(120, 40));
    boton_reiniciar.setFillColor(sf::Color(100, 100, 100));
    boton_reiniciar.setPosition(sf::Vector2f(ANCHO_VENTANA - OFFSET_GUI_TABLERO_X - 120, OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 5)); 
    texto_boton_reiniciar.setCharacterSize(20);
    texto_boton_reiniciar.setFillColor(sf::Color::White);
    texto_boton_reiniciar.setString(sf::String("Reiniciar")); // CORREGIDO
    sf::FloatRect limitesTexto = texto_boton_reiniciar.getLocalBounds();
    texto_boton_reiniciar.setOrigin(sf::Vector2f(limitesTexto.size.x / 2.0f, limitesTexto.size.y / 2.0f));
    texto_boton_reiniciar.setPosition(sf::Vector2f(boton_reiniciar.getPosition().x + boton_reiniciar.getSize().x / 2.0f,
                                            boton_reiniciar.getPosition().y + boton_reiniciar.getSize().y / 2.0f));

    titulo_menu.setString(sf::String("FLIPTURN")); // CORREGIDO
    titulo_menu.setCharacterSize(60);
    titulo_menu.setFillColor(sf::Color::White);
    titulo_menu.setOrigin(sf::Vector2f(titulo_menu.getLocalBounds().size.x / 2.0f, titulo_menu.getLocalBounds().size.y / 2.0f));
    titulo_menu.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 150.0f));

    boton_jugar_ia.setSize(sf::Vector2f(250, 60));
    boton_jugar_ia.setFillColor(sf::Color(50, 150, 50));
    boton_jugar_ia.setOrigin(sf::Vector2f(boton_jugar_ia.getLocalBounds().size.x / 2.0f, boton_jugar_ia.getLocalBounds().size.y / 2.0f));
    boton_jugar_ia.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 50.0f));
    texto_boton_jugar_ia.setString(sf::String("Jugar vs IA")); // CORREGIDO
    texto_boton_jugar_ia.setCharacterSize(24);
    texto_boton_jugar_ia.setFillColor(sf::Color::White);
    texto_boton_jugar_ia.setOrigin(sf::Vector2f(texto_boton_jugar_ia.getLocalBounds().size.x / 2.0f, texto_boton_jugar_ia.getLocalBounds().size.y / 2.0f));
    texto_boton_jugar_ia.setPosition(boton_jugar_ia.getPosition());

    boton_jugar_humano.setSize(sf::Vector2f(250, 60));
    boton_jugar_humano.setFillColor(sf::Color(50, 50, 150));
    boton_jugar_humano.setOrigin(sf::Vector2f(boton_jugar_humano.getLocalBounds().size.x / 2.0f, boton_jugar_humano.getLocalBounds().size.y / 2.0f));
    boton_jugar_humano.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f + 50.0f));
    texto_boton_jugar_humano.setString(sf::String("Jugar 1 vs 1")); // CORREGIDO
    texto_boton_jugar_humano.setCharacterSize(24);
    texto_boton_jugar_humano.setFillColor(sf::Color::White);
    texto_boton_jugar_humano.setOrigin(sf::Vector2f(texto_boton_jugar_humano.getLocalBounds().size.x / 2.0f, texto_boton_jugar_humano.getLocalBounds().size.y / 2.0f));
    texto_boton_jugar_humano.setPosition(boton_jugar_humano.getPosition());

    boton_volver_menu.setSize(sf::Vector2f(200, 40));
    boton_volver_menu.setFillColor(sf::Color(150, 50, 50));
    boton_volver_menu.setOrigin(sf::Vector2f(boton_volver_menu.getLocalBounds().size.x / 2.0f, boton_volver_menu.getLocalBounds().size.y / 2.0f));
    boton_volver_menu.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA - 50.0f));
    texto_boton_volver_menu.setString(sf::String("Volver al Menu")); // CORREGIDO
    texto_boton_volver_menu.setCharacterSize(20);
    texto_boton_volver_menu.setFillColor(sf::Color::White);
    texto_boton_volver_menu.setOrigin(sf::Vector2f(texto_boton_volver_menu.getLocalBounds().size.x / 2.0f, texto_boton_volver_menu.getLocalBounds().size.y / 2.0f));
    texto_boton_volver_menu.setPosition(boton_volver_menu.getPosition());

    float offset_boton_habilidad_y = MARGEN_PANEL_VERTICAL + 150; // Variable local

    boton_intercambiar_color.setSize(sf::Vector2f(180, 40));
    boton_intercambiar_color.setFillColor(sf::Color(200, 100, 0));
    boton_intercambiar_color.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, offset_boton_habilidad_y));
    // CORREGIDO: Usar sf::String directamente con la std::string construida
    texto_boton_intercambiar_color.setString(sf::String("Intercambiar Color (" + std::to_string(COSTO_INTERCAMBIO_COLOR) + " Energia)")); 
    texto_boton_intercambiar_color.setCharacterSize(16);
    texto_boton_intercambiar_color.setFillColor(sf::Color::White);
    sf::FloatRect limitesTextoIntercambio = texto_boton_intercambiar_color.getLocalBounds();
    texto_boton_intercambiar_color.setOrigin(sf::Vector2f(limitesTextoIntercambio.size.x / 2.0f, limitesTextoIntercambio.size.y / 2.0f));
    texto_boton_intercambiar_color.setPosition(sf::Vector2f(boton_intercambiar_color.getPosition().x + boton_intercambiar_color.getSize().x / 2.0f,
                                      boton_intercambiar_color.getPosition().y + boton_intercambiar_color.getSize().y / 2.0f));

    /*boton_ficha_fantasma.setSize(sf::Vector2f(150, 40));
    boton_ficha_fantasma.setFillColor(sf::Color(100, 50, 150));
    boton_ficha_fantasma.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, offset_boton_habilidad_y + boton_intercambiar_color.getSize().y + 10));
    // CORREGIDO: Usar sf::String directamente con la std::string construida
    texto_boton_ficha_fantasma.setString(sf::String("Ficha Fantasma (" + std::to_string(COSTO_FICHA_FANTASMA) + " Energia)")); 
    texto_boton_ficha_fantasma.setCharacterSize(16);
    texto_boton_ficha_fantasma.setFillColor(sf::Color::White);
    sf::FloatRect limitesTextoFantasma = texto_boton_ficha_fantasma.getLocalBounds();
    texto_boton_ficha_fantasma.setOrigin(sf::Vector2f(limitesTextoFantasma.size.x / 2.0f, limitesTextoFantasma.size.y / 2.0f));
    texto_boton_ficha_fantasma.setPosition(sf::Vector2f(boton_ficha_fantasma.getPosition().x + boton_ficha_fantasma.getSize().x / 2.0f,
                                      boton_ficha_fantasma.getPosition().y + boton_ficha_fantasma.getSize().y / 2.0f));*/

    boton_volteo_forzado.setSize(sf::Vector2f(160, 40));
    boton_volteo_forzado.setFillColor(sf::Color(0, 100, 200));
    boton_volteo_forzado.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, offset_boton_habilidad_y + boton_intercambiar_color.getSize().y + 10 + boton_ficha_fantasma.getSize().y + 10));
    // CORREGIDO: Usar sf::String directamente con la std::string construida
    texto_boton_volteo_forzado.setString(sf::String("Volteo Forzado (" + std::to_string(COSTO_VOLTEO_FORZADO) + " Energia)")); 
    texto_boton_volteo_forzado.setCharacterSize(16);
    texto_boton_volteo_forzado.setFillColor(sf::Color::White);
    sf::FloatRect limitesTextoVolteo = texto_boton_volteo_forzado.getLocalBounds();
    texto_boton_volteo_forzado.setOrigin(sf::Vector2f(limitesTextoVolteo.size.x / 2.0f, limitesTextoVolteo.size.y / 2.0f));
    texto_boton_volteo_forzado.setPosition(sf::Vector2f(boton_volteo_forzado.getPosition().x + boton_volteo_forzado.getSize().x / 2.0f,
                                      boton_volteo_forzado.getPosition().y + boton_volteo_forzado.getSize().y / 2.0f));


    texto_energia_jugador_x.setFont(fuente);
    texto_energia_jugador_x.setCharacterSize(20);
    texto_energia_jugador_x.setFillColor(sf::Color::Yellow);

    texto_energia_jugador_o.setFont(fuente);
    texto_energia_jugador_o.setCharacterSize(20);
    texto_energia_jugador_o.setFillColor(sf::Color::Yellow);

    texto_stats_jugador_x.setFont(fuente);
    texto_stats_jugador_x.setCharacterSize(20);
    texto_stats_jugador_x.setFillColor(sf::Color::White);

    texto_stats_jugador_o.setFont(fuente);
    texto_stats_jugador_o.setCharacterSize(20);
    texto_stats_jugador_o.setFillColor(sf::Color::White);

    forma_ficha_fantasma.setOutlineThickness(2);
    forma_ficha_fantasma.setOutlineColor(sf::Color(100, 100, 100));
}

void GUIReversi::ajustar_vista(unsigned int ancho_ventana, unsigned int alto_ventana) {
    sf::View vista;
    vista.setSize(sf::Vector2f(ANCHO_VENTANA, ALTO_VENTANA));
    vista.setCenter(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f));

    float relacion_aspecto_ventana = static_cast<float>(ancho_ventana) / alto_ventana;
    float relacion_aspecto_base = static_cast<float>(ANCHO_VENTANA) / ALTO_VENTANA;

    float escala_x = 1.0f;
    float escala_y = 1.0f;

    if (relacion_aspecto_ventana > relacion_aspecto_base) {
        escala_y = 1.0f;
        escala_x = relacion_aspecto_base / relacion_aspecto_ventana;
    } else {
        escala_x = 1.0f;
        escala_y = relacion_aspecto_ventana / relacion_aspecto_base;
    }

    float ancho_viewport = escala_x;
    float alto_viewport = escala_y;
    float pos_x_viewport = (1.0f - ancho_viewport) / 2.0f;
    float pos_y_viewport = (1.0f - alto_viewport) / 2.0f;

    vista.setViewport(sf::FloatRect(sf::Vector2f(pos_x_viewport, pos_y_viewport), sf::Vector2f(ancho_viewport, alto_viewport)));
    ventana.setView(vista);

    reposicionar_elementos_gui();
}

void GUIReversi::ejecutar() {
    while (ventana.isOpen()) {
        procesar_eventos();
        actualizar();
        renderizar();
    }
}

void GUIReversi::procesar_eventos() { // Corregido: procesar_eventos()
    while (const std::optional<sf::Event> evento_opt = ventana.pollEvent()) {
        if (evento_opt->is<sf::Event::Closed>()) {
            ventana.close();
        }
        else if (const sf::Event::Resized* resizeEvent = evento_opt->getIf<sf::Event::Resized>()) {
            ajustar_vista(resizeEvent->size.x, resizeEvent->size.y); 
        }
        else if (evento_opt->is<sf::Event::MouseMoved>()) {
            if (estado_gui == JUGANDO && gestor_juego.obtener_estado_juego() == EN_CURSO && estado_habilidad_actual == NINGUNA_HABILIDAD) {
                sf::Vector2f pos_raton_ventana = sf::Vector2f(static_cast<float>(evento_opt->getIf<sf::Event::MouseMoved>()->position.x), static_cast<float>(evento_opt->getIf<sf::Event::MouseMoved>()->position.y));
                sf::Vector2f pos_raton_vista = ventana.mapPixelToCoords(static_cast<sf::Vector2i>(pos_raton_ventana));

                int columna = static_cast<int>((pos_raton_vista.x - (ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X)) / TAMANO_CELDA); 
                int fila = static_cast<int>((pos_raton_vista.y - OFFSET_GUI_TABLERO_Y) / TAMANO_CELDA);

                if (fila >= 0 && fila < TAMANO_TABLERO && columna >= 0 && columna < TAMANO_TABLERO && gestor_juego.tablero.tablero[fila][columna] == VACIO) {
                    fila_fantasma = fila;
                    columna_fantasma = columna;
                    celdas_a_voltear_fantasma = gestor_juego.tablero.obtener_fichas_a_voltear(fila, columna, gestor_juego.obtener_jugador_actual());
                    mostrar_previsualizacion_fantasma = !celdas_a_voltear_fantasma.empty();
                } else {
                    mostrar_previsualizacion_fantasma = false;
                }
            } else {
                mostrar_previsualizacion_fantasma = false;
            }
        }
        else if (const sf::Event::MouseButtonPressed* clicRaton = evento_opt->getIf<sf::Event::MouseButtonPressed>()) {
            if (clicRaton->button == sf::Mouse::Button::Left) {
                sf::Vector2f pos_raton_ventana = sf::Vector2f(static_cast<float>(clicRaton->position.x), static_cast<float>(clicRaton->position.y));
                sf::Vector2f pos_raton_vista = ventana.mapPixelToCoords(static_cast<sf::Vector2i>(pos_raton_ventana));

                if (estado_gui == MENU_PRINCIPAL) {
                    if (boton_jugar_ia.getGlobalBounds().contains(pos_raton_vista)) {
                        gestor_juego.establecer_modo_jugador(HUMANO_VS_IA);
                        estado_gui = JUGANDO;
                        std::cout << "Iniciando juego: Humano vs IA" << std::endl;
                    } else if (boton_jugar_humano.getGlobalBounds().contains(pos_raton_vista)) {
                        gestor_juego.establecer_modo_jugador(HUMANO_VS_HUMANO);
                        estado_gui = JUGANDO;
                        std::cout << "Iniciando juego: Humano vs Humano" << std::endl;
                    }
                } else if (estado_gui == JUGANDO) {
                    if (boton_reiniciar.getGlobalBounds().contains(pos_raton_vista)) {
                        gestor_juego.reiniciar_juego();
                        estado_habilidad_actual = NINGUNA_HABILIDAD;
                        std::cout << "Juego reiniciado." << std::endl;
                        return;
                    }

                    if (boton_intercambiar_color.getGlobalBounds().contains(pos_raton_vista)) {
                        if (gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_INTERCAMBIO_COLOR)) { 
                            if (estado_habilidad_actual == NINGUNA_HABILIDAD) {
                                estado_habilidad_actual = MODO_INTERCAMBIO_COLOR;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Intercambio de Color activado. Haz clic en una ficha del oponente." << std::endl;
                            } else if (estado_habilidad_actual == MODO_INTERCAMBIO_COLOR) {
                                estado_habilidad_actual = NINGUNA_HABILIDAD;
                                std::cout << "Modo Intercambio de Color desactivado." << std::endl;
                            } else {
                                estado_habilidad_actual = MODO_INTERCAMBIO_COLOR;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Intercambio de Color activado (desactivando otra habilidad)." << std::endl;
                            }
                        } else {
                            std::cout << "No tienes suficiente energía para Intercambio de Color (Costo: " << COSTO_INTERCAMBIO_COLOR << ")." << std::endl;
                        }
                        return;
                    }

                    if (boton_ficha_fantasma.getGlobalBounds().contains(pos_raton_vista)) {
                        if (gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_FICHA_FANTASMA)) {
                            if (estado_habilidad_actual == NINGUNA_HABILIDAD) {
                                estado_habilidad_actual = MODO_FICHA_FANTASMA;
                                std::cout << "Modo Ficha Fantasma activado. Mueve el ratón para previsualizar." << std::endl;
                            } else if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
                                estado_habilidad_actual = NINGUNA_HABILIDAD;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Ficha Fantasma desactivado." << std::endl;
                            } else {
                                estado_habilidad_actual = MODO_FICHA_FANTASMA;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Ficha Fantasma activado (desactivando otra habilidad)." << std::endl;
                            }
                        } else {
                            std::cout << "No tienes suficiente energía para Ficha Fantasma (Costo: " << COSTO_FICHA_FANTASMA << ")." << std::endl;
                        }
                        return;
                    }
                    
                    if (boton_volteo_forzado.getGlobalBounds().contains(pos_raton_vista)) {
                        if (gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_VOLTEO_FORZADO)) {
                            if (estado_habilidad_actual == NINGUNA_HABILIDAD) {
                                estado_habilidad_actual = MODO_VOLTEO_FORZADO;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Volteo Forzado activado. Haz clic en una ficha del oponente." << std::endl;
                            } else if (estado_habilidad_actual == MODO_VOLTEO_FORZADO) {
                                estado_habilidad_actual = NINGUNA_HABILIDAD;
                                std::cout << "Modo Volteo Forzado desactivado." << std::endl;
                            } else {
                                estado_habilidad_actual = MODO_VOLTEO_FORZADO;
                                mostrar_previsualizacion_fantasma = false;
                                std::cout << "Modo Volteo Forzado activado (desactivando otra habilidad)." << std::endl;
                            }
                        } else {
                            std::cout << "No tienes suficiente energía para Volteo Forzado (Costo: " << COSTO_VOLTEO_FORZADO << ")." << std::endl;
                        }
                        return;
                    }


                    manejar_clic_tablero(
                        static_cast<int>((pos_raton_vista.y - OFFSET_GUI_TABLERO_Y) / TAMANO_CELDA),
                        static_cast<int>((pos_raton_vista.x - (ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X)) / TAMANO_CELDA) 
                    );

                } else if (estado_gui == PANTALLA_FIN_JUEGO) {
                    if (boton_volver_menu.getGlobalBounds().contains(pos_raton_vista)) {
                        estado_gui = MENU_PRINCIPAL;
                        gestor_juego.reiniciar_juego();
                        estado_habilidad_actual = NINGUNA_HABILIDAD;
                        std::cout << "Volviendo al menu principal." << std::endl;
                    } else if (boton_reiniciar.getGlobalBounds().contains(pos_raton_vista)) {
                        gestor_juego.reiniciar_juego();
                        estado_gui = JUGANDO;
                        estado_habilidad_actual = NINGUNA_HABILIDAD;
                        std::cout << "Juego reiniciado desde fin de partida." << std::endl;
                    }
                }
            }
        }
    }
}

void GUIReversi::manejar_clic_tablero(int fila, int columna) {
    if (fila < 0 || fila >= TAMANO_TABLERO || columna < 0 || columna >= TAMANO_TABLERO) return;

    if (estado_habilidad_actual == MODO_INTERCAMBIO_COLOR) {
        if (gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_INTERCAMBIO_COLOR)) { 
            if (gestor_juego.realizar_intercambio_color(fila, columna, gestor_juego.obtener_jugador_actual())) {
                gestor_juego.gastar_energia(gestor_juego.obtener_jugador_actual(), COSTO_INTERCAMBIO_COLOR); 
                gestor_juego.usar_intercambio_color(gestor_juego.obtener_jugador_actual());
                estado_habilidad_actual = NINGUNA_HABILIDAD;
                gestor_juego.actualizar_estado_juego();
                gestor_juego.cambiar_turno();
                std::cout << "Intercambio de color realizado en: " << fila + 1 << ", " << columna + 1 << std::endl;
            } else {
                std::cout << "Movimiento de Intercambio de Color invalido (debe ser una ficha del oponente)." << std::endl;
            }
        } else {
            std::cout << "Error: No tienes suficiente energía para Intercambio de Color (Costo: " << COSTO_INTERCAMBIO_COLOR << ")." << std::endl;
            estado_habilidad_actual = NINGUNA_HABILIDAD;
        }
    } else if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
        estado_habilidad_actual = NINGUNA_HABILIDAD;
        mostrar_previsualizacion_fantasma = false;
        std::cout << "Ficha Fantasma desactivada. Haz un movimiento normal." << std::endl;
    } else if (estado_habilidad_actual == MODO_VOLTEO_FORZADO) {
        if (gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_VOLTEO_FORZADO)) {
            if (gestor_juego.realizar_volteo_forzado(fila, columna, gestor_juego.obtener_jugador_actual())) {
                gestor_juego.gastar_energia(gestor_juego.obtener_jugador_actual(), COSTO_VOLTEO_FORZADO);
                estado_habilidad_actual = NINGUNA_HABILIDAD;
                gestor_juego.actualizar_estado_juego();
                gestor_juego.cambiar_turno();
                std::cout << "Volteo forzado realizado en: " << fila + 1 << ", " << columna + 1 << std::endl;
            } else {
                std::cout << "Volteo Forzado inválido (debe ser una ficha del oponente)." << std::endl;
            }
        } else {
            std::cout << "Error: No tienes suficiente energía para realizar el Volteo Forzado." << std::endl;
            estado_habilidad_actual = NINGUNA_HABILIDAD;
        }
    }
    else { // Modo normal (NINGUNA_HABILIDAD)
        if (gestor_juego.obtener_estado_juego() == EN_CURSO) {
            bool movimiento_exitoso = gestor_juego.intentar_mover_jugador(fila, columna);
            if (movimiento_exitoso) {
                std::cout << "Jugador " << gestor_juego.obtener_jugador_actual() << " hizo movimiento en: " << fila + 1 << ", " << columna + 1 << std::endl;
            }
        }
    }
}

void GUIReversi::actualizar() {
    if (estado_gui == JUGANDO) {
        
if (gestor_juego.obtener_jugador_actual() == JUGADOR_O &&
    gestor_juego.obtener_modo_jugador() == HUMANO_VS_IA &&
    gestor_juego.obtener_estado_juego() == EN_CURSO)
{
    if (!gestor_juego.tablero.obtener_movimientos_posibles(JUGADOR_O).empty()) {
        sf::Clock reloj_ia;
        gestor_juego.procesar_turno_ia();
        sf::Time tiempo_transcurrido = reloj_ia.getElapsedTime();
        std::cout << "IA procesó en: " << tiempo_transcurrido.asSeconds() << " segundos." << std::endl;
    } else {
        std::cout << "IA no tiene movimientos válidos. Se salta el turno." << std::endl;
        gestor_juego.cambiar_turno();
    }
}


        if (gestor_juego.obtener_estado_juego() != EN_CURSO) {
            estado_gui = PANTALLA_FIN_JUEGO;
        }
    }
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) {
        actualizar_texto_estado();
    }
}

void GUIReversi::renderizar() {
    ventana.clear(sf::Color::Black);
    ventana.setView(ventana.getView());

    if (estado_gui == MENU_PRINCIPAL) {
        dibujar_menu();
    } else if (estado_gui == JUGANDO) {
        dibujar_pantalla_juego();
        dibujar_ficha_fantasma(); 
        dibujar_boton_intercambiar_color(); 
        dibujar_boton_ficha_fantasma(); 
        dibujar_boton_volteo_forzado(); 
        dibujar_texto_energia();
        dibujar_paneles_laterales();
    } else if (estado_gui == PANTALLA_FIN_JUEGO) {
        dibujar_pantalla_juego();
        dibujar_pantalla_fin_juego();
        dibujar_boton_intercambiar_color(); 
        dibujar_boton_ficha_fantasma(); 
        dibujar_boton_volteo_forzado(); 
        dibujar_texto_energia();
        dibujar_paneles_laterales();
    }

    ventana.display();
}

void GUIReversi::dibujar_menu() {
    ventana.draw(titulo_menu);
    ventana.draw(boton_jugar_ia);
    ventana.draw(texto_boton_jugar_ia);
    ventana.draw(boton_jugar_humano);
    ventana.draw(texto_boton_jugar_humano);
}

void GUIReversi::dibujar_pantalla_juego() {
    dibujar_tablero();
    dibujar_fichas();
    dibujar_texto_estado();
    dibujar_boton_reiniciar();
}

void GUIReversi::dibujar_tablero() {
    for (int fila = 0; fila < TAMANO_TABLERO; ++fila) {
        for (int columna = 0; columna < TAMANO_TABLERO; ++columna) {
            sf::RectangleShape celda(sf::Vector2f(TAMANO_CELDA, TAMANO_CELDA));
            celda.setPosition(sf::Vector2f(ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X + columna * TAMANO_CELDA, OFFSET_GUI_TABLERO_Y + fila * TAMANO_CELDA));

            if ((fila + columna) % 2 == 0) {
                celda.setFillColor(sf::Color(0, 100, 0));
            } else {
                celda.setFillColor(sf::Color(0, 150, 0));
            }
            celda.setOutlineThickness(1);
            celda.setOutlineColor(sf::Color::Black);
            ventana.draw(celda);
        }
    }
}

void GUIReversi::dibujar_fichas() {
    for (int fila = 0; fila < TAMANO_TABLERO; ++fila) {
        for (int columna = 0; columna < TAMANO_TABLERO; ++columna) {
            char caracter_ficha = gestor_juego.tablero.tablero[fila][columna];
            if (caracter_ficha != VACIO) {
                sf::CircleShape ficha(static_cast<float>(TAMANO_CELDA / 2 - 5));
                ficha.setPosition(sf::Vector2f(ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X + columna * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + fila * TAMANO_CELDA + 5));
                if (caracter_ficha == JUGADOR_X) {
                    ficha.setFillColor(sf::Color::Black);
                } else {
                    ficha.setFillColor(sf::Color::White);
                }
                ficha.setOutlineThickness(2.0f);
                ficha.setOutlineColor(sf::Color(150, 150, 150));
                ventana.draw(ficha);
            }
        }
    }
}

void GUIReversi::dibujar_ficha_fantasma() {
    if (mostrar_previsualizacion_fantasma) {
        forma_ficha_fantasma.setPosition(sf::Vector2f(ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X + columna_fantasma * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + fila_fantasma * TAMANO_CELDA + 5));
        if (gestor_juego.obtener_jugador_actual() == JUGADOR_X) {
            forma_ficha_fantasma.setFillColor(sf::Color(0, 0, 0, 100));
        } else {
            forma_ficha_fantasma.setFillColor(sf::Color(255, 255, 255, 100));
        }
        ventana.draw(forma_ficha_fantasma);

        for (const auto& coords_celda : celdas_a_voltear_fantasma) {
            sf::CircleShape previsualizacion_a_voltear(static_cast<float>(TAMANO_CELDA / 2 - 5));
            previsualizacion_a_voltear.setPosition(sf::Vector2f(ANCHO_PANEL_LATERAL + OFFSET_GUI_TABLERO_X + coords_celda.second * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + coords_celda.first * TAMANO_CELDA + 5));
            if (gestor_juego.obtener_jugador_actual() == JUGADOR_X) {
                previsualizacion_a_voltear.setFillColor(sf::Color(0, 0, 0, 180));
            } else {
                previsualizacion_a_voltear.setFillColor(sf::Color(255, 255, 255, 180));
            }
            previsualizacion_a_voltear.setOutlineThickness(2.0f);
            previsualizacion_a_voltear.setOutlineColor(sf::Color(255, 0, 0, 200));
            ventana.draw(previsualizacion_a_voltear);
        }
    }
}

void GUIReversi::dibujar_boton_intercambiar_color() {
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) {
        if (!gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_INTERCAMBIO_COLOR)) {
            boton_intercambiar_color.setFillColor(sf::Color(50, 50, 50));
            texto_boton_intercambiar_color.setFillColor(sf::Color(150, 150, 150));
        } else {
            boton_intercambiar_color.setFillColor(sf::Color(200, 100, 0));
            texto_boton_intercambiar_color.setFillColor(sf::Color::White);
        }

        if (estado_habilidad_actual == MODO_INTERCAMBIO_COLOR) {
            boton_intercambiar_color.setOutlineThickness(3);
            boton_intercambiar_color.setOutlineColor(sf::Color::Yellow);
        } else {
            boton_intercambiar_color.setOutlineThickness(0);
        }    }
}

void GUIReversi::dibujar_boton_ficha_fantasma() {
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) {
        if (!gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_FICHA_FANTASMA)) {
             boton_ficha_fantasma.setFillColor(sf::Color(50, 50, 50));
             texto_boton_ficha_fantasma.setFillColor(sf::Color(150, 150, 150));
        } else {
            boton_ficha_fantasma.setFillColor(sf::Color(100, 50, 150));
            texto_boton_ficha_fantasma.setFillColor(sf::Color::White);
        }

        if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
            boton_ficha_fantasma.setOutlineThickness(3);
            boton_ficha_fantasma.setOutlineColor(sf::Color::Yellow);
        } else {
            boton_ficha_fantasma.setOutlineThickness(0);
        }    }
}

void GUIReversi::dibujar_boton_volteo_forzado() {
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) { 
        if (!gestor_juego.puede_comprar_habilidad(gestor_juego.obtener_jugador_actual(), COSTO_VOLTEO_FORZADO)) {
            boton_volteo_forzado.setFillColor(sf::Color(50, 50, 50));
            texto_boton_volteo_forzado.setFillColor(sf::Color(150, 150, 150));
        } else {
            boton_volteo_forzado.setFillColor(sf::Color(0, 100, 200));
            texto_boton_volteo_forzado.setFillColor(sf::Color::White);
        }

        if (estado_habilidad_actual == MODO_VOLTEO_FORZADO) {
            boton_volteo_forzado.setOutlineThickness(3);
            boton_volteo_forzado.setOutlineColor(sf::Color::Yellow);
        } else {
            boton_volteo_forzado.setOutlineThickness(0);
        }    }
}

void GUIReversi::dibujar_texto_energia() {
    std::string energia_x_str = "Energía " + gestor_juego.obtener_nombre_jugador(JUGADOR_X) + ": " + std::to_string(gestor_juego.obtener_energia(JUGADOR_X));
    std::string energia_o_str = "Energía " + gestor_juego.obtener_nombre_jugador(JUGADOR_O) + ": " + std::to_string(gestor_juego.obtener_energia(JUGADOR_O));

    texto_energia_jugador_x.setString(sf::String::fromUtf8(energia_x_str.begin(), energia_x_str.end()));
    texto_energia_jugador_o.setString(sf::String::fromUtf8(energia_o_str.begin(), energia_o_str.end())); 

    // Posición para energía X (en el panel izquierdo, debajo de stats J1)
    texto_energia_jugador_x.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, MARGEN_PANEL_VERTICAL + 90)); 
    // Posición para energía O (en el panel derecho, debajo de stats J2)
    texto_energia_jugador_o.setPosition(sf::Vector2f(ANCHO_VENTANA - ANCHO_PANEL_LATERAL + MARGEN_BOTONES_HABILIDAD_X, MARGEN_PANEL_VERTICAL + 90));}


void GUIReversi::actualizar_texto_estado() {
    std::string mensaje_estado_std;
    if (gestor_juego.obtener_estado_juego() == JUGADOR_X_GANO) {
        mensaje_estado_std = "¡" + gestor_juego.obtener_nombre_jugador(JUGADOR_X) + " GANA!";
    } else if (gestor_juego.obtener_estado_juego() == JUGADOR_O_GANO) {
        mensaje_estado_std = "¡" + gestor_juego.obtener_nombre_jugador(JUGADOR_O) + " GANA!";
    } else if (gestor_juego.obtener_estado_juego() == EMPATE) {
        mensaje_estado_std = "¡EMPATE!";
    } else {
        mensaje_estado_std = "Turno de: " + gestor_juego.obtener_nombre_jugador(gestor_juego.obtener_jugador_actual());
    }
    mensaje_estado_std += "\nPuntuación " + gestor_juego.obtener_nombre_jugador(JUGADOR_X) + ": " + std::to_string(gestor_juego.tablero.contar_fichas(JUGADOR_X)) +
                      " (Bonus Esquinas: " + std::to_string(gestor_juego.obtener_puntos_esquina_jugador_x()) + ")";
    mensaje_estado_std += "\nPuntuación " + gestor_juego.obtener_nombre_jugador(JUGADOR_O) + ": " + std::to_string(gestor_juego.tablero.contar_fichas(JUGADOR_O)) +
                      " (Bonus Esquinas: " + std::to_string(gestor_juego.obtener_puntos_esquina_jugador_o()) + ")";

    texto_estado.setString(sf::String::fromUtf8(mensaje_estado_std.begin(), mensaje_estado_std.end()));

    // Reposicionar el texto de estado principal (debajo del tablero)
    // Esto es para el texto que va debajo del tablero, "Turno de: ..."
    texto_estado.setOrigin(sf::Vector2f(texto_estado.getLocalBounds().size.x / 2.0f, texto_estado.getLocalBounds().size.y / 2.0f)); 
    texto_estado.setPosition(sf::Vector2f(ANCHO_PANEL_LATERAL + (TAMANO_TABLERO * TAMANO_CELDA) / 2.0f + OFFSET_GUI_TABLERO_X, 
                                          OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 50));

    // --- ACTUALIZAR Y POSICIONAR TEXTOS DE ESTADÍSTICAS LATERALES ---
    // Jugador 1 (Negro)
    std::string stats_x_str = gestor_juego.obtener_nombre_jugador(JUGADOR_X);
    stats_x_str += "\nFichas: " + std::to_string(gestor_juego.tablero.contar_fichas(JUGADOR_X));
    stats_x_str += "\nEsquinas: " + std::to_string(gestor_juego.obtener_puntos_esquina_jugador_x() / 10);
    texto_stats_jugador_x.setString(sf::String::fromUtf8(stats_x_str.begin(), stats_x_str.end()));
    // Posición ya fijada en reposicionar_elementos_gui.

    // Jugador 2 (Blanco/IA)
    std::string stats_o_str = gestor_juego.obtener_nombre_jugador(JUGADOR_O); // "Jugador 2 (Blanco)" o "IA (Blanco)"
    stats_o_str += "\nFichas: " + std::to_string(gestor_juego.tablero.contar_fichas(JUGADOR_O));
    stats_o_str += "\nEsquinas: " + std::to_string(gestor_juego.obtener_puntos_esquina_jugador_o() / 10);
    texto_stats_jugador_o.setString(sf::String::fromUtf8(stats_o_str.begin(), stats_o_str.end()));
    // Posición ya fijada en reposicionar_elementos_gui.
}

void GUIReversi::dibujar_texto_estado() {
    ventana.draw(texto_estado);
}

void GUIReversi::dibujar_boton_reiniciar() {
    ventana.draw(boton_reiniciar);
    ventana.draw(texto_boton_reiniciar);
}

void GUIReversi::dibujar_pantalla_fin_juego() {
    ventana.draw(sprite_fondo_fin_juego);

    sf::RectangleShape superposicion(sf::Vector2f(ANCHO_VENTANA, ALTO_VENTANA));
    superposicion.setFillColor(sf::Color(0, 0, 0, 180));

    sf::Text mensaje_final(fuente);
    mensaje_final.setCharacterSize(20);
    mensaje_final.setFillColor(sf::Color::Red);
    mensaje_final.setString(texto_estado.getString());
    
    mensaje_final.setOrigin(sf::Vector2f(mensaje_final.getLocalBounds().size.x / 2.0f, mensaje_final.getLocalBounds().size.y / 2.0f));
    mensaje_final.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 100.0f));

    ventana.draw(superposicion);
    ventana.draw(mensaje_final);
    ventana.draw(boton_volver_menu);
    ventana.draw(texto_boton_volver_menu);
    ventana.draw(boton_reiniciar);
    ventana.draw(texto_boton_reiniciar);
}

void GUIReversi::reposicionar_elementos_gui() {
    // Reposicionar elementos del panel izquierdo (Jugador 1)
    texto_stats_jugador_x.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, MARGEN_PANEL_VERTICAL + 20));
    
    // Reposicionar elementos del panel derecho (Jugador 2)  
    texto_stats_jugador_o.setPosition(sf::Vector2f(ANCHO_VENTANA - ANCHO_PANEL_LATERAL + MARGEN_BOTONES_HABILIDAD_X, MARGEN_PANEL_VERTICAL + 20));
    
    // Reposicionar botones de habilidades (ya están posicionados en inicializar_graficos)
    // Solo necesitamos ajustar si hay cambios de ventana, pero por ahora mantenemos posiciones fijas
    
    // Reposicionar botón reiniciar
    boton_reiniciar.setPosition(sf::Vector2f(ANCHO_VENTANA - OFFSET_GUI_TABLERO_X - 120, OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 5));
    sf::FloatRect limitesTexto = texto_boton_reiniciar.getLocalBounds();
    texto_boton_reiniciar.setOrigin(sf::Vector2f(limitesTexto.size.x / 2.0f, limitesTexto.size.y / 2.0f));
    texto_boton_reiniciar.setPosition(sf::Vector2f(boton_reiniciar.getPosition().x + boton_reiniciar.getSize().x / 2.0f,
                                            boton_reiniciar.getPosition().y + boton_reiniciar.getSize().y / 2.0f));
}

void GUIReversi::dibujar_paneles_laterales() {
    // Dibujar panel izquierdo (Jugador 1)
    sf::RectangleShape panel_izquierdo(sf::Vector2f(ANCHO_PANEL_LATERAL - 10, ALTO_VENTANA - 2 * MARGEN_PANEL_VERTICAL));
    panel_izquierdo.setPosition(sf::Vector2f(5, MARGEN_PANEL_VERTICAL));
    panel_izquierdo.setFillColor(sf::Color(40, 40, 40, 200));
    panel_izquierdo.setOutlineThickness(2);
    panel_izquierdo.setOutlineColor(sf::Color(100, 100, 100));
    ventana.draw(panel_izquierdo);
    // Dibujar habilidades y energía Jugador X (panel izquierdo)
    ventana.draw(boton_intercambiar_color);
    ventana.draw(texto_boton_intercambiar_color);
    ventana.draw(boton_ficha_fantasma);
    ventana.draw(texto_boton_ficha_fantasma);
    ventana.draw(boton_volteo_forzado);
    ventana.draw(texto_boton_volteo_forzado);
    ventana.draw(texto_energia_jugador_x);
    
    // Dibujar panel derecho (Jugador 2)
    sf::RectangleShape panel_derecho(sf::Vector2f(ANCHO_PANEL_LATERAL - 10, ALTO_VENTANA - 2 * MARGEN_PANEL_VERTICAL));
    panel_derecho.setPosition(sf::Vector2f(ANCHO_VENTANA - ANCHO_PANEL_LATERAL + 5, MARGEN_PANEL_VERTICAL));
    panel_derecho.setFillColor(sf::Color(40, 40, 40, 200));
    panel_derecho.setOutlineThickness(2);
    panel_derecho.setOutlineColor(sf::Color(100, 100, 100));
    ventana.draw(panel_derecho);
    // Dibujar energía IA (panel derecho)
    ventana.draw(texto_energia_jugador_o);
    
    // Dibujar textos de estadísticas
    ventana.draw(texto_stats_jugador_x);
    ventana.draw(texto_stats_jugador_o);
    
    // Dibujar indicador de turno actual
    sf::CircleShape indicador_turno(15);
    indicador_turno.setOutlineThickness(3);
    indicador_turno.setOutlineColor(sf::Color::Yellow);
    
    if (gestor_juego.obtener_jugador_actual() == JUGADOR_X) {
        // Indicador en panel izquierdo
        indicador_turno.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X + 175, MARGEN_PANEL_VERTICAL + 20));
        indicador_turno.setFillColor(sf::Color::Black);
    } else {
        // Indicador en panel derecho
        indicador_turno.setPosition(sf::Vector2f(ANCHO_VENTANA - ANCHO_PANEL_LATERAL + MARGEN_BOTONES_HABILIDAD_X + 180, MARGEN_PANEL_VERTICAL + 20));
        indicador_turno.setFillColor(sf::Color::White);
    }
    
    // Solo mostrar indicador si el juego está en curso
    if (gestor_juego.obtener_estado_juego() == EN_CURSO) {
        ventana.draw(indicador_turno);
    }
}