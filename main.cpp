#include "SudokuSolver.cpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// Predefined Sudoku puzzles for demonstration
vector<vector<int>> getEasyPuzzle() {
    return {
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
}

vector<vector<int>> getHardPuzzle() {
    return {
        {0,0,0,0,0,0,0,1,2},
        {0,0,0,0,3,5,0,0,0},
        {0,0,0,6,0,0,0,7,0},
        {7,0,0,0,0,0,3,0,0},
        {0,0,0,4,0,0,8,0,0},
        {1,0,0,0,0,0,0,0,0},
        {0,0,0,1,2,0,0,0,0},
        {0,8,0,0,0,0,0,4,0},
        {0,5,0,0,0,0,6,0,0}
    };
}

vector<vector<int>> getExpertPuzzle() {
    return {
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,3,0,8,5},
        {0,0,1,0,2,0,0,0,0},
        {0,0,0,5,0,7,0,0,0},
        {0,0,4,0,0,0,1,0,0},
        {0,9,0,0,0,0,0,0,0},
        {5,0,0,0,0,0,0,7,3},
        {0,0,2,0,1,0,0,0,0},
        {0,0,0,0,4,0,0,0,9}
    };
}

// Function to measure execution time
template<typename Func>
double measureTime(Func func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    return duration.count();
}

// Function to input Sudoku board manually
vector<vector<int>> inputBoard() {
    vector<vector<int>> board(9, vector<int>(9, 0));
    cout << "\n=== Manual Input Mode ===\n";
    cout << "Enter the Sudoku board row by row.\n";
    cout << "Use 0 for empty cells, 1-9 for filled cells.\n";
    cout << "Enter each row as 9 space-separated numbers:\n\n";
    
    for (int i = 0; i < 9; i++) {
        cout << "Row " << (i + 1) << ": ";
        for (int j = 0; j < 9; j++) {
            cin >> board[i][j];
            if (board[i][j] < 0 || board[i][j] > 9) {
                cout << "Invalid input! Please enter numbers 0-9 only.\n";
                j--; // Retry this position
            }
        }
    }
    return board;
}

// Function to compare algorithms
void compareAlgorithms(const vector<vector<int>>& puzzle, const string& puzzleName) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "ALGORITHM COMPARISON: " << puzzleName << "\n";
    cout << string(60, '=') << "\n";
    
    // Test Backtracking
    SudokuSolver solver1;
    solver1.setBoard(puzzle);
    
    cout << "\nOriginal Puzzle:";
    solver1.printBoard();
    
    double backtrackingTime = measureTime([&]() {
        solver1.solveBacktracking();
    });
    
    cout << "\nBacktracking Solution:";
    solver1.printBoard();
    cout << "Backtracking Time: " << fixed << setprecision(3) << backtrackingTime << " ms\n";
    
    // Test Constraint Propagation
    SudokuSolver solver2;
    solver2.setBoard(puzzle);
    
    double constraintTime = measureTime([&]() {
        solver2.solveConstraintPropagation();
    });
    
    cout << "\nConstraint Propagation Solution:";
    solver2.printBoard();
    cout << "Constraint Propagation Time: " << fixed << setprecision(3) << constraintTime << " ms\n";
    
    // Performance comparison
    cout << "\n" << string(40, '-') << "\n";
    cout << "PERFORMANCE COMPARISON:\n";
    cout << string(40, '-') << "\n";
    cout << "Backtracking:           " << setw(10) << backtrackingTime << " ms\n";
    cout << "Constraint Propagation: " << setw(10) << constraintTime << " ms\n";
    
    if (constraintTime < backtrackingTime) {
        double speedup = backtrackingTime / constraintTime;
        cout << "Constraint Propagation is " << fixed << setprecision(2) << speedup << "x faster!\n";
    } else if (backtrackingTime < constraintTime) {
        double speedup = constraintTime / backtrackingTime;
        cout << "Backtracking is " << fixed << setprecision(2) << speedup << "x faster!\n";
    } else {
        cout << "Both algorithms performed similarly.\n";
    }
    
    // Verify solutions are identical
    bool solutionsMatch = (solver1.getBoard() == solver2.getBoard());
    cout << "Solutions match: " << (solutionsMatch ? "YES" : "NO") << "\n";
}

// Function to demonstrate DSA concepts
void demonstrateDSAConcepts() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "DSA CONCEPTS DEMONSTRATION\n";
    cout << string(60, '=') << "\n";
    
    cout << "\n1. BACKTRACKING ALGORITHM:\n";
    cout << "   - Time Complexity: O(9^(n*n)) in worst case\n";
    cout << "   - Space Complexity: O(n*n) for recursion stack\n";
    cout << "   - Strategy: Try each digit (1-9) in empty cells\n";
    cout << "   - If current placement doesn't lead to solution, backtrack\n";
    cout << "   - Classic example of recursive problem solving\n";
    
    cout << "\n2. CONSTRAINT PROPAGATION:\n";
    cout << "   - Time Complexity: O(n^4) for constraint checking\n";
    cout << "   - Space Complexity: O(n*n) for candidate sets\n";
    cout << "   - Strategy: Maintain set of possible values for each cell\n";
    cout << "   - Remove impossible candidates based on filled cells\n";
    cout << "   - Choose cell with minimum candidates for next move\n";
    
    cout << "\n3. DATA STRUCTURES USED:\n";
    cout << "   - 2D Vector: Represent Sudoku board\n";
    cout << "   - Set: Store candidate values for each cell\n";
    cout << "   - Recursion Stack: Implement backtracking\n";
    cout << "   - Hash Set: Check for duplicates in validation\n";
    
    cout << "\n4. ALGORITHM OPTIMIZATION:\n";
    cout << "   - Constraint propagation reduces search space\n";
    cout << "   - Minimum candidate selection reduces branching\n";
    cout << "   - Early termination on invalid states\n";
    cout << "   - Efficient validation using sets\n";
}

int main() {
  
    while (true) {
        cout << "\n" << string(50, '=') << "\n";
        cout << "MAIN MENU\n";
        cout << string(50, '=') << "\n";
        cout << "1. Solve Easy Puzzle (Demo)\n";
        cout << "2. Solve Hard Puzzle (Demo)\n";
        cout << "3. Solve Expert Puzzle (Demo)\n";
        cout << "4. Manual Input Mode\n";
        cout << "5. Algorithm Comparison\n";
        cout << "6. DSA Concepts Explanation\n";
        cout << "7. Exit\n";
        cout << "\nEnter your choice (1-7): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\n=== EASY PUZZLE DEMO ===\n";
                compareAlgorithms(getEasyPuzzle(), "Easy Puzzle");
                break;
            }
            case 2: {
                cout << "\n=== HARD PUZZLE DEMO ===\n";
                compareAlgorithms(getHardPuzzle(), "Hard Puzzle");
                break;
            }
            case 3: {
                cout << "\n=== EXPERT PUZZLE DEMO ===\n";
                compareAlgorithms(getExpertPuzzle(), "Expert Puzzle");
                break;
            }
            case 4: {
                vector<vector<int>> board = inputBoard();
                SudokuSolver solver;
                solver.setBoard(board);
                
                if (!solver.isValidBoard()) {
                    cout << "\n❌ Invalid Sudoku board! Please check your input.\n";
                    break;
                }
                
                cout << "\nYour puzzle:";
                solver.printBoard();
                
                cout << "\nChoose solving method:\n";
                cout << "1. Backtracking\n";
                cout << "2. Constraint Propagation\n";
                cout << "3. Both (with comparison)\n";
                cout << "Enter choice (1-3): ";
                
                int method;
                cin >> method;
                
                if (method == 1) {
                    double time = measureTime([&]() { solver.solveBacktracking(); });
                    cout << "\nSolution:";
                    solver.printBoard();
                    cout << "Time: " << fixed << setprecision(3) << time << " ms\n";
                } else if (method == 2) {
                    double time = measureTime([&]() { solver.solveConstraintPropagation(); });
                    cout << "\nSolution:";
                    solver.printBoard();
                    cout << "Time: " << fixed << setprecision(3) << time << " ms\n";
                } else if (method == 3) {
                    compareAlgorithms(board, "Manual Input");
                }
                break;
            }
            case 5: {
                cout << "\n=== ALGORITHM COMPARISON MODE ===\n";
                cout << "Choose puzzle difficulty:\n";
                cout << "1. Easy\n";
                cout << "2. Hard\n";
                cout << "3. Expert\n";
                cout << "Enter choice (1-3): ";
                
                int difficulty;
                cin >> difficulty;
                
                vector<vector<int>> puzzle;
                string name;
                switch (difficulty) {
                    case 1: puzzle = getEasyPuzzle(); name = "Easy"; break;
                    case 2: puzzle = getHardPuzzle(); name = "Hard"; break;
                    case 3: puzzle = getExpertPuzzle(); name = "Expert"; break;
                    default: cout << "Invalid choice!\n"; continue;
                }
                
                compareAlgorithms(puzzle, name + " Puzzle");
                break;
            }
            case 6: {
                demonstrateDSAConcepts();
                break;
            }
            case 7: {
                cout << "\n🎉 Thank you for using Sudoku Solver Master! 🎉\n";
                cout << "Good luck with your DSA presentation!\n";
                return 0;
            }
            default: {
                cout << "Invalid choice! Please enter 1-7.\n";
                break;
            }
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
} 