#ifndef MOVE_H
#define MOVE_H

struct Move {
    int row;
    int col;

    bool operator==(const Move& other) const {
        return row == other.row && col == other.col;
    }
};

#endif 