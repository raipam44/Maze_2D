
#include <vector>
#include <iostream>
#include <maze.h>
#include <queue>
#include <path_finding.h>
#include <limits>
#include <functional>

bool IsValidPath(const std::vector<std::vector<bool>>& visited, int row, int col)
{
    int numRows = MazeGame::MAX_ROWS;
    int numCols = MazeGame::MAX_COLS;
    return (row >= 0 && row < numRows && col >= 0 && col < numCols && MazeGame::maze[row][col] != MazeGame::WALL && !visited[row][col]);
}
std::vector<Cell> GetBestPath(const std::vector<std::vector<Cell>> &parents, int goalRow, int goalCol)
{
    std::vector<Cell> path;
    int row = goalRow;
    int col = goalCol;
    while (row != -1 && col != -1)
    {
        path.push_back({row, col, 0});
        int parentRow = parents[row][col].row;
        int parentCol = parents[row][col].col;
        row = parentRow;
        col = parentCol;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
std::vector<Cell> Dijkstra(int startRow, int startCol, int goalRow, int goalCol)
{
    std::vector<std::vector<bool>> visited(MazeGame::MAX_ROWS, std::vector<bool>(MazeGame::MAX_COLS, false));

    std::priority_queue<Cell, std::vector<Cell>, std::function<bool(const Cell &, const Cell &)>> pq(
        [](const Cell &cell1, const Cell &cell2)
        {
            // Compare cells based on their distance from the start cell
            return cell1.distance > cell2.distance;
        });

    std::vector<std::vector<int>> distances(MazeGame::MAX_ROWS, std::vector<int>(MazeGame::MAX_COLS, std::numeric_limits<int>::max()));
    std::vector<std::vector<Cell>> parents(MazeGame::MAX_ROWS, std::vector<Cell>(MazeGame::MAX_COLS, {-1, -1, 0}));

    pq.push({startRow, startCol, 0});
    distances[startRow][startCol] = 0;

    while (!pq.empty())
    {
        Cell currentCell = pq.top();
        pq.pop();

        int currentRow = currentCell.row;
        int currentCol = currentCell.col;

        visited[currentRow][currentCol] = true;

        if (currentRow == goalRow && currentCol == goalCol)
        {
            // Goal reached, construct and return the best path
            return GetBestPath(parents, goalRow, goalCol);
        }

        static int dr[] = {-1, 1, 0, 0};
        static int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++)
        {
            int newRow = currentRow + dr[i];
            int newCol = currentCol + dc[i];

            if (IsValidPath(visited, newRow, newCol))
            {
                int newDistance = currentCell.distance + 1;

                if (newDistance < distances[newRow][newCol])
                {
                    distances[newRow][newCol] = newDistance;
                    pq.push({newRow, newCol, newDistance});
                    parents[newRow][newCol] = currentCell;
                }
            }
        }
    }

    // No path found, return an empty vector
    return {};
}