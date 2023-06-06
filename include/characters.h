#pragma once

class Player
{
public:
    int playerRow = 1;
    int playerCol = 1;
    int speed = 1;
};

class Monster
{
public:
    static int MonsterRow;
    static int MonsterColumn;

    static void MovingMonster(Player&);
    static void Move(Player &player);
};
