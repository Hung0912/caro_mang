#include <stdio.h>
#define COLUMN 17
#define ROW 20

int checkVerticalLine(int player,int x,int y,int board[COLUMN][ROW]);
int checkHorizontalLine(int player,int x,int y,int board[COLUMN][ROW]);
int checkDiagonallyLine1(int player,int x,int y,int board[COLUMN][ROW]);
int checkDiagonallyLine2(int player,int x,int y,int board[COLUMN][ROW]);
int checkWinning(int player,int x,int y,int board[COLUMN][ROW]);
void test(char* i);
