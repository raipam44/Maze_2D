#include <iostream>
#include "maze.h"
#include <characters.h>
#include <limits>
#include <windows.h>
#include <conio.h>
#include <path_finding.h>

/*
    approached the movement of enemy is wrong,
    fix that first, before I continue;
    implement a something like a pac-man
*/

int main()
{

    HANDLE InputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD PrevMode;
    GetConsoleMode(InputHandle, &PrevMode);
    SetConsoleMode(InputHandle, ENABLE_EXTENDED_FLAGS | (PrevMode & ~ENABLE_QUICK_EDIT_MODE));

    MazeGame game;
    Player player;
    char foo;

    int CountFrames = 1;

    std::cout << "Press WASD or ARROW KEYS, ESC to quit!\n";

    game.DisplayFrameConsole(player);
    do
    {
        std::cout << CountFrames << '\n';
        foo = game.ProcessPlayerInput();
        std::cout << '\n';

        if (foo == VK_ESCAPE)
        {
            std::cout << "Thanks for stopping by!\n";
            break;
        }
        else
        {

            game.MovePlayer(player, foo);
        }

        Monster::MovingMonster(player);
        game.DisplayFrameConsole(player);

        // Sleep(0) allow other threads to run

        CountFrames++;

    } while (game.IsNotGameOver);
    system("pause");
}
