#include <vector>
#include <string>

class PicrossLevel {
    public:
        int getNumRows();
        int getNumCols();
        std::vector<int> getRowRule(int rowNum);
        std::vector<int> getColRule(int colNum);
        std::vector<std::vector<int>> getRowRules();
        std::vector<std::vector<int>> getColRules();
        std::vector<std::vector<char>> getBoard();
        std::vector<char> getRow(int rowNum);
        std::vector<char> getCol(int colNum);
        bool validateRow(int rowNum);
        bool validateCol(int colNum);
        bool isSolved();
        void markCell(int row, int col, char symbol);
        void reset();
        PicrossLevel(const std::string &name, const std::vector<std::vector<int>> &rowRules, const std::vector<std::vector<int>> &colRules);
        PicrossLevel(const std::string &filename);
    private:
        std::string name;
        int numRows;
        int numCols;
        std::vector<std::vector<int>> rowRules;
        std::vector<std::vector<int>> colRules;
        std::vector<std::vector<char>> board;
        std::vector<bool> solvedRows;
        std::vector<bool> solvedCols;

        void initLevel(const std::string &name, const std::vector<std::vector<int>> &rowRules, const std::vector<std::vector<int>> &colRules);
};
