# FlipTurn - Proyecto de Estructuras de Datos

## Descripción General

Este proyecto corresponde a una implementación del juego de mesa **Reversi (Othello)** desarrollado en **C++** utilizando la biblioteca gráfica **SFML**. La versión presentada está orientada al modo de juego entre **dos jugadores humanos**, con énfasis en el uso de **estructuras de datos fundamentales** y la incorporación de funcionalidades innovadoras que enriquecen la jugabilidad.

## Características del Juego

- **Modo de juego:** 1 vs 1 entre jugadores humanos.
- **Interfaz gráfica (GUI):** Diseñada con SFML. Responde dinámicamente al tamaño de la ventana.
- **Reglas estándar del juego:** Volteo de fichas y validación de movimientos implementadas correctamente.

### Funcionalidades especiales:

- **Dominio de esquinas:** Bonificación de puntos al final del juego por controlar esquinas del tablero.
- **Intercambio de color:** Habilidad de uso único que convierte una ficha enemiga en propia.
- **Ficha fantasma:** Visualiza las fichas que se voltearían al posicionar el cursor sobre una casilla válida.

## Estructuras de Datos Implementadas

- **Pilas (`std::stack`):** Para manejar el historial de estados del tablero (deshacer jugadas).
- **Colas (concepto):** Implementación tipo BFS al validar fichas a voltear, puede modelarse con una cola.
- **Árboles de decisión (implícitos):** Cada estado del tablero puede considerarse como un nodo de un árbol de decisiones.
- **Listas enlazadas dinámicas (`std::vector`):** Para manejar posibles movimientos y fichas a voltear.

## Arquitectura del Código

El proyecto está modularizado en múltiples clases, separando responsabilidades para facilitar su comprensión y mantenimiento.

### `ConfiguracionJuego.h`
- Contiene constantes globales, enumeraciones y configuraciones del juego.
- Define símbolos, tamaños y modos de juego.

### `Movimiento.h`
- Define la estructura `Movimiento`, con coordenadas de fila y columna.
- Incluye sobrecarga del operador `==` para comparar movimientos.

### `Tablero.h / Tablero.cpp`
- Maneja el estado del tablero y las reglas básicas del juego.
- Incluye funciones para validar y ejecutar movimientos, contar fichas y detectar fin del juego.

### `GestorJuego.h / GestorJuego.cpp`
- Controla la lógica central del juego, turnos y habilidades especiales.
- Implementa reversión de jugadas con pila.
- Determina el estado actual del juego y conecta con la interfaz gráfica.

### `GUIReversi.h / GUIReversi.cpp`
- Desarrollado con **SFML**.
- Administra eventos, renderizado del tablero, botones y textos.
- Permite visualización dinámica de fichas, turnos y uso de habilidades.

### `main.cpp`
- Punto de entrada del programa.
- Inicializa componentes y ejecuta el ciclo principal de juego.

## Conclusión

Este proyecto demuestra el uso aplicado de estructuras de datos en el contexto de un juego clásico con interfaz gráfica, integrando:
- Diseño modular,
- Lógica compleja de juego,
- Interacción con el usuario,
- Y un enfoque académico claro.

Además, constituye una base sólida para futuras extensiones, como la inclusión de Inteligencia Artificial o mejoras multijugador.

---

Desarrollado por Daniel Trigo y Julián Gallardo.