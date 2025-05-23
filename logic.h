#ifndef _LOGIC__H
#define _LOGIC__H
#define BOARD_SIZE 15
#define EMPTY_CELL ' '
#define O_CELL 'o'
#define X_CELL 'x'

struct Tictactoe {
    char board[BOARD_SIZE][BOARD_SIZE];
    char nextMove = X_CELL;
    int turn=1, End=0;

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

    void reset()
    {
        for(int i=0;i<BOARD_SIZE;i++)
            for(int j=0;j<BOARD_SIZE;j++)
                board[i][j]=EMPTY_CELL;
        turn=1;
        End=0;
        nextMove=X_CELL;
    }

    bool win_check(char player)
    {
        for(int i=0;i<BOARD_SIZE-4;i++)
        {
            for(int j=0;j<BOARD_SIZE-4;j++)
            {
                for(int x=0;x<5;x++)
                {
                    if(board[i][j+x]!=player) break;
                    if(x==4)
                    {
                        End=1;
                        return 1;
                    }
                }
                for(int x=0;x<5;x++)
                {
                    if(board[i+x][j]!=player) break;
                    if(x==4)
                    {
                        End=1;
                        return 1;
                    }
                }
                for(int x=0;x<5;x++)
                {
                    if(board[i+x][j+x]!=player) break;
                    if(x==4)
                    {
                        End=1;
                        return 1;
                    }
                }
            }
        }
        for(int i=4;i<BOARD_SIZE;i++)
        {
            for(int j=0;j<BOARD_SIZE-4;j++)
            {
                for(int x=0;x<5;x++)
                {
                    if(board[i-x][j+x]!=player) break;
                    if(x==4)
                    {
                        End=1;
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
};

#endif
