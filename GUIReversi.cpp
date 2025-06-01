#include "GUIReversi.h"
#include <iostream>
#include <SFML/System.hpp> 
#include <string>

GUIReversi::GUIReversi(GestorJuego& gm) :
    // MIEMBROS: Inicializar en el ORDEN EXACTO de declaración en GUIReversi.h
    ventana(sf::VideoMode(sf::Vector2u(ANCHO_VENTANA, ALTO_VENTANA)), sf::String(L"Reversi (Othello)")),
    gestor_juego(gm),

    fuente(), // sf::Font

    // Textura y Sprite (deben inicializarse antes de los otros elementos que los usen)
    textura_fondo_fin_juego(), // Inicializar la textura por defecto
    sprite_fondo_fin_juego(textura_fondo_fin_juego), // Inicializar el sprite con la textura

    // sf::Text (inicializados con la fuente)
    texto_estado(fuente),
    texto_boton_reiniciar(fuente),
    titulo_menu(fuente),
    texto_boton_jugar_humano(fuente),
    texto_boton_volver_menu(fuente),
    texto_boton_intercambiar_color(fuente),
    texto_boton_ficha_fantasma(fuente),

    // sf::RectangleShape (inicializados con su constructor por defecto)
    boton_reiniciar(), 
    boton_jugar_humano(), 
    boton_volver_menu(), 
    boton_intercambiar_color(),
    boton_ficha_fantasma(),

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
    ajustar_vista(ANCHO_VENTANA, ALTO_VENTANA);
}

void GUIReversi::inicializar_graficos() {
    if (!fuente.openFromFile("arial.ttf")) {
        std::cerr << "Error cargando fuente arial.ttf" << std::endl;
    }
     if (!textura_fondo_fin_juego.loadFromFile("fondo_fin_juego.png")) {
        std::cerr << "Error al cargar la textura del fondo de fin de juego: fondo_fin_juego.png" << std::endl;
    }
    sprite_fondo_fin_juego.setTexture(textura_fondo_fin_juego);

    texto_estado.setCharacterSize(24);
    texto_estado.setFillColor(sf::Color::White);
    texto_estado.setPosition(sf::Vector2f(OFFSET_GUI_TABLERO_X, OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 20));

    boton_reiniciar.setSize(sf::Vector2f(120, 40));
    boton_reiniciar.setFillColor(sf::Color(100, 100, 100));
    boton_reiniciar.setPosition(sf::Vector2f(ANCHO_VENTANA - OFFSET_GUI_TABLERO_X - 120, OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 5)); 
    texto_boton_reiniciar.setCharacterSize(20);
    texto_boton_reiniciar.setFillColor(sf::Color::White);
    texto_boton_reiniciar.setString(sf::String(L"Reiniciar"));
    sf::FloatRect limitesTexto = texto_boton_reiniciar.getLocalBounds();
    texto_boton_reiniciar.setOrigin(sf::Vector2f(limitesTexto.size.x / 2.0f, limitesTexto.size.y / 2.0f));
    texto_boton_reiniciar.setPosition(sf::Vector2f(boton_reiniciar.getPosition().x + boton_reiniciar.getSize().x / 2.0f,
                                            boton_reiniciar.getPosition().y + boton_reiniciar.getSize().y / 2.0f));

    titulo_menu.setString(sf::String(L"REVERSI"));
    titulo_menu.setCharacterSize(60);
    titulo_menu.setFillColor(sf::Color::White);
    titulo_menu.setOrigin(sf::Vector2f(titulo_menu.getLocalBounds().size.x / 2.0f, titulo_menu.getLocalBounds().size.y / 2.0f));
    titulo_menu.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 150.0f));

    boton_jugar_humano.setSize(sf::Vector2f(250, 60));
    boton_jugar_humano.setFillColor(sf::Color(50, 50, 150));
    boton_jugar_humano.setOrigin(sf::Vector2f(boton_jugar_humano.getLocalBounds().size.x / 2.0f, boton_jugar_humano.getLocalBounds().size.y / 2.0f));
    boton_jugar_humano.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 50.0f));
    texto_boton_jugar_humano.setString(sf::String(L"Jugar 1 vs 1"));
    texto_boton_jugar_humano.setCharacterSize(24);
    texto_boton_jugar_humano.setFillColor(sf::Color::White);
    texto_boton_jugar_humano.setOrigin(sf::Vector2f(texto_boton_jugar_humano.getLocalBounds().size.x / 2.0f, texto_boton_jugar_humano.getLocalBounds().size.y / 2.0f));
    texto_boton_jugar_humano.setPosition(boton_jugar_humano.getPosition());

    boton_volver_menu.setSize(sf::Vector2f(200, 40));
    boton_volver_menu.setFillColor(sf::Color(150, 50, 50));
    boton_volver_menu.setOrigin(sf::Vector2f(boton_volver_menu.getLocalBounds().size.x / 2.0f, boton_volver_menu.getLocalBounds().size.y / 2.0f));
    boton_volver_menu.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA - 50.0f));
    texto_boton_volver_menu.setString(sf::String(L"Volver al Menú"));
    texto_boton_volver_menu.setCharacterSize(20);
    texto_boton_volver_menu.setFillColor(sf::Color::White);
    texto_boton_volver_menu.setOrigin(sf::Vector2f(texto_boton_volver_menu.getLocalBounds().size.x / 2.0f, texto_boton_volver_menu.getLocalBounds().size.y / 2.0f));
    texto_boton_volver_menu.setPosition(boton_volver_menu.getPosition());

    float offset_boton_habilidad_y = OFFSET_GUI_TABLERO_Y + TAMANO_TABLERO * TAMANO_CELDA + 100;

    boton_intercambiar_color.setSize(sf::Vector2f(180, 40));
    boton_intercambiar_color.setFillColor(sf::Color(200, 100, 0));
    boton_intercambiar_color.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, offset_boton_habilidad_y));
    texto_boton_intercambiar_color.setString(sf::String(L"Intercambiar Color"));
    texto_boton_intercambiar_color.setCharacterSize(18);
    texto_boton_intercambiar_color.setFillColor(sf::Color::White);
    sf::FloatRect limitesTextoIntercambio = texto_boton_intercambiar_color.getLocalBounds();
    texto_boton_intercambiar_color.setOrigin(sf::Vector2f(limitesTextoIntercambio.size.x / 2.0f, limitesTextoIntercambio.size.y / 2.0f));
    texto_boton_intercambiar_color.setPosition(sf::Vector2f(boton_intercambiar_color.getPosition().x + boton_intercambiar_color.getSize().x / 2.0f,
                                      boton_intercambiar_color.getPosition().y + boton_intercambiar_color.getSize().y / 2.0f));

    boton_ficha_fantasma.setSize(sf::Vector2f(150, 40));
    boton_ficha_fantasma.setFillColor(sf::Color(100, 50, 150));
    boton_ficha_fantasma.setPosition(sf::Vector2f(MARGEN_BOTONES_HABILIDAD_X, offset_boton_habilidad_y + boton_intercambiar_color.getSize().y + 10));
    texto_boton_ficha_fantasma.setString(sf::String(L"Ficha Fantasma"));
    texto_boton_ficha_fantasma.setCharacterSize(18);
    texto_boton_ficha_fantasma.setFillColor(sf::Color::White);
    sf::FloatRect limitesTextoFantasma = texto_boton_ficha_fantasma.getLocalBounds();
    texto_boton_ficha_fantasma.setOrigin(sf::Vector2f(limitesTextoFantasma.size.x / 2.0f, limitesTextoFantasma.size.y / 2.0f));
    texto_boton_ficha_fantasma.setPosition(sf::Vector2f(boton_ficha_fantasma.getPosition().x + boton_ficha_fantasma.getSize().x / 2.0f,
                                      boton_ficha_fantasma.getPosition().y + boton_ficha_fantasma.getSize().y / 2.0f));

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
}

void GUIReversi::ejecutar() {
    while (ventana.isOpen()) {
        procesar_eventos();
        actualizar();
        renderizar();
    }
}

void GUIReversi::procesar_eventos() {
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

                int columna = static_cast<int>((pos_raton_vista.x - OFFSET_GUI_TABLERO_X) / TAMANO_CELDA);
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
                    if (boton_jugar_humano.getGlobalBounds().contains(pos_raton_vista)) {
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
                        if (gestor_juego.puede_intercambiar_color(gestor_juego.obtener_jugador_actual())) {
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
                            std::cout << "Ya usaste la habilidad de Intercambio de Color." << std::endl;
                        }
                        return;
                    }

                    if (boton_ficha_fantasma.getGlobalBounds().contains(pos_raton_vista)) {
                        if (estado_habilidad_actual == NINGUNA_HABILIDAD) {
                            estado_habilidad_actual = MODO_FICHA_FANTASMA;
                            std::cout << "Modo Ficha Fantasma activado. Mueve el ratón para previsualizar." << std::endl;
                        } else if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
                            estado_habilidad_actual = NINGUNA_HABILIDAD;
                            mostrar_previsualizacion_fantasma = false;
                            std::cout << "Modo Ficha Fantasma desactivado." << std::endl;
                        } else {
                            estado_habilidad_actual = MODO_FICHA_FANTASMA;
                            std::cout << "Modo Ficha Fantasma activado (desactivando otra habilidad)." << std::endl;
                        }
                        return;
                    }

                    manejar_clic_tablero(
                        static_cast<int>((pos_raton_vista.y - OFFSET_GUI_TABLERO_Y) / TAMANO_CELDA),
                        static_cast<int>((pos_raton_vista.x - OFFSET_GUI_TABLERO_X) / TAMANO_CELDA)
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
        if (gestor_juego.realizar_intercambio_color(fila, columna, gestor_juego.obtener_jugador_actual())) {
            gestor_juego.usar_intercambio_color(gestor_juego.obtener_jugador_actual());
            estado_habilidad_actual = NINGUNA_HABILIDAD;
            gestor_juego.actualizar_estado_juego();
            gestor_juego.cambiar_turno();
            std::cout << "Intercambio de color realizado en: " << fila + 1 << ", " << columna + 1 << std::endl;
        } else {
            std::cout << "Movimiento de Intercambio de Color invalido (debe ser una ficha del oponente)." << std::endl;
        }
    } else if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
        estado_habilidad_actual = NINGUNA_HABILIDAD;
        mostrar_previsualizacion_fantasma = false;
        std::cout << "Ficha Fantasma desactivada. Haz un movimiento normal." << std::endl;
    } else { 
        if (gestor_juego.obtener_estado_juego() == EN_CURSO) {
            bool movimiento_exitoso = gestor_juego.intentar_mover_jugador(fila, columna);
            if (movimiento_exitoso) {
                std::cout << "Jugador " << gestor_juego.obtener_jugador_actual() << " hizo movimiento en: " << fila + 1 << ", " << columna + 1 << std::endl;
            }
        }
    }
}

void GUIReversi::actualizar() {

    if (gestor_juego.obtener_estado_juego() != EN_CURSO) {
        estado_gui = PANTALLA_FIN_JUEGO;
    }
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) {
        actualizar_texto_estado();
    }
}

void GUIReversi::renderizar() {
    ventana.clear(sf::Color(50, 50, 50));
    ventana.setView(ventana.getView());

    if (estado_gui == MENU_PRINCIPAL) {
        dibujar_menu();
    } else if (estado_gui == JUGANDO) {
        dibujar_pantalla_juego();
        dibujar_ficha_fantasma();
        dibujar_boton_intercambiar_color();
        dibujar_boton_ficha_fantasma();
    } else if (estado_gui == PANTALLA_FIN_JUEGO) {
        dibujar_pantalla_juego();
        dibujar_pantalla_fin_juego();
        dibujar_boton_intercambiar_color();
        dibujar_boton_ficha_fantasma();
    }

    ventana.display();
}

void GUIReversi::dibujar_menu() {
    ventana.draw(titulo_menu);
    // Solo un botón de inicio: Jugar 1 vs 1
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
            celda.setPosition(sf::Vector2f(OFFSET_GUI_TABLERO_X + columna * TAMANO_CELDA, OFFSET_GUI_TABLERO_Y + fila * TAMANO_CELDA));

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
                ficha.setPosition(sf::Vector2f(OFFSET_GUI_TABLERO_X + columna * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + fila * TAMANO_CELDA + 5));
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
        forma_ficha_fantasma.setPosition(sf::Vector2f(OFFSET_GUI_TABLERO_X + columna_fantasma * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + fila_fantasma * TAMANO_CELDA + 5));
        if (gestor_juego.obtener_jugador_actual() == JUGADOR_X) {
            forma_ficha_fantasma.setFillColor(sf::Color(0, 0, 0, 100));
        } else {
            forma_ficha_fantasma.setFillColor(sf::Color(255, 255, 255, 100));
        }
        ventana.draw(forma_ficha_fantasma);

        for (const auto& coords_celda : celdas_a_voltear_fantasma) {
            sf::CircleShape previsualizacion_a_voltear(static_cast<float>(TAMANO_CELDA / 2 - 5));
            previsualizacion_a_voltear.setPosition(sf::Vector2f(OFFSET_GUI_TABLERO_X + coords_celda.second * TAMANO_CELDA + 5, OFFSET_GUI_TABLERO_Y + coords_celda.first * TAMANO_CELDA + 5));
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
    if (gestor_juego.puede_intercambiar_color(gestor_juego.obtener_jugador_actual()) || estado_habilidad_actual == MODO_INTERCAMBIO_COLOR) {
        if (!gestor_juego.puede_intercambiar_color(gestor_juego.obtener_jugador_actual())) {
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
        }
        
        ventana.draw(boton_intercambiar_color);
        ventana.draw(texto_boton_intercambiar_color);
    }
}

void GUIReversi::dibujar_boton_ficha_fantasma() {
    if (estado_gui == JUGANDO || estado_gui == PANTALLA_FIN_JUEGO) {
        boton_ficha_fantasma.setFillColor(sf::Color(100, 50, 150));

        if (estado_habilidad_actual == MODO_FICHA_FANTASMA) {
            boton_ficha_fantasma.setOutlineThickness(3);
            boton_ficha_fantasma.setOutlineColor(sf::Color::Yellow);
        } else {
            boton_ficha_fantasma.setOutlineThickness(0);
        }

        ventana.draw(boton_ficha_fantasma);
        ventana.draw(texto_boton_ficha_fantasma);
    }
}

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
    mensaje_final.setCharacterSize(22);
    mensaje_final.setFillColor(sf::Color::White);
    mensaje_final.setString(texto_estado.getString());

    mensaje_final.setOrigin(sf::Vector2f(mensaje_final.getLocalBounds().size.x / 2.0f, mensaje_final.getLocalBounds().size.y / 2.0f));
    mensaje_final.setPosition(sf::Vector2f(ANCHO_VENTANA / 2.0f, ALTO_VENTANA / 2.0f - 50.0f));

    ventana.draw(superposicion);
    ventana.draw(mensaje_final);
    ventana.draw(boton_volver_menu);
    ventana.draw(texto_boton_volver_menu);
    ventana.draw(boton_reiniciar);
    ventana.draw(texto_boton_reiniciar);
}