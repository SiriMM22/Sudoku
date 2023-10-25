#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sstream>



using namespace std::chrono;
using namespace std;

const int N = 9;

class Sudoku {
public:
	int mat[N][N];
	int SRN;
	int K;
	
	Sudoku(int K)
	{
		this->K = K;
		double SRNd = sqrt(N);
		SRN = (int)SRNd;
	}

	void fillValues()
	{
		fillDiagonal();
		fillRemaining(0, SRN);
		removeKDigits();
	}

	void fillDiagonal()
	{
		for (int i = 0; i < N; i = i + SRN)
		{
			fillBox(i, i);
		}
	}
	// Returns false if given 3 x 3 block contains num.
	bool unUsedInBox(int rowStart, int colStart, int num)
	{
		for (int i = 0; i < SRN; i++) {
			for (int j = 0; j < SRN; j++) {
				if (mat[rowStart + i][colStart + j]
					== num) {
					return false;
				}
			}
		}
		return true;
	}
	// Fill a 3 x 3 matrix.
	void fillBox(int row, int col)
	{
		int num;
		for (int i = 0; i < SRN; i++) {
			for (int j = 0; j < SRN; j++) {
				do {
					num = randomGenerator(N);
				} while (!unUsedInBox(row, col, num));
				mat[row + i][col + j] = num;
			}
		}
	}
	// Random generator
	int randomGenerator(int num)
	{
		return (int)floor(
			(float)(rand() / double(RAND_MAX) * num + 1));
	}
	// Check if safe to put in cell
	bool CheckIfSafe(int i, int j, int num)
	{
		return (
			unUsedInRow(i, num) && unUsedInCol(j, num)
			&& unUsedInBox(i - i % SRN, j - j % SRN, num));
	}
	// check in the row for existence
	bool unUsedInRow(int i, int num)
	{
		for (int j = 0; j < N; j++) {
			if (mat[i][j] == num) {
				return false;
			}
		}
		return true;
	}
	// check in the row for existence
	bool unUsedInCol(int j, int num)
	{
		for (int i = 0; i < N; i++) {
			if (mat[i][j] == num) {
				return false;
			}
		}
		return true;
	}
	// A recursive function to fill remaining matrix
	bool fillRemaining(int i, int j)
	{
		// System.out.println(i+" "+j);
		if (j >= N && i < N - 1) {
			i = i + 1;
			j = 0;
		}
		if (i >= N && j >= N) {
			return true;
		}
		if (i < SRN) {
			if (j < SRN) {
				j = SRN;
			}
		}
		else if (i < N - SRN) {
			if (j == (int)(i / SRN) * SRN) {
				j = j + SRN;
			}
		}
		else {
			if (j == N - SRN) {
				i = i + 1;
				j = 0;
				if (i >= N) {
					return true;
				}
			}
		}
		for (int num = 1; num <= N; num++) {
			if (CheckIfSafe(i, j, num)) {
				mat[i][j] = num;
				if (fillRemaining(i, j + 1)) {
					return true;
				}
				mat[i][j] = 0;
			}
		}
		return false;
	}
	// Remove the K no. of digits to
	// complete game
	void removeKDigits()
	{
		int count = K;
		while (count != 0) {
			int cellId = randomGenerator(N * N) - 1;
			// System.out.println(cellId);
			// extract coordinates i and j
			int i = (cellId / N);
			int j = cellId % 9;
			if (j != 0) {
				j = j - 1;
			}
			// System.out.println(i+" "+j);
			if (mat[i][j] != 0) {
				count--;
				mat[i][j] = 0;
			}
		}
	}
};

bool isSafe(int board[N][N], int row, int col, int num) {
    // Check if 'num' is already in the same row
    for (int i = 0; i < N; i++)
        if (board[row][i] == num)
            return false;

    // Check if 'num' is already in the same column
    for (int i = 0; i < N; i++)
        if (board[i][col] == num)
            return false;

    // Check if 'num' is already in the same 3x3 box
    int boxRowStart = row - row % 3;
    int boxColStart = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + boxRowStart][j + boxColStart] == num)
                return false;

    return true;
}

void printBoard(int grid[N][N]) {
    system("cls");
    cout << "\t<================================================================================>" << endl;
    cout << "\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
    cout << "\t|               Fill in the missing numbers to solve the puzzle.                 |" << endl;
    cout << "\t<================================================================================>" << endl;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (col == 3 || col == 6)
                cout << " | ";
            if(grid[row][col]==0)
                cout<<"_"<<" ";
            else
                cout << grid[row][col] << " ";
        }
        if (row == 2 || row == 5) {
            cout << endl;
            for (int i = 0; i < N; i++)
                cout << "---";
        }
        cout << endl;
    }
}

bool solveSudoku(int board[N][N], int row, int col) {
    // If all cells are filled, the puzzle is solved
    if (row == N - 1 && col == N)
        return true;

    // Move to the next row if the current column is N
    if (col == N) {
        row++;
        col = 0;
    }

    // Skip the cells that already have a value
    if (board[row][col] != 0)
        return solveSudoku(board, row, col + 1);

    // Try filling the current cell with a number from 1 to 9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            if (solveSudoku(board, row, col + 1))
                return true;

            board[row][col] = 0;
        }
    }
    return false;
}

bool isSolvedCompletely(int grid[N][N]) {
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return false;

    return true;
}

void saveHighScore(const string& username, int score) {
    //cout<<score;
    std::ofstream file("highscore.txt", std::ios::app);
    if (file.is_open()) {
        file << username << " \t  " << score << "\n";
        file.close();
    } else {
        std::cout << "Error opening file for writing!" << std::endl;
    }
}

void loadHighScore() {
    string text;
    std::ifstream file("highscore.txt");
    cout<<"NAME\tTIME"<<endl;
    while(getline(file,text)){
        cout<<text<<endl;
    }
file.close();
}

void displayHighScores() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        vector<int> scores;
        int score;
        while (file >> score) {
            scores.push_back(score);
        }
        file.close();

        sort(scores.rbegin(), scores.rend());

        cout << "High Scores: " << endl;
        for (int i = 0; i < scores.size(); i++) {
            cout << i + 1 << ". " << scores[i] << endl;
        }
    }
    else {
        cout << "No high scores found." << endl;
    }
}



int playGame(int board[N][N]) {
    int ch;
    int row, col, num;
    while (true) {
        printBoard(board);
        cout << endl << endl;
        cout << "Unable to solve? Enter -1 as row, col, and num to view the solved Sudoku." << endl;
        cout << "Enter row: ";
        cin >> row;
        cout << "Enter column: ";
        cin >> col;
        cout << "Enter number: ";
        cin >> num;

        if (row == -1 || col == -1 || num == -1) {
            solveSudoku(board, 0, 0);
            printBoard(board);
            cout << endl;
            cout << "Better luck next time!!!" << endl;
            return 0;
        }
        
        row--;
        col--;
        if (!isSafe(board, row, col, num)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }
        board[row][col] = num;
        if (isSolvedCompletely(board))
            break;
    }

    bool solved = true;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                solved = false;
                break;
            }
        }
    }

    if (solved) {
        printBoard(board);
        cout << "Congratulations! You have solved the puzzle." << endl;
        return 1;
    }
    else {
        cout << "Puzzle not solved. Better luck next time." << endl;
        return 0;
    }
}

int main() {
    system("title Sudoku Game @copyassignment");
    system("color B0");

    cout << "\t<================================================================================>" << endl;
    cout << "\t|                        WELCOME TO SUDOKU Game!                                 |" << endl;
    cout << "\t|       Fill in the missing numbers (represented by _) to solve the puzzle.      |" << endl;
    cout << "\t<================================================================================>" << endl;

    while (true) {

        int board[N][N];
        int choice;
        cout << endl << endl;
        cout << "\t\t[1] Solve the Sudoku" << endl;
        //cout << "\t\t[2] Unable to solve? View the solved Sudoku" << endl;
        cout << "\t\t[2] Show High Score" << endl;
        cout << "\t\t[3] Exit" << endl;
        cout << "\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string username;
            cout<<"enter your name:"<<endl;
            cin>>username;
            int difficulty,k;
            cout << endl << endl;
            cout << "\t\t\t Select difficulty" << endl<<endl<<endl;
            cout << "\t\t[1] Easy" << endl;
            cout << "\t\t[2] Show Medium" << endl;
            cout << "\t\t[3] Hard" << endl;
            cout << "\t\tEnter your choice: ";
            cin >> difficulty;
            switch(difficulty){
                case 1:k=1;
                    break;
                case 2:k=20;
                    break;
                case 3:k=30;
                    break;
                default:k=10;
            }
            // Start the timer
            auto start = high_resolution_clock::now();
            srand(time(0));
            Sudoku* sudoku = new Sudoku(k);
	        sudoku->fillValues();
            for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = sudoku->mat[i][j];
            }
        }
        
           if( playGame(board))
         { 
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        saveHighScore(username,duration.count());
        // Display the solved grid and elapsed time
        cout << "Solved in " << duration.count() << " seconds" << endl;
         }
            break;

        }
        case 2:
            loadHighScore();
            break;
        case 3:
            exit(0);
        default:
            cout << "Invalid choice" << endl;
        }
    }
    return 0;
}