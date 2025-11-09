#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <iomanip>
#include <cstring>

using namespace std;

class SudokuSolver {
private:
    vector<vector<int>> board;
    int N = 9;
    
    // Helper function to check if a number can be placed at given position
    bool isValid(int row, int col, int num) {
        // Check row
        for (int x = 0; x < N; x++) {
            if (board[row][x] == num) return false;
        }
        
        // Check column
        for (int x = 0; x < N; x++) {
            if (board[x][col] == num) return false;
        }
        
        // Check 3x3 box
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i + startRow][j + startCol] == num) return false;
            }
        }
        
        return true;
    }
    
    // Find empty cell (returns false if no empty cell found)
    bool findEmptyCell(int& row, int& col) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (board[row][col] == 0) return true;
            }
        }
        return false;
    }
    
    // Constraint Propagation: Remove candidates based on filled cells
    void propagateConstraints(vector<vector<set<int>>>& candidates) {
        bool changed = true;
        while (changed) {
            changed = false;
            
            // For each cell
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (board[i][j] != 0) continue;
                    
                    // Remove candidates based on row constraints
                    for (int k = 0; k < N; k++) {
                        if (board[i][k] != 0 && candidates[i][j].find(board[i][k]) != candidates[i][j].end()) {
                            candidates[i][j].erase(board[i][k]);
                            changed = true;
                        }
                    }
                    
                    // Remove candidates based on column constraints
                    for (int k = 0; k < N; k++) {
                        if (board[k][j] != 0 && candidates[i][j].find(board[k][j]) != candidates[i][j].end()) {
                            candidates[i][j].erase(board[k][j]);
                            changed = true;
                        }
                    }
                    
                    // Remove candidates based on 3x3 box constraints
                    int startRow = i - i % 3;
                    int startCol = j - j % 3;
                    for (int r = startRow; r < startRow + 3; r++) {
                        for (int c = startCol; c < startCol + 3; c++) {
                            if (board[r][c] != 0 && candidates[i][j].find(board[r][c]) != candidates[i][j].end()) {
                                candidates[i][j].erase(board[r][c]);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }

public:
    SudokuSolver() {
        board = vector<vector<int>>(N, vector<int>(N, 0));
    }
    
    // Set the board
    void setBoard(const vector<vector<int>>& newBoard) {
        board = newBoard;
    }
    
    // Get the board
    vector<vector<int>> getBoard() const {
        return board;
    }
    
    // Print the board
    void printBoard() const {
        cout << "\n┌─────────┬─────────┬─────────┐\n";
        for (int i = 0; i < N; i++) {
            if (i > 0 && i % 3 == 0) {
                cout << "├─────────┼─────────┼─────────┤\n";
            }
            cout << "│ ";
            for (int j = 0; j < N; j++) {
                if (j > 0 && j % 3 == 0) cout << "│ ";
                if (board[i][j] == 0) {
                    cout << "· ";
                } else {
                    cout << board[i][j] << " ";
                }
            }
            cout << "│\n";
        }
        cout << "└─────────┴─────────┴─────────┘\n";
    }
    
    // Backtracking Algorithm (Classic DSA approach)
    bool solveBacktracking() {
        int row, col;
        
        // If no empty cell, puzzle is solved
        if (!findEmptyCell(row, col)) {
            return true;
        }
        
        // Try digits 1 to 9
        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                board[row][col] = num;
                
                if (solveBacktracking()) {
                    return true;
                }
                
                // If placing num doesn't lead to solution, backtrack
                board[row][col] = 0;
            }
        }
        
        return false;
    }
    
    // Constraint Propagation with Backtracking (Advanced DSA approach)
    bool solveConstraintPropagation() {
        // Initialize candidates for all empty cells
        vector<vector<set<int>>> candidates(N, vector<set<int>>(N));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == 0) {
                    for (int num = 1; num <= 9; num++) {
                        candidates[i][j].insert(num);
                    }
                }
            }
        }
        
        return solveConstraintPropagationHelper(candidates);
    }
    
private:
    bool solveConstraintPropagationHelper(vector<vector<set<int>>>& candidates) {
        // Propagate constraints
        propagateConstraints(candidates);
        
        // Find cell with minimum candidates
        int minCandidates = 10;
        int minRow = -1, minCol = -1;
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == 0 && candidates[i][j].size() < minCandidates) {
                    minCandidates = candidates[i][j].size();
                    minRow = i;
                    minCol = j;
                }
            }
        }
        
        // If no empty cell, puzzle is solved
        if (minRow == -1) {
            return true;
        }
        
        // If a cell has no candidates, this branch is invalid
        if (candidates[minRow][minCol].empty()) {
            return false;
        }
        
        // Try each candidate for the cell with minimum candidates
        set<int> currentCandidates = candidates[minRow][minCol];
        for (int num : currentCandidates) {
            if (isValid(minRow, minCol, num)) {
                board[minRow][minCol] = num;
                
                // Create new candidates state
                vector<vector<set<int>>> newCandidates = candidates;
                newCandidates[minRow][minCol].clear();
                
                if (solveConstraintPropagationHelper(newCandidates)) {
                    return true;
                }
                
                // Backtrack
                board[minRow][minCol] = 0;
            }
        }
        
        return false;
    }
    
public:
    // Check if the board is valid
    bool isValidBoard() const {
        // Check rows
        for (int i = 0; i < N; i++) {
            set<int> row;
            for (int j = 0; j < N; j++) {
                if (board[i][j] != 0) {
                    if (row.find(board[i][j]) != row.end()) return false;
                    row.insert(board[i][j]);
                }
            }
        }
        
        // Check columns
        for (int j = 0; j < N; j++) {
            set<int> col;
            for (int i = 0; i < N; i++) {
                if (board[i][j] != 0) {
                    if (col.find(board[i][j]) != col.end()) return false;
                    col.insert(board[i][j]);
                }
            }
        }
        
        // Check 3x3 boxes
        for (int box = 0; box < N; box++) {
            set<int> boxSet;
            int startRow = (box / 3) * 3;
            int startCol = (box % 3) * 3;
            for (int i = startRow; i < startRow + 3; i++) {
                for (int j = startCol; j < startCol + 3; j++) {
                    if (board[i][j] != 0) {
                        if (boxSet.find(board[i][j]) != boxSet.end()) return false;
                        boxSet.insert(board[i][j]);
                    }
                }
            }
        }
        
        return true;
    }
    
    // Check if the board is complete
    bool isComplete() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == 0) return false;
            }
        }
        return true;
    }
}; 