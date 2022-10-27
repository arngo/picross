#include "picrosslevel.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool isVectorSolved(std::vector<char> &v, std::vector<int> &hints) {
    std::vector<int>::iterator curr_hint = hints.begin();
    int curr_sum = 0;
    for (auto e = v.begin(); e != v.end(); e++) {
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
            if (curr_sum < *curr_hint && v.end()-e == 1) { // end of row reached before filling hint
                return false;
            }
        } else if (curr_sum == 0 && *e != 'o' && v.end()-e > 1) { // empty cell found before end of row and before next hint
            continue;
        } else if (curr_hint == hints.end()) {
            break;
        } else {
            return false;
        }
    }
    // if 'o' found at end, curr_hint end up at the last hint rather than incrementing to hints.end()
    return curr_hint == hints.end() || (curr_sum == hints.back() && hints.end() - curr_hint == 1);
}

void PicrossLevel::initLevel(const std::string &name, const std::vector<std::vector<int>> &rowRules, const std::vector<std::vector<int>> &colRules) {
    this->name = name;
    this->rowRules = rowRules;
    this->colRules = colRules;
    numRows = rowRules.size();
    numCols = colRules.size();
    board = std::vector<std::vector<char>>(numRows, std::vector<char>(numCols, '.'));
    solvedRows = std::vector<bool>(numRows, false);
    solvedCols = std::vector<bool>(numCols, false);
    for (int i = 0; i < numRows; i++) {
        if (rowRules[i].size() == 1 && rowRules[i][0] == 0) {
            solvedRows[i] = true;
        }
    }
    for (int i = 0; i < numCols; i++) {
        if (colRules[i].size() == 1 && colRules[i][0] == 0) {
            solvedCols[i] = true;
        }
    }
}

PicrossLevel::PicrossLevel(const std::string &name, const std::vector<std::vector<int>> &rowRules, const std::vector<std::vector<int>> &colRules) {
    initLevel(name, rowRules, colRules);
}

/* level file format:
 * first line is 2 numbers, first num is number of rows, second is number of columns
 * remaining lines are the rules themselves
 * each rule is a separate line
 * row rules are listed first before column rules
 * row rules are top to bottom, column rules are left to right
 */
PicrossLevel::PicrossLevel(const std::string &filename) {
    std::vector<std::vector<std::vector<int>>> rules;
    std::ifstream levelFile(filename);
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

    name = "";
    initLevel(name, rules[0], rules[1]);
}

int PicrossLevel::getNumRows() {
    return numRows;
}

int PicrossLevel::getNumCols() {
    return numCols;
}

std::vector<int> PicrossLevel::getRowRule(int rowNum) {
    return rowRules[rowNum];
}

std::vector<int> PicrossLevel::getColRule(int colNum) {
    return colRules[colNum];
}

std::vector<std::vector<int>> PicrossLevel::getRowRules() {
    return rowRules;
}

std::vector<std::vector<int>> PicrossLevel::getColRules() {
    return colRules;
}

std::vector<std::vector<char>> PicrossLevel::getBoard() {
    return board;
}

std::vector<char> PicrossLevel::getRow(int rowNum) {
    return board[rowNum];
} 

std::vector<char> PicrossLevel::getCol(int colNum) {
    std::vector<char> col;
    for (auto &it : board) {
        col.push_back(it[colNum]);
    }
    return col;
} 

bool PicrossLevel::validateRow(int rowNum) {
    solvedRows[rowNum] = isVectorSolved(board[rowNum], rowRules[rowNum]);
    return solvedRows[rowNum];
}

bool PicrossLevel::validateCol(int colNum) {
    std::vector<char> col = getCol(colNum);
    solvedCols[colNum] = isVectorSolved(col, colRules[colNum]);
    return solvedCols[colNum];
}

bool PicrossLevel::isSolved() {
    for (auto it : solvedRows) {
        if (!it) {
            return false;
        }
    }
    for (auto it : solvedCols) {
        if (!it) {
            return false;
        }
    }
    return true;
}

void PicrossLevel::markCell(int row, int col, char symbol) {
    board[row][col] = symbol;
    validateCol(col);
    validateRow(row);
}

void PicrossLevel::reset() {
    std::fill(solvedRows.begin(), solvedRows.end(), false);
    std::fill(solvedCols.begin(), solvedCols.end(), false);
    for (auto &it : board) {
        std::fill(it.begin(), it.end(), ' ');
    }
}
