#include "SudokuSolver.cpp"
#include <iostream>
#include <chrono>

using namespace std;

int main() {
    cout << "🧪 Quick Test Demo - Sudoku Solver Master\n";
    cout << "==========================================\n\n";
    
    // Test puzzle
    vector<vector<int>> testPuzzle = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };
    
    SudokuSolver solver;
    solver.setBoard(testPuzzle);
    
    cout << "Original Puzzle:";
    solver.printBoard();
    
    // Test backtracking
    auto start = chrono::high_resolution_clock::now();
    bool solved = solver.solveBacktracking();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    
    if (solved) {
        cout << "✅ Backtracking Solution:";
        solver.printBoard();
        cout << "Time: " << duration.count() << " ms\n";
    } else {
        cout << "❌ Backtracking failed to solve\n";
    }
    
    cout << "\n🎉 Test completed successfully!\n";
    cout << "You can now run the full program with: ./sudoku_solver\n";
    
    return 0;
} 