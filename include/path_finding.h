#pragma once

struct Cell
{
    int row;
    int col;
    int distance; // distance from the start cell
};

bool IsValidPath(const std::vector<std::vector<bool>>& visited, int row, int col);
std::vector<Cell> Dijkstra(int startRow, int startCol, int goalRow, int goalCol);
std::vector<Cell> GetBestPath(const std::vector<std::vector<Cell>> &parents, int goalRow, int goalCol);
