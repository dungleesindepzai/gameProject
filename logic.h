#ifndef _LOGIC__H
#define _LOGIC__H
#define BOARD_SIZE 3
#define EMPTY_CELL ' '
#define O_CELL 'o'
#define X_CELL 'x'

struct Tictactoe {
    char board[BOARD_SIZE][BOARD_SIZE];
    char nextMove = X_CELL;
    int turn=0;

    void init() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++) board[i][j] = EMPTY_CELL;
    }
    void move(int row, int column) {
        if (row >= 0 && row < BOARD_SIZE &&
            column >= 0 && column < BOARD_SIZE && board[row][column] == EMPTY_CELL)
        {
            board[row][column] = nextMove;
            nextMove = (nextMove == O_CELL) ? X_CELL : O_CELL;
            turn++;
        }
    }
    bool win_check(char player)
    {
        for(int i=0;i<BOARD_SIZE;i++)
        {
            for(int j=0;j<BOARD_SIZE;j++)
            {
                if(board[i][j]!=player) break;
                if(j==BOARD_SIZE-1) return 1;
            }
        }
        for(int j=0;j<BOARD_SIZE;j++)
        {
            for(int i=0;i<BOARD_SIZE;i++)
            {
                if(board[i][j]!=player) break;
                if(i==BOARD_SIZE-1) return 1;
            }
        }
        for(int i=0;i<BOARD_SIZE;i++)
        {
            if(board[i][i]!=player) break;
            if(i==BOARD_SIZE-1) return 1;
        }
        for(int i=0;i<BOARD_SIZE;i++)
        {
            if(board[i][BOARD_SIZE-i-1]!=player) break;
            if(i==BOARD_SIZE-1) return 1;
        }
        return 0;
    }
};

#endif
