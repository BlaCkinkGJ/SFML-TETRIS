#ifndef TETRIS_HPP
#define TETRIS_HPP
#include<iostream>
using namespace std;

void show(const int value[], const int& row, const int& col);
void show(const int value[], const int& row, const int& row_gap, const int& col, const int& col_gap);
void memcpy(const int* src, int* dst, const unsigned int size);

typedef struct _POINT{
    int x;
    int y;
}Point;

typedef struct _BOUND{
    int width;
    int height;
}Bound;

enum class Shape{
    I, O, T, S, Z, J, L
};

enum class Key{
    Rotate, Down, Left, Right, Nothing
};

class Board{
    public:
        static const int col = 4;
        static const int row = 4;
        const int shape[7][row * col] = {
            { // I case
                0, 0, 1, 0,
                0, 0, 1, 0,
                0, 0, 2, 0,
                0, 0, 1, 0
            },
            { // O case
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 1, 2, 0,
                0, 0, 0, 0
            },
            { // T case
                0, 0, 0, 0,
                1, 2, 1, 0,
                0, 1, 0, 0,
                0, 0, 0, 0,
            },
            { // S case
                0, 0, 0, 0,
                0, 0, 1, 1,
                0, 1, 2, 0,
                0, 0, 0, 0,
            },
            { // Z case
                0, 0, 0, 0,
                0, 1, 1, 0,
                0, 0, 2, 1,
                0, 0, 0, 0
            },
            { // J case
                0, 0, 0, 0,
                0, 0, 1, 0,
                0, 0, 2, 0,
                0, 1, 1, 0
            },
            { // L case
                0, 0, 0, 0,
                0, 1, 0, 0,
                0, 2, 0, 0,
                0, 1, 1, 0
            }
        };
};

namespace Move{
        Point getOrigin(const int block[], const int& row, const int& col);
        const static bool left = false;
        const static bool right = true;
        int* turn(const int ref[], const int& row, const int& col, const bool& state);
        Point shift(const Point& pos, const bool& state, const int& min, const int& max);
        Point down(const Point& pos, const int& max, const int& speed);
};



class Tetris{
    private:
        // block's size
        const int col = 4;
        const int row = 4;

        const Point start = {1, 0};
        Point curPos = {1, 1};
        Bound bound;

        int* block; // store the current block
        int* board; // board size
        bool isExist = false; // block이 현재 존재하는 가?
        bool isOver = false;
    public:
        Tetris(const int& width, const int& height);
        ~Tetris();

        void genBlock(const int block[]);
        void update(Key input);

        const int* getBoard(void);
        const int getWidth(void);
        const int getHeight(void);
        const bool getExist(void);
        const bool getGameOver(void);
		const Point getPos(void);
		const int *getBlock(void);

    protected:
        void setBoard(const int block[], const Point& pos);
        void setBoard(const int& value, const int cmp[], const Point& pos);
        bool isNotCollide(const int block[], const Point& pos);
};
#endif
