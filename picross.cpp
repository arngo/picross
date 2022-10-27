#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "picrosslevel.h"
#include "raylib.h"

void drawLevel(PicrossLevel level) {
    int cellSize = 20;
    int rows = level.getNumRows();
    int cols = level.getNumCols();

    // where rule numbers are displayed
    int leftRuleWidth = std::ceil(cols/2.0) * cellSize;
    int topRuleHeight = std::ceil(rows/2.0) * cellSize;
    // total w/h rules + cells
    int levelWidth = cols * cellSize + leftRuleWidth;
    int levelHeight = rows * cellSize + topRuleHeight;

    // TODO: use for scaling
    int maxAxis = std::max(rows, cols);
    int levelSize = maxAxis-1+5-((maxAxis-1)%5); // round up to multiple of 5
    int canvasWidth = std::ceil(levelSize * 1.5) * cellSize;
    int canvasHeight = std::ceil(levelSize * 1.5) * cellSize;
    Vector2 topLeft = {(GetScreenWidth()-canvasWidth)/2.0f, (GetScreenHeight()-canvasHeight)/2.0f};
    DrawRectangleLines(topLeft.x, topLeft.y, canvasWidth, canvasHeight, BLACK);

    int posX = (GetScreenWidth()-levelWidth)/2;
    int posY = (GetScreenHeight()-levelHeight)/2;

    for (int i = 0; i <= cols; i++) {
        int x = posX + leftRuleWidth + i * cellSize;
        DrawLine(x, posY, x, posY + levelHeight, BLACK);
        if (i < cols) {
            int lenRule = level.getColRules()[i].size();
            for (int j = 0; j < lenRule; j++) {
                std::string ruleNumStr = std::to_string(level.getColRules()[i][j]);
                DrawText(ruleNumStr.c_str(), x+2, posY + j*20, 20, BLACK);
            }
        }
    }
    for (int i = 0; i <= rows; i++) {
        int y = posY + topRuleHeight + i * cellSize;
        DrawLine(posX, y, posX + levelWidth, y, BLACK);
        if (i < rows) {
            int lenRule = level.getRowRules()[i].size();
            for (int j = 0; j < lenRule; j++) {
                std::string ruleNumStr = std::to_string(level.getRowRules()[i][j]);
                DrawText(ruleNumStr.c_str(), posX + j*20 + 4, y, 20, BLACK);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    PicrossLevel level("levels/test.txt");

    std::cout << "Row rules:" << std::endl;

    for (auto &r : level.getRowRules()) {
        for (auto &n : r) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Column rules:" << std::endl;

    for (auto &r : level.getColRules()) {
        for (auto &n : r) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    std::vector<std::pair<int, int>> toFill{ {0,1},{0,2},{0,3},{1,1},{1,3},{2,1},{2,2},{2,3},{3,2},{4,2},{4,3} };

    for (auto &it : toFill) {
        level.markCell(it.first, it.second, 'o');
        std::vector<std::vector<char>> board = level.getBoard();
        for (int row = 0; row < board.size(); row++) {
            std::vector<int> rule = level.getRowRule(row);
            for (auto &cell : board[row]) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    }

    std::cout << "level solved: " << level.isSolved() << std::endl;
    level.reset();
    std::cout << "level cleared." << std::endl;
    std::cout << "level solved: " << level.isSolved() << std::endl;

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Picross");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawLevel(level);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
