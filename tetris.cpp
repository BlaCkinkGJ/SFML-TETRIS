#include "tetris.hpp"

void show(const int value[], const int& row, const int& col){
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < col; j++){
            value[i*col + j] > 0 ? cout << "1" : cout << "0";
        }
        cout << endl;
    }
    cout << endl;
}

void show(const int value[], const int& row, const int& row_gap, const int& col, const int& col_gap) {
    for(int i = row_gap; i < row ; i++){
        for(int j = col_gap; j < col; j++){
            value[i*col + j] > 0 ? cout << "1" : cout << "0";
        }
        cout << endl;
    }
}

void memcpy(const int* src, int* dst, const unsigned int size) {
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
}

Point Move::getOrigin(const int block[], const int& row, const int& col){
    Point result = {-1, -1};
    for(int y = 0; y < row; y++){
        for(int x = 0; x < col; x++){
            if(block[y*col + x] == 2){
                result = {x, y};
                return result;
            }
        }
    }
    if(result.x == -1 || result.y == -1)
        throw "Cannot find the focus point";
    return result;
}


int* Move::turn(const int ref[], const int& row, const int& col, const bool& state){
    int* block = new int[row * col];
	memcpy(ref, block, row*col);
    Point origin = getOrigin(block, row, col);
    bool* exist = new bool[row * col];
    bool* visit = new bool[row * col];
    // bool initialization
    for(int i = 0; i < row*col; i++)
        exist[i] = block[i] == 1 ? true : false;
    for(int i = 0; i < row*col; i++)
        visit[i] = false;
    // move the value
    for(int y = 0; y < col; y++){
        for(int x = 0; x < row; x++){
            bool isOrigin = ((origin.x == x) && (origin.y == y));
            bool isBlock = (block[y*col + x] == 1);
            bool isExist = exist[y*col + x];
            if(!isOrigin && isBlock && isExist){
                Point temp, slope;
                slope.x = state == right ? -1 : 1;
                slope.y = state == right ? 1 : -1;
                temp.x =  (slope.x * (y - origin.y)  + origin.x)%4;
                temp.y = (slope.y * (x - origin.x) + origin.y)%4;
                block[temp.y*col + temp.x] = block[y*col + x];
                visit[temp.y*col + temp.x] = true;
                if(visit[y*col + x] == false)
                    block[y*col + x] = 0;
            }
        }
    }
    return block;
}

Point Move::shift(const Point& pos, const bool& state, const int& min, const int& max){
    Point result = pos;
    result.x = state == left ? result.x - 1 : result.x + 1;
    if(result.x < min)
        result.x = min;
    if(result.x > max)
        result.x = max;
    return result;
}

Point Move::down(const Point& pos, const int& max, const int& speed){
    Point result = pos;
    result.y = result.y > max ? max : result.y + speed;
    return result;
}

Tetris::Tetris(const int& width, const int& height){
    Bound bound = {width + 2, height+ 2};
    this->bound = bound;
    // 2 for make bound
    board = new int[bound.width * bound.height];
    block = new int[row * col];

    // initialization
    for(int i = 0; i < bound.width * bound.height; i++)
        board[i] = 0;
    for(int i = 0; i < bound.width; i++)
        board[(bound.height - 1)*bound.width + i] = 3;
	for (int i = 0; i < row * col; i++)
		block[i] = 0;

    // make bound
    for(int i = 0; i < bound.height; i++){
        board[i*bound.width + 0] = 3;
        board[i*bound.width + bound.width-1] = 3;
    }
}

Tetris::~Tetris(){
    delete[] board;
    delete[] block;
}

void Tetris::genBlock(const int block[]){
    curPos = {-1, -1};
    for(int i = 0; i < row*col; i++)
        this->block[i] = 0;

    Point origin = {bound.width/2 + start.x - col, start.y};
    curPos = {origin.x, origin.y};
    for(int y = 0; y < row; y++){
        for(int x = 0; x < col; x++){
            int getBoardValue = this->board[(y+origin.y)*bound.width+ (x+origin.x)];
            this->block[y*col + x] = block[y*col + x];
            if(getBoardValue == 0 && block[y*col + x] > 0){
                this->board[(y+origin.y)*bound.width+ (x+origin.x)] = block[y*col + x];
            }
            if(getBoardValue > 0){
                isOver = true;
                return;
            }
        }
    }
    isExist = true;
}

void Tetris::update(Key input){
    static int counter = 0;
    Point cur = curPos;
    bool current_state = isExist;
	bool previous_state = current_state;
    int* block = new int[row * col];
    // wipe exist board value
    setBoard(0, this->block, cur);

    // copy the block value
    for(int i = 0; i < row * col; i++)
        block[i] = this->block[i];


    switch(input){
        case Key::Down:
        { // down twice
            Point temp = Move::down(cur, bound.height - 2, 2);

            current_state = isNotCollide(block, temp);
            if(current_state){ // true
                setBoard(block, temp);
                curPos = temp;
            }
			else {
				Point temp2 = Move::down(cur, bound.height - 2, 1);
				current_state = isNotCollide(block, temp2);
				if(current_state){ // true
					setBoard(block, temp2);
					curPos = temp2;
				}
			}
        }
        break;
        case Key::Left:
        {
            Point temp = Move::shift(cur, Move::left, start.x - 2, bound.width - 1);

            current_state = isNotCollide(block, temp);
            if(current_state){ // true
                setBoard(block, temp);
                curPos = temp;
            }
			else {
				setBoard(block, curPos);
			}
        }
        break;
        case Key::Right:
        {
            Point temp = Move::shift(cur, Move::right, start.x - 2, bound.width - 1);

            current_state = isNotCollide(block, temp);
            if(current_state){ // true
                setBoard(block, temp);
                curPos = temp;
            }
			else {
				setBoard(block, curPos);
			}
        }
        break;
        case Key::Rotate:
        {//slightly different
            int* temp = Move::turn(this->block, row, col, Move::right);

            current_state = isNotCollide(temp, curPos);
            if(current_state){ // true
                setBoard(temp, curPos);
				memcpy(temp, this->block, row*col);
            }
			else {
				setBoard(this->block, curPos);
			}
            delete temp;
            break;
        }
    }
	if (input != Key::Down) {
		cur = curPos;
		setBoard(0, this->block, cur);
		// copy the block value
		for(int i = 0; i < row * col; i++)
			block[i] = this->block[i];
		Point temp = Move::down(cur, bound.height - 1, 1);
		// bound check
		current_state = isNotCollide(block, temp);
		if (current_state) { // true
			setBoard(block, temp);
			curPos = temp;
		}
		else {
			setBoard(this->block, curPos);
		}
	}
    isExist = current_state;
    if(!current_state){
		if (input != Key::Left && input != Key::Right && input != Key::Rotate) {
			// copy the block value
			memcpy(this->block, block, row*col);

			// back up
			setBoard(this->block, curPos);
		}
		else {
			isExist = true;
		}
		for (int curY = bound.height - 2; curY > 0; curY--) {
			bool isClear = true;
			for (int x = start.x; x < bound.width - 1; x++)
				if (this->board[curY*bound.width + x] == 0)
					isClear = false;
			// clear the line
			if (isClear) {
				for (int y = curY - 1; y > -1; y--) {
					for (int x = start.x; x < bound.width - 1; x++) {
						this->board[(y + 1)*bound.width + x] = 0;
						this->board[(y + 1)*bound.width + x] = this->board[y*bound.width + x];
					}
				}
				curY = curY + 1;
				isExist = false;
			}
		}
	}	
    delete block;
}

const int* Tetris::getBoard(void){
    return board;
}

const int Tetris::getWidth(void){
    return bound.width;
}

const int Tetris::getHeight(void){
    return bound.height;
}

const bool Tetris::getExist(void){
    return isExist;
}

const bool Tetris::getGameOver(void){
    return isOver;
}

const Point Tetris::getPos(void) {
	return curPos;
}

const int* Tetris::getBlock(void) {
	return block;
}

void Tetris::setBoard(const int block[], const Point& pos){
    for(int y = 0; y < row; y++)
        for(int x = 0; x < col; x++)
            if(block[y*col + x] > 0)
                board[(y + pos.y)*bound.width+ (x + pos.x)] = block[y*col + x];
}

void Tetris::setBoard(const int& value, const int cmp[], const Point& pos){
        for(int y = 0; y < row; y++)
            for(int x = 0; x < col; x++)
                if(cmp[y*col + x] > 0)
                    board[(y + pos.y)*bound.width+ (x + pos.x)] = value;
}

bool Tetris::isNotCollide(const int block[], const Point& pos){
    for(int y = 0; y < row; y++){
        for(int x = 0; x < col; x++){
            int getBoardValue = board[(pos.y + y)*bound.width+ (pos.x + x)];
            if(getBoardValue > 0 &&  block[y*col + x] > 0)
                return false;
        }
    }
    return true;
}
