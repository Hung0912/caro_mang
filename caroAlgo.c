#include <stdio.h>
#include "Algo.h"
#define COLUMN 17
#define ROW 20

/*void initBoard(int board[COLUMN][ROW]){
  int i,j;
  for (i = 0;i < COLUMN;i++) {
    for (j = 0;j < ROW;j++) 
      board[i][j] = 0;
  }
  }*/
void test(char* i){
  printf("%s",i);
}

int checkVerticalLine(int player,int x,int y,int board[COLUMN][ROW]) {
  int j,count=1;
  
  j=y+1;

  while (board[x][j] == player && j < ROW) {
    printf("%d\n",count);
    count++;
    if (count == 5) return 1;
    j++;
  }
  
  j=y-1;
  while (board[x][j] == player && j >= 0) {
    count++;
    if (count == 5) return 1;
    j--;
  }
  return 0;
}

int checkHorizontalLine(int player,int x,int y,int board[COLUMN][ROW]) {
  int i,count=1;
  
  i=x+1;
  while (board[i][y] == player && i < COLUMN) {
    count++;
    if (count == 5) return 1;
    i++;
  }

  i=x-1;
  while (board[i][y] == player && i >= 0) {
    count++;
    if (count == 5) return 1;
    i--;
  }
  return 0;
}
  
int checkDiagonallyLine1(int player,int x,int y,int board[COLUMN][ROW]) {
  int i,j,count=1;
  
  i=x+1;
  j=y-1;

  while (board[i][j] == player && j >= 0 && i < COLUMN) {
    count++;
    if (count == 5) return 1;
    i++;j--;
  }

  i=x-1;j=y+1;
  while (board[i][j] == player && i >= 0 && j < ROW) {
    count++;
    if (count == 5) return 1;
    i--;j++;
  }
  return 0;
}

int checkDiagonallyLine2(int player,int x,int y,int board[COLUMN][ROW]) {
  int i,j,count=1;
  
  i=x+1;
  j=y+1;

  while (board[i][j] == player && j < COLUMN && i < ROW) {
    count++;
    if (count == 5) return 1;
    i++;j++;
  }
  
  i=x-1;j=y-1;
  while (board[i][j] == player && i >= 0 && j >= 0) {
    count++;
    if (count == 5) return 1;
    i--;j--;
  }
  return 0;
}

int checkWinning(int player,int x,int y,int board[COLUMN][ROW]) {
  if (
      checkVerticalLine(player,x,y,board)==1 ||
      checkHorizontalLine(player,x,y,board)==1 ||
      checkDiagonallyLine1(player,x,y,board)==1 ||
      checkDiagonallyLine2(player,x,y,board)==1 )
    return 1;
  return 0;
}

// int main() {
//   int komoku[COLUMN][ROW];
//   int i,j,x,y;
//   //Tao lap ban co trong
//   for (i = 0;i < COLUMN;i++) {
//     for (j = 0;j < ROW;j++) 
//       komoku[i][j] = 0;
//   }
//   while(1) {
//     printf("Nguoi choi A: \n");
//     printf("Nhap toa do x: ");
//     scanf("%d",&x);
//     printf("\nNhap toa do y: ");
//     scanf("%d",&y);    
//     komoku[x][y] = 1;
//     if (checkWinning(1,x,y,komoku) == 1)
//       break;

//     printf("Nguoi choi B: \n");
//     printf("Nhap toa do x: ");
//     scanf("%d",&x);
//     printf("\nNhap toa do y: ");
//     scanf("%d",&y);    
//     komoku[x][y] = 2;
//     if (checkWinning(2,x,y,komoku) == 1)
//     break;
//   }
//   return 1;
// }

