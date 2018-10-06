#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define SIZE 20
#define BACK 4
#define SPEED 20
typedef struct MStack
{
 int x;
 int y;
 
}STACK;

typedef struct option
{
	float score;
	float hel;
	float frequency;
}OPTION;

//int speed = 0;

void F_initMap(int (*Maze)[SIZE]);
void initMap(int (*Maze)[SIZE]);
void displayMap(int (*Maze)[SIZE]);

OPTION mazeStart(int (*Maze)[SIZE], STACK* mStack, int* top);

int currentPosition(int (*Maze)[SIZE], int* x, int* y);
void push(STACK* mStack, int* top, int x, int y);
struct MStack pop(struct MStack* mStack, int *top);
int mazeReStart(int (*Maze)[SIZE], struct MStack* mStack, int* top);
void gotoxy(int x, int y);

int main()
{
 int Maze[SIZE][SIZE];
 struct MStack mStack[SIZE*SIZE];
 int F_score, L_score;
 int top = -1;
 OPTION t;

/*
 printf("미로 탈출 이미지 속도 (100 = 1 초, 1000 = 10초) ");
 fflush(stdin); 
 scanf("%d ", &speed);*/
 
 initMap(Maze);    //초기 미로를 보여 주기 위한 함수   
 displayMap(Maze); //초기 미로를 보여주기위한 함수 
 gotoxy(0,SIZE+1);
 

 initMap(Maze);
 puts(" 경로 학습 시작 ");
 system("pause");
 system("cls"); 
 
 t = mazeStart(Maze, mStack, &top);       
 gotoxy(0,SIZE+1);
 printf("경로 학습 완료\n비용(길의 총비용) = %10.1f 캔디를 통하여 획득한 HP %1.1f 캔디를 먹은 횟수 %10.1f \n", t.score, t.hel, t.frequency);
 system("pause");
 system("cls");
 
  
 initMap(Maze);
 displayMap(Maze);
 L_score = mazeReStart(Maze, mStack, &top);
    
 gotoxy(0,SIZE+1);
 printf("경로 학습 성공\n총비용 = %d \n", L_score);
 system("pause");
 
}

/*미로를 초기화 시키기위하여 사용한 함수 
0 : 지나가기전 2: 지가간후 1: 벽 3 경로 탐색 4 되돌아감 */
void initMap(int (*Maze)[SIZE])
{
 int i,j;

 int MAP[SIZE][SIZE] = 
 {
  {1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,5,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1},
  {1,5,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,1},
  {1,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,1,1,1,0,1},
  {1,0,1,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1},
  {1,0,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,1,0,1},
  {1,0,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,1},
  {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1},
  {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,1},
  {1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0},
  {1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1},
  {1,0,1,0,1,0,1,1,1,1,0,1,1,0,1,0,0,1,0,1},
  {1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,1,1,1},
  {1,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1},
  {1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1},
  {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
 };

 for(i=0; i<SIZE; i++)
 {
  for(j=0; j<SIZE; j++)
  {
   Maze[i][j] = MAP[i][j];
  }
 }
}

//맵을 표시하기위한 함수 
void displayMap(int (*Maze)[SIZE])
{
 int i, j;
 gotoxy(0,0);
 for(i=0; i<SIZE; i++)
 {
  for(j=0; j<SIZE; j++)
  {
   switch(Maze[i][j])
   {
   case 0:
    printf("  ");
    break;
   case 1:
    printf("■");
    break;
   case 2:
    printf("  ");
    break;
   case 3:
   case BACK:   // pop
    printf("★");
    break;
   case 5:
    printf("! ");
   default:
    break;
   }
  }
  puts("");
 }
}


OPTION mazeStart(int (*Maze)[SIZE], STACK* mStack, int* top)
{
 int x, y;
 STACK tmpStack;
 OPTION toption;
 toption.hel =0; toption.score = 0, toption.frequency = 0;
 
 tmpStack.x = -1;
 tmpStack.y = -1;
 int i,j;
 
  for(i=0; i<SIZE; i++)
 {
  for(j=0; j<SIZE; j++)
  {
    if(Maze[x][y]== 5)
	{
		toption.frequency = toption.frequency +1;
		printf("%d ", toption.frequency);
	} 
  }
 }
 
 while(1)
 {
  toption.score++;
  if(!currentPosition(Maze, &x, &y))
  {
            if(x == 11 && y == 19)                      // 도착
            {
            	return toption;
                break;
            }       
   if(x > 0 && ((Maze[x-1][y] == 0) || (Maze[x-1][y]==5)))       // 상
   {
    if(Maze[x][y] == BACK) push(mStack, top, x, y);
    Maze[x][y] = 2;    
    Maze[--x][y] = 3;

    push(mStack, top, x, y);
   }
   else if(x < SIZE-1 && ((Maze[x+1][y] == 0) || (Maze[x+1][y]== 5)))  // 하
   {
    if(Maze[x][y] == BACK) push(mStack, top, x, y);
    Maze[x][y] = 2;    
    Maze[++x][y] = 3;
	push(mStack, top, x, y);
   }
   else if(y > 0 && ((Maze[x][y-1] == 0)|| (Maze[x][y-1] == 5)))      // 좌
   {
    if(Maze[x][y] == BACK) push(mStack, top, x, y);
    Maze[x][y] = 2;
    Maze[x][--y] = 3;
    push(mStack, top, x, y);
   }
   else if(y < SIZE-1 && ((Maze[x][y+1] == 0)|| (Maze[x][y+1] == 5) )) //우
   {
    if(Maze[x][y] == BACK) push(mStack, top, x, y);
    Maze[x][y] = 2;    
    Maze[x][++y] = 3;
    push(mStack, top, x, y);
   }
   else
   {
    Maze[x][y] = 2;
    tmpStack = pop(mStack, top);
    x = tmpStack.x;
    y = tmpStack.y;
    Maze[x][y] = BACK;
   }
   Sleep(SPEED);
   displayMap(Maze);
  }
 }
}

int currentPosition(int (*Maze)[SIZE], int* x, int* y)
{
 int i, j;
 for(i=0; i<SIZE; i++)
 {
  for(j=0; j<SIZE; j++)
  {
   if(Maze[i][j] == 3 || Maze[i][j] == BACK)
   {
    *x = i;
    *y = j;
    return 0;
   }
  }
 }
 return 1;
}



void push(STACK* mStack, int* top, int x, int y)
{
 if(*top < SIZE*SIZE-1) 
 {
  mStack[++*top].x = x;
  mStack[*top].y = y;
 }
 else puts("Stack Overflow");
}



STACK pop(STACK* mStack, int *top)
{
 if(*top < 0)
 {
  puts("Stack Underflow");
  mStack[SIZE*SIZE].x = -1;
  mStack[SIZE*SIZE].y = -1;
  return mStack[SIZE*SIZE];
 }
 else return mStack[(*top)--];
}

int mazeReStart(int (*Maze)[SIZE], struct MStack* mStack, int* top)
{
 int i;
 Maze[0][1] = 2;

 for(i=1; i<=*top; i++)
 {
   	
  Maze[mStack[i-1].x][mStack[i-1].y] = 2;  
  Maze[mStack[i].x][mStack[i].y] = 3;
  Sleep(SPEED);
  displayMap(Maze);
 }
 return i;
}

//방향을 나타내기위한 좌표 함수 
void gotoxy(int x, int y)
{
     COORD cd;
     cd.X=x;
     cd.Y=y;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
