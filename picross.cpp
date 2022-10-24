#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "picrosslevel.h"
#include "raylib.h"

bool isValidRow(std::vector<char>& row, std::vector<int>& hints) {
    std::vector<int>::iterator curr_hint = hints.begin();
    int curr_sum = 0;
    for (auto e = row.begin(); e != row.end(); e++) {
        if (*curr_hint == 0) {
            if (*e == 'o') {
                return false;
            } else {
                continue;
            }
        }
        if (curr_sum == *curr_hint && *e != 'o') { // empty cell found after current hint fulfilled
            curr_sum = 0;
            curr_hint++;
        } else if (curr_sum < *curr_hint && *e == 'o') { // count filled cell for current hint
            curr_sum++;
            if (curr_sum < *curr_hint && row.end()-e == 1) { // end of row reached before filling hint
                return false;
            }
        } else if (curr_sum == 0 && *e != 'o' && row.end()-e > 1) { // empty cell found before end of row and between hints
            continue;
        } else {
            return false;
        }
    }
    // if 'o' found at end, curr_hint end up at the last hint rather than incrementing to hints.end()
    return curr_hint == hints.end() || (curr_sum == *curr_hint && hints.end() - curr_hint == 1);
}

bool isValidSolution(std::vector<std::vector<char>> board, std::vector<std::vector<int>> row_hints, std::vector<std::vector<int>> col_hints) {
    int rows = row_hints.size();
    int cols = col_hints.size();
    std::vector<char> col;

    for (int i = 0; i < rows; i++) {
        if (!isValidRow(board[i], row_hints[i])) {
            return false;
        }
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            col[j] = board[j][i];
        }
        if (!isValidRow(col, col_hints[i])) {
            return false;
        }
    }
    return true;
}

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
    }
    for (int i = 0; i <= rows; i++) {
        int y = posY + topRuleHeight + i * cellSize;
        DrawLine(posX, y, posX + levelWidth, y, BLACK);
    }
}

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        std::vector<int> hint;
        for (int i = 1; i < argc; i++) {
            hint.push_back(std::stoi(argv[i]));
        }
        std::vector<std::vector<char>> rows = {
            {'o','x','o','o','o','o','o','x','o','o'}, // 1 5 2
            {'o','x','x','x','o','o','o','o','o','x','x','x','o','o'}, // 1 5 2
            {'o','x','o','o','o','o','o','x','o','o','x','o'}, // 1 5 2 1
            {'o','x','x','x','o','o','x','o','o','x','x','x','o','o'}, // 1 2 2 2
            {'o','o','o','o','o','o','o','x','o','o'}, // 7 2
            {'o','o','o','o','o','x','x','x','o','o'}, // 5 2
            {'o','x','o'}, // 1 1
            {'o','x','x','o'}, // 1 1
            {'o','o','o'}, // 3
            {'x','o','o','o'}, // 3
            {'o','o','o','x'}, // 3
            {'x','x','o','o','o'}, // 3
            {'o','o','o','x','x'}, // 3
            {'x','x','x','o'}, // 1
            {'o','x','x','x'}, // 1
            {'x','o','x'}, // 1
            {'o'}, // 1
            {'x','x','x'}, // 0
            {'x'}, // 0
            {'.',' ','x'}, // 0
        };
        std::cout << "Hint:";
        for (auto &e : hint) {
            std::cout << " " << e;
        }
        std::cout << "\n";
        for (auto &r : rows) {
            std::cout << "Row: ";
            for (auto &e : r) {
                std::cout << e;
            }
            std::cout << "\n" << isValidRow(r, hint) << "\n";
        }
    }

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
