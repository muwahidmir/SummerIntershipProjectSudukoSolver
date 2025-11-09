// Sudoku Solver Master - Frontend JavaScript
class SudokuSolver {
    constructor() {
        this.board = Array(9).fill().map(() => Array(9).fill(0));
        this.originalBoard = Array(9).fill().map(() => Array(9).fill(0));
        this.selectedAlgorithm = 'backtracking';
        this.isSolving = false;
        this.stepCount = 0;
        
        this.initializeBoard();
        this.setupEventListeners();
        this.loadPuzzle('easy');
    }

    // Initialize the Sudoku board
    initializeBoard() {
        const boardElement = document.getElementById('sudokuBoard');
        boardElement.innerHTML = '';

        for (let i = 0; i < 9; i++) {
            for (let j = 0; j < 9; j++) {
                const cell = document.createElement('input');
                cell.type = 'text';
                cell.className = 'cell';
                cell.dataset.row = i;
                cell.dataset.col = j;
                cell.maxLength = 1;
                
                // Add thicker borders for 3x3 boxes
                if (i % 3 === 0) cell.style.borderTop = '2px solid #2d3748';
                if (i === 8) cell.style.borderBottom = '2px solid #2d3748';
                if (j % 3 === 0) cell.style.borderLeft = '2px solid #2d3748';
                if (j === 8) cell.style.borderRight = '2px solid #2d3748';
                
                cell.addEventListener('input', (e) => this.handleCellInput(e));
                cell.addEventListener('keydown', (e) => this.handleKeyDown(e));
                cell.addEventListener('click', (e) => this.handleCellClick(e));
                
                boardElement.appendChild(cell);
            }
        }
    }

    // Handle cell input
    handleCellInput(event) {
        const cell = event.target;
        const value = cell.value;
        const row = parseInt(cell.dataset.row);
        const col = parseInt(cell.dataset.col);

        // Only allow numbers 1-9 or empty
        if (value === '' || (value >= '1' && value <= '9')) {
            this.board[row][col] = value === '' ? 0 : parseInt(value);
            cell.classList.remove('error');
        } else {
            cell.value = '';
            this.board[row][col] = 0;
        }

        this.updateStatus('Board updated');
    }

    // Handle key navigation
    handleKeyDown(event) {
        const cell = event.target;
        const row = parseInt(cell.dataset.row);
        const col = parseInt(cell.dataset.col);

        let nextCell = null;

        switch (event.key) {
            case 'ArrowUp':
                if (row > 0) nextCell = document.querySelector(`[data-row="${row-1}"][data-col="${col}"]`);
                break;
            case 'ArrowDown':
                if (row < 8) nextCell = document.querySelector(`[data-row="${row+1}"][data-col="${col}"]`);
                break;
            case 'ArrowLeft':
                if (col > 0) nextCell = document.querySelector(`[data-row="${row}"][data-col="${col-1}"]`);
                break;
            case 'ArrowRight':
                if (col < 8) nextCell = document.querySelector(`[data-row="${row}"][data-col="${col+1}"]`);
                break;
        }

        if (nextCell) {
            event.preventDefault();
            nextCell.focus();
        }
    }

    // Handle cell click
    handleCellClick(event) {
        const cell = event.target;
        if (cell.classList.contains('fixed')) {
            return; // Don't allow editing fixed cells
        }
        cell.select();
    }

    // Setup event listeners
    setupEventListeners() {
        // Algorithm selection
        document.getElementById('backtrackingBtn').addEventListener('click', () => {
            this.selectedAlgorithm = 'backtracking';
            this.updateAlgorithmButtons();
        });

        document.getElementById('constraintBtn').addEventListener('click', () => {
            this.selectedAlgorithm = 'constraint';
            this.updateAlgorithmButtons();
        });

        document.getElementById('compareBtn').addEventListener('click', () => {
            this.compareAlgorithms();
        });

        // Puzzle selection
        document.getElementById('easyBtn').addEventListener('click', () => this.loadPuzzle('easy'));
        document.getElementById('hardBtn').addEventListener('click', () => this.loadPuzzle('hard'));
        document.getElementById('expertBtn').addEventListener('click', () => this.loadPuzzle('expert'));
        document.getElementById('clearBtn').addEventListener('click', () => this.clearBoard());

        // Board controls
        document.getElementById('solveBtn').addEventListener('click', () => this.solve());
        document.getElementById('resetBtn').addEventListener('click', () => this.resetBoard());

        // Modal
        document.getElementById('closeModal').addEventListener('click', () => this.hideModal());
    }

    // Update algorithm button states
    updateAlgorithmButtons() {
        document.getElementById('backtrackingBtn').classList.remove('btn-primary');
        document.getElementById('backtrackingBtn').classList.add('btn-outline');
        document.getElementById('constraintBtn').classList.remove('btn-secondary');
        document.getElementById('constraintBtn').classList.add('btn-outline');

        if (this.selectedAlgorithm === 'backtracking') {
            document.getElementById('backtrackingBtn').classList.remove('btn-outline');
            document.getElementById('backtrackingBtn').classList.add('btn-primary');
        } else {
            document.getElementById('constraintBtn').classList.remove('btn-outline');
            document.getElementById('constraintBtn').classList.add('btn-secondary');
        }
    }

    // Load predefined puzzles
    loadPuzzle(difficulty) {
        const puzzles = {
            easy: [
                [5,3,0,0,7,0,0,0,0],
                [6,0,0,1,9,5,0,0,0],
                [0,9,8,0,0,0,0,6,0],
                [8,0,0,0,6,0,0,0,3],
                [4,0,0,8,0,3,0,0,1],
                [7,0,0,0,2,0,0,0,6],
                [0,6,0,0,0,0,2,8,0],
                [0,0,0,4,1,9,0,0,5],
                [0,0,0,0,8,0,0,7,9]
            ],
            hard: [
                [0,0,0,0,0,0,0,1,2],
                [0,0,0,0,3,5,0,0,0],
                [0,0,0,6,0,0,0,7,0],
                [7,0,0,0,0,0,3,0,0],
                [0,0,0,4,0,0,8,0,0],
                [1,0,0,0,0,0,0,0,0],
                [0,0,0,1,2,0,0,0,0],
                [0,8,0,0,0,0,0,4,0],
                [0,5,0,0,0,0,6,0,0]
            ],
            expert: [
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,3,0,8,5],
                [0,0,1,0,2,0,0,0,0],
                [0,0,0,5,0,7,0,0,0],
                [0,0,4,0,0,0,1,0,0],
                [0,9,0,0,0,0,0,0,0],
                [5,0,0,0,0,0,0,7,3],
                [0,0,2,0,1,0,0,0,0],
                [0,0,0,0,4,0,0,0,9]
            ]
        };

        this.board = puzzles[difficulty].map(row => [...row]);
        this.originalBoard = puzzles[difficulty].map(row => [...row]);
        this.updateBoardDisplay();
        this.updateStatus(`${difficulty.charAt(0).toUpperCase() + difficulty.slice(1)} puzzle loaded`);
    }

    // Clear the board
    clearBoard() {
        this.board = Array(9).fill().map(() => Array(9).fill(0));
        this.originalBoard = Array(9).fill().map(() => Array(9).fill(0));
        this.updateBoardDisplay();
        this.updateStatus('Board cleared');
    }

    // Update board display
    updateBoardDisplay() {
        for (let i = 0; i < 9; i++) {
            for (let j = 0; j < 9; j++) {
                const cell = document.querySelector(`[data-row="${i}"][data-col="${j}"]`);
                const value = this.board[i][j];
                const originalValue = this.originalBoard[i][j];

                cell.value = value === 0 ? '' : value.toString();
                cell.classList.remove('fixed', 'solved', 'error');

                if (originalValue !== 0) {
                    cell.classList.add('fixed');
                } else if (value !== 0) {
                    cell.classList.add('solved');
                }
            }
        }
    }

    // Solve the puzzle
    async solve() {
        if (this.isSolving) return;

        this.isSolving = true;
        this.stepCount = 0;
        this.showLoading();

        try {
            const startTime = performance.now();
            let solved = false;

            if (this.selectedAlgorithm === 'backtracking') {
                solved = this.solveBacktracking();
            } else {
                solved = this.solveConstraintPropagation();
            }

            const endTime = performance.now();
            const solveTime = endTime - startTime;

            if (solved) {
                this.updateBoardDisplay();
                this.updatePerformance(this.selectedAlgorithm, solveTime);
                this.showSuccessModal(this.selectedAlgorithm, solveTime);
                this.updateStatus('Puzzle solved successfully!');
            } else {
                this.updateStatus('No solution found');
            }
        } catch (error) {
            console.error('Error solving puzzle:', error);
            this.updateStatus('Error solving puzzle');
        } finally {
            this.hideLoading();
            this.isSolving = false;
        }
    }

    // Backtracking algorithm
    solveBacktracking() {
        const board = this.board.map(row => [...row]);
        
        const isValid = (row, col, num) => {
            // Check row
            for (let x = 0; x < 9; x++) {
                if (board[row][x] === num) return false;
            }
            
            // Check column
            for (let x = 0; x < 9; x++) {
                if (board[x][col] === num) return false;
            }
            
            // Check 3x3 box
            const startRow = Math.floor(row / 3) * 3;
            const startCol = Math.floor(col / 3) * 3;
            for (let i = 0; i < 3; i++) {
                for (let j = 0; j < 3; j++) {
                    if (board[i + startRow][j + startCol] === num) return false;
                }
            }
            
            return true;
        };

        const findEmptyCell = () => {
            for (let row = 0; row < 9; row++) {
                for (let col = 0; col < 9; col++) {
                    if (board[row][col] === 0) {
                        return [row, col];
                    }
                }
            }
            return null;
        };

        const solve = () => {
            this.stepCount++;
            const emptyCell = findEmptyCell();
            
            if (!emptyCell) {
                return true; // Puzzle solved
            }
            
            const [row, col] = emptyCell;
            
            for (let num = 1; num <= 9; num++) {
                if (isValid(row, col, num)) {
                    board[row][col] = num;
                    
                    if (solve()) {
                        return true;
                    }
                    
                    board[row][col] = 0; // Backtrack
                }
            }
            
            return false;
        };

        const result = solve();
        if (result) {
            // Update the main board with the solution
            this.board = board;
        }
        return result;
    }

    // Constraint propagation algorithm
    solveConstraintPropagation() {
        const board = this.board.map(row => [...row]);
        
        // Initialize candidates for all empty cells
        const candidates = Array(9).fill().map(() => 
            Array(9).fill().map(() => new Set([1,2,3,4,5,6,7,8,9]))
        );

        // Remove candidates based on filled cells
        for (let i = 0; i < 9; i++) {
            for (let j = 0; j < 9; j++) {
                if (board[i][j] !== 0) {
                    candidates[i][j].clear();
                    removeCandidates(candidates, i, j, board[i][j]);
                }
            }
        }

        const isValid = (row, col, num) => {
            // Check row
            for (let x = 0; x < 9; x++) {
                if (board[row][x] === num) return false;
            }
            
            // Check column
            for (let x = 0; x < 9; x++) {
                if (board[x][col] === num) return false;
            }
            
            // Check 3x3 box
            const startRow = Math.floor(row / 3) * 3;
            const startCol = Math.floor(col / 3) * 3;
            for (let i = 0; i < 3; i++) {
                for (let j = 0; j < 3; j++) {
                    if (board[i + startRow][j + startCol] === num) return false;
                }
            }
            
            return true;
        };

        const removeCandidates = (candidates, row, col, num) => {
            // Remove from row
            for (let j = 0; j < 9; j++) {
                if (j !== col) {
                    candidates[row][j].delete(num);
                }
            }
            
            // Remove from column
            for (let i = 0; i < 9; i++) {
                if (i !== row) {
                    candidates[i][col].delete(num);
                }
            }
            
            // Remove from 3x3 box
            const startRow = Math.floor(row / 3) * 3;
            const startCol = Math.floor(col / 3) * 3;
            for (let i = startRow; i < startRow + 3; i++) {
                for (let j = startCol; j < startCol + 3; j++) {
                    if (i !== row || j !== col) {
                        candidates[i][j].delete(num);
                    }
                }
            }
        };

        const findMinCandidates = () => {
            let minCandidates = 10;
            let minCell = null;
            
            for (let i = 0; i < 9; i++) {
                for (let j = 0; j < 9; j++) {
                    if (board[i][j] === 0 && candidates[i][j].size < minCandidates) {
                        minCandidates = candidates[i][j].size;
                        minCell = [i, j];
                    }
                }
            }
            
            return minCell;
        };

        const solve = () => {
            this.stepCount++;
            const minCell = findMinCandidates();
            
            if (!minCell) {
                return true; // Puzzle solved
            }
            
            const [row, col] = minCell;
            const currentCandidates = new Set(candidates[row][col]);
            
            for (const num of currentCandidates) {
                if (isValid(row, col, num)) {
                    board[row][col] = num;
                    
                    // Create new candidates state
                    const newCandidates = candidates.map(row => 
                        row.map(cell => new Set(cell))
                    );
                    newCandidates[row][col].clear();
                    removeCandidates(newCandidates, row, col, num);
                    
                    if (solve()) {
                        return true;
                    }
                    
                    board[row][col] = 0; // Backtrack
                }
            }
            
            return false;
        };

        const result = solve();
        if (result) {
            // Update the main board with the solution
            this.board = board;
        }
        return result;
    }

    // Compare both algorithms
    async compareAlgorithms() {
        if (this.isSolving) return;

        this.isSolving = true;
        this.showLoading();

        try {
            const boardCopy = this.board.map(row => [...row]);
            
            // Test backtracking
            this.board = boardCopy.map(row => [...row]);
            const backtrackingStart = performance.now();
            const backtrackingSolved = this.solveBacktracking();
            const backtrackingTime = performance.now() - backtrackingStart;
            
            // Test constraint propagation
            this.board = boardCopy.map(row => [...row]);
            const constraintStart = performance.now();
            const constraintSolved = this.solveConstraintPropagation();
            const constraintTime = performance.now() - constraintStart;
            
            // Update performance display
            this.updatePerformance('backtracking', backtrackingTime);
            this.updatePerformance('constraint', constraintTime);
            
            // Calculate speedup
            if (constraintTime > 0 && backtrackingTime > 0) {
                const speedup = backtrackingTime / constraintTime;
                document.getElementById('speedup').textContent = `${speedup.toFixed(2)}x`;
            }
            
            // Use the faster solution for display
            if (constraintTime < backtrackingTime) {
                this.board = boardCopy.map(row => [...row]);
                this.solveConstraintPropagation();
                this.selectedAlgorithm = 'constraint';
            } else {
                this.board = boardCopy.map(row => [...row]);
                this.solveBacktracking();
                this.selectedAlgorithm = 'backtracking';
            }
            
            this.updateBoardDisplay();
            this.updateAlgorithmButtons();
            this.updateStatus('Algorithm comparison completed');
            
        } catch (error) {
            console.error('Error comparing algorithms:', error);
            this.updateStatus('Error comparing algorithms');
        } finally {
            this.hideLoading();
            this.isSolving = false;
        }
    }

    // Update performance display
    updatePerformance(algorithm, time) {
        if (algorithm === 'backtracking') {
            document.getElementById('backtrackingTime').textContent = `${time.toFixed(2)} ms`;
        } else if (algorithm === 'constraint') {
            document.getElementById('constraintTime').textContent = `${time.toFixed(2)} ms`;
        }
    }

    // Reset board to original state
    resetBoard() {
        this.board = this.originalBoard.map(row => [...row]);
        this.updateBoardDisplay();
        this.updateStatus('Board reset to original');
    }

    // Update status message
    updateStatus(message) {
        document.getElementById('statusText').textContent = message;
    }

    // Show loading overlay
    showLoading() {
        document.getElementById('loadingOverlay').style.display = 'flex';
    }

    // Hide loading overlay
    hideLoading() {
        document.getElementById('loadingOverlay').style.display = 'none';
    }

    // Show success modal
    showSuccessModal(algorithm, time) {
        document.getElementById('modalAlgorithm').textContent = 
            algorithm === 'backtracking' ? 'Backtracking' : 'Constraint Propagation';
        document.getElementById('modalTime').textContent = `${time.toFixed(2)} ms`;
        document.getElementById('modalSteps').textContent = this.stepCount.toLocaleString();
        
        document.getElementById('successModal').style.display = 'flex';
    }

    // Hide modal
    hideModal() {
        document.getElementById('successModal').style.display = 'none';
    }
}

// Initialize the application when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    new SudokuSolver();
}); 