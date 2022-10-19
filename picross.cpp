#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/* level file format:
 * first line is 2 numbers, first num is number of rows, second is number of columns
 * remaining lines are the rules themselves
 * each rule is a separate line
 * row rules are listed first before column rules
 * row rules are top to bottom, column rules are left to right
 */
std::vector<std::vector<std::vector<int>>> loadLevel(const std::string &fileName) {
    std::vector<std::vector<std::vector<int>>> rules;
    std::ifstream levelFile(fileName);
    std::string line = "";

    int numRows, numCols, spacePos;
    std::getline(levelFile, line);
    spacePos = line.find(' ');
    numRows = std::stoi(line.substr(0, spacePos));
    numCols = std::stoi(line.substr(spacePos+1));
    std::vector<int> rowcol = {numRows, numCols};
    //std::cout << numRows << " x " << numCols << std::endl;

    for (int i = 0; i < 2; i++) {
        //std::cout << "row/col: " << i << std::endl;
        rules.push_back(std::vector<std::vector<int>>());
        for (int j = 0; j < rowcol[i]; j++) {
            //std::cout << "rule: " << j << std::endl;
            std::vector<int> rule;
            int pos = 0, len = 1;
            std::getline(levelFile, line);
            //std::cout << "line: " << line << std::endl;
            for (auto it = line.begin(); it != line.end(); it++) {
                if (*it == ' ' || it == line.end() - 1) {
                    std::string rulenum = line.substr(pos, len);
                    //std::cout << "adding to rule: " << rulenum << std::endl;
                    rule.push_back(std::stoi(rulenum));
                    pos += len;
                    len = 1;
                } else {
                    len++;
                }
            }
            rules[i].push_back(rule);
        }
    }

    levelFile.close();
    return rules;
}

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

    std::vector<std::vector<std::vector<int>>> board;

    board = loadLevel("levels/test.txt");

    std::cout << "Row rules:" << std::endl;

    for (auto &r : board[0]) {
        for (auto &n : r) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Column rules:" << std::endl;

    for (auto &r : board[1]) {
        for (auto &n : r) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
