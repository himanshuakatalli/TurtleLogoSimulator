#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#define MAX_WIDTH 12

/****************************
Structure declarations
****************************/
typedef struct _Cordinates{
  size_t X;
  size_t Y;
}Cordinates;

typedef struct _TurtleBoard{
  char playBoard[50][50];
  Cordinates obCurrent;
  Cordinates obInitial;
}TurtleBoard;

typedef struct _InstructionSequence{
  size_t sizeOfSequence;
  char **instructionSequence;
}Sequence;

enum _PenStatus{
  PENUP, PENDOWN
};

enum _Direction{
  NORTH, EAST, SOUTH, WEST
};

/***************************
Function declarations
***************************/
void initializeBoard(TurtleBoard *turtleBoardPtr);
void printBoard(TurtleBoard *turtleBoardPtr);
void displayBanner();
size_t startLogo(TurtleBoard *turtleBoardPtr);
size_t getMoveStepSize(char *instruction);
void askSequence(Sequence *sequencePtr);
void allocateSequenceArray(Sequence *sequencePtr);
enum _Direction turnTo(enum _Direction value, char *direction);
size_t checkOverflow(size_t moveStep, TurtleBoard *turtleBoardPtr, enum _Direction Direction);

/**************************
Main function
**************************/
int main(){
  TurtleBoard obTurtleBoard;
  size_t _GameControl = 1;

  displayBanner();

  while(_GameControl){
    initializeBoard(&obTurtleBoard);
    startLogo(&obTurtleBoard);
    printf("\n%s","0 - To quit or 1 - To continue ? ");
    scanf("%zu", &_GameControl);
  }
}

/*************************
Function definitions
*************************/
void initializeBoard(TurtleBoard *turtleBoardPtr){
  size_t controlA, controlB;
  for(controlA = 0; controlA < 50; controlA++){
    for(controlB = 0; controlB < 50; controlB++){
      turtleBoardPtr->playBoard[controlA][controlB] = 'O';
    }
  }
  turtleBoardPtr->obInitial.X = 25;
  turtleBoardPtr->obInitial.Y = 25;

  turtleBoardPtr->obCurrent.X = 25;
  turtleBoardPtr->obCurrent.Y = 25;
  turtleBoardPtr->playBoard[turtleBoardPtr->obInitial.X][turtleBoardPtr->obInitial.Y] = '+';
}

void printBoard(TurtleBoard *turtleBoardPtr){
  size_t controlA, controlB;
  for(controlA = 0; controlA < 50; controlA++){
    for(controlB = 0; controlB < 50; controlB++){
      printf("%c ", turtleBoardPtr->playBoard[controlA][controlB]);
    }
    printf("%s", "\n");
  }
}

void displayBanner(){
  puts("-----------------------");
  puts("Logo Turtle Simulator");
  puts("-----------------------");
  puts("Commands that can be issued are given along with example below : ");
  puts("PENUP, PENDOWN, TURNRIGHT, TURNLEFT, MOVE,X PRINT, END");
  puts("\nExample : ");
  puts("Following sequence of instructions results in a square .....");
  puts("PENDOWN");
  puts("MOVE,12");
  puts("TURNRIGHT");
  puts("MOVE,12");
  puts("TURNRIGHT");
  puts("MOVE,12");
  puts("TURNRIGHT");
  puts("MOVE,12");
  puts("PENUP");
  puts("PRINT");
  puts("END");
  puts("-----------------------");
}

size_t startLogo(TurtleBoard *turtleBoardPtr){
  Sequence obSequence;
  enum _PenStatus PenStatus;
  enum _Direction Direction = EAST;
  size_t controlA;
  size_t controlB;
  size_t moveStep;

  askSequence(&obSequence);
  for (controlA = 0; controlA < obSequence.sizeOfSequence; controlA++){
    if(strcmp(obSequence.instructionSequence[controlA],"PENUP") == 0){
      PenStatus = PENUP;
    } else if(strcmp(obSequence.instructionSequence[controlA],"PENDOWN") == 0){
      PenStatus = PENDOWN;
    } else if(strcmp(obSequence.instructionSequence[controlA],"TURNLEFT") == 0){
      Direction = turnTo(Direction,"LEFT");
    } else if(strcmp(obSequence.instructionSequence[controlA],"TURNRIGHT") == 0){
      Direction = turnTo(Direction,"RIGHT");
    } else if(strcmp(obSequence.instructionSequence[controlA],"PRINT") == 0){
      printBoard(turtleBoardPtr);
    } else if(strcmp(obSequence.instructionSequence[controlA],"END") == 0){
      return 1;
    } else if(strncmp(obSequence.instructionSequence[controlA],"MOVE",4) == 0){
      moveStep = getMoveStepSize(obSequence.instructionSequence[controlA]);
      if(checkOverflow(moveStep,turtleBoardPtr,Direction) == 1){
        puts("Invalid move size. Description : Area to be printed is out of bounds.");
      } else {
        switch (PenStatus) {
          case PENUP:
            switch (Direction) {
              case NORTH:
                turtleBoardPtr->obCurrent.Y -= moveStep;
                break;
              case EAST:
                turtleBoardPtr->obCurrent.X += moveStep;
                break;
              case SOUTH:
                turtleBoardPtr->obCurrent.Y += moveStep;
                break;
              case WEST:
                turtleBoardPtr->obCurrent.X -= moveStep;
                break;
            }
            break;

          case PENDOWN:
            switch (Direction) {
              case NORTH:
                puts("North");
                for(controlB = turtleBoardPtr->obCurrent.X; controlB >= turtleBoardPtr->obCurrent.X - moveStep; controlB--){
                  turtleBoardPtr->playBoard[controlB][turtleBoardPtr->obCurrent.Y] = '+';
                }
                turtleBoardPtr->obCurrent.X -= moveStep;
                break;
              case EAST:
                puts("EAST");
                for(controlB = turtleBoardPtr->obCurrent.Y; controlB <= turtleBoardPtr->obCurrent.Y + moveStep; controlB++){
                  turtleBoardPtr->playBoard[turtleBoardPtr->obCurrent.X][controlB] = '+';
                }
                turtleBoardPtr->obCurrent.Y += moveStep;
                break;
              case SOUTH:
                puts("SOUTH");
                for(controlB = turtleBoardPtr->obCurrent.X; controlB <= turtleBoardPtr->obCurrent.X + moveStep; controlB++){
                  turtleBoardPtr->playBoard[controlB][turtleBoardPtr->obCurrent.Y] = '+';
                }
                turtleBoardPtr->obCurrent.X += moveStep;
                break;
              case WEST:
                puts("West");
                for(controlB = turtleBoardPtr->obCurrent.Y; controlB >= turtleBoardPtr->obCurrent.Y - moveStep; controlB--){
                  turtleBoardPtr->playBoard[turtleBoardPtr->obCurrent.X][controlB] = '+';
                }
                turtleBoardPtr->obCurrent.Y -= moveStep;
                break;
            }
            break;
        }
      }
    } else{
      printf("%s %s %s","Invalid instruction entered. Description : ", obSequence.instructionSequence[controlA], " is an unknown instruction.");
      return 0;
    }
  }
}

size_t checkOverflow(size_t moveStep, TurtleBoard *turtleBoardPtr, enum _Direction Direction){
  size_t xC, yC;
  xC = turtleBoardPtr->obCurrent.X;
  yC = turtleBoardPtr->obCurrent.Y;
  switch (Direction) {
    case NORTH:
      if((yC - moveStep) < 0){
        return 1;
      }
    case EAST:
      if((xC + moveStep) >= 50){
        return 1;
      }
    case SOUTH:
      if((yC + moveStep) >= 50){
        return 1;
      }
    case WEST:
      if((xC - moveStep) < 0){
        return 1;
      }
  }
  return 0;
}

enum _Direction turnTo(enum _Direction value, char *direction){
  if(strcmp(direction,"LEFT") == 0){
    switch (value) {
      case NORTH:
        return WEST;
      case WEST:
        return SOUTH;
      case SOUTH:
        return EAST;
      case EAST:
        return NORTH;
    }
  } else if(strcmp(direction,"RIGHT") == 0){
    switch(value){
      case NORTH:
        return EAST;
      case EAST:
        return SOUTH;
      case SOUTH:
        return WEST;
      case WEST:
        return NORTH;
    }
  }
}

size_t getMoveStepSize(char *instruction){
  instruction += 5;
  return atoi(instruction);
}

void askSequence(Sequence *sequencePtr){
  size_t controlA;
  printf("Enter number of instructions contained in sequence : ");
  scanf("%zu", &sequencePtr->sizeOfSequence);
  puts("Start entering your sequence of instructions : ");
  allocateSequenceArray(sequencePtr);
  for(controlA = 0; controlA < sequencePtr->sizeOfSequence; controlA++){
    scanf("%s",sequencePtr->instructionSequence[controlA]);
  }
}

void allocateSequenceArray(Sequence *sequencePtr){
  size_t controlA;
  sequencePtr->instructionSequence = (char**)malloc(sizeof(char *) * sequencePtr->sizeOfSequence);
  for(controlA = 0; controlA < sequencePtr->sizeOfSequence; controlA++){
    sequencePtr->instructionSequence[controlA] = (char*)malloc(sizeof(char) * MAX_WIDTH);
  }
}
