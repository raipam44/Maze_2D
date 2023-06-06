#pragma once
#include <vector>
#include <characters.h>

class MazeGame
{

public:
  static bool IsNotGameOver;
  static const int MAX_ROWS = 10;
  static const int MAX_COLS = 10;
  static const int MIN_SPEED = 0;
  static const char WALL = '+';

  static char ProcessPlayerInput(void);
  static char UserInput(void);
  static char maze[MAX_ROWS][MAX_COLS];
  static bool MoveStatus(Player &, char);
  static void MovePlayer(Player &, char);
  static void DisplayFrameConsole(Player &);
  static void GameStatus(Player &);
  static bool DisplayCharacters(Player &, int, int);
 
  //static void MovingEnemy();
};
