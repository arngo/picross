#include <iostream>
#include <vector>

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
        if (curr_sum == *curr_hint && *e != 'o') {
            curr_sum = 0;
            curr_hint++;
        } else if (curr_sum < *curr_hint && *e == 'o') {
            curr_sum++;
            if (curr_sum < *curr_hint && row.end()-e == 1) {
                return false;
            }
        } else if (curr_sum == 0 && *e != 'o' && row.end()-e > 1) {
            continue;
        } else {
            return false;
        }
    }
    return hints.end() - curr_hint == 1;
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

int main(int argc, char *argv[]) {
    std::vector<int> hint;
    if (argc < 2) {
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        hint.push_back(std::stoi(argv[i]));
    }
    std::vector<std::vector<char>> rows = {
        {'o','x','o','o','o','o','o','x','o','o'}, // 1 5 2
        {'o','o','o','o','o','o','o','x','o','o'}, // 7 2
        {'o','x','x','x','o','o','o','o','o','x','x','x','o','o'}, // 1 5 2
        {'o','x','x','x','o','o','x','o','o','x','x','x','o','o'}, // 1 2 2 2
        {'o','x','o','o','o','o','o','x','o','o','x','o'}, // 1 5 2 1
        {'x','x','x'}, // 0
        {'o','o','o'}, // 3
        {'o'}, // 1
        {'x'}, // 0
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
    return 0;
}
