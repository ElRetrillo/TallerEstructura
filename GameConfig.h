
#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H


const int BOARD_SIZE = 8;
const char EMPTY = '.';
const char PLAYER_X = 'X'; 
const char PLAYER_O = 'O'; 


enum GameState {
    RUNNING,
    PLAYER_X_WON,
    PLAYER_O_WON,
    DRAW
};


enum PlayerMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI
};


const int GUI_BOARD_OFFSET_X = 50;
const int GUI_BOARD_OFFSET_Y = 50;
const int CELL_SIZE = 70;
const int WINDOW_WIDTH = GUI_BOARD_OFFSET_X * 2 + BOARD_SIZE * CELL_SIZE;
const int WINDOW_HEIGHT = GUI_BOARD_OFFSET_Y * 2 + BOARD_SIZE * CELL_SIZE + 100;


enum GuiState {
    MAIN_MENU,
    GAME_PLAYING,
    GAME_OVER_SCREEN
};

#endif