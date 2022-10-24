#include "picrosslevel.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void PicrossLevel::initLevel(const std::string &name, const std::vector<std::vector<int>> &rowRules, const std::vector<std::vector<int>> &colRules) {
    this->name = name;
    this->rowRules = rowRules;
    this->colRules = colRules;
    numRows = rowRules.size();
    numCols = colRules.size();
    board = std::vector<std::vector<char>>(numRows, std::vector<char>(numCols, ' '));
    solvedRows = std::vector<bool>(numRows, false);
    solvedCols = std::vector<bool>(numCols, false);
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

std::vector<std::vector<int>> PicrossLevel::getRowRules() {
    return rowRules;
}

std::vector<std::vector<int>> PicrossLevel::getColRules() {
    return colRules;
}
