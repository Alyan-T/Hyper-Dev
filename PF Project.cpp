#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include<string>
#include<fstream>
using namespace std;
// Constant
const int max_players = 10;
// Functions used
void updateLeaderboard(string rollNumber, int score, string rollNumbers[], int scores[], int& leaderboardSize);
void displayLeaderboard(string rollNumbers[], int scores[], int leaderboardSize);
bool login(string rollNumbers[], int leaderboardSize);
void registerUser(string rollNumbers[], int scores[], int& leaderboardSize);
void cell_number(int i, int j, int size);
void createmine_positions(int size, int n);
void createtable(int size);
void printtable(char arr[][100], int size);
void opencell(int size);
void placeflag(int size);
void removeflag(int size);
void input_symbol(int size);
void reveal(int i, int j, int size);
bool endgame_wincheck(int size, int totalMines);
void game(int size, int n);
void difficulty();
void loadfrom_file(string file, string rollNumbers[], int scores[], int &leaderboardSize);
void saveto_file(string filename, string rollNumbers[], int scores[], int leaderboardSize);

// Global variables
string rollNumbers[max_players];
int scores[max_players];
int leaderboardSize = 0;
char table[100][100];
char tablemine_position[100][100];
char symbol;
int flag_counter = 0;
int minesflagged_counter = 0;
bool endgame_lose = false;
string rollNumber;
time_t start_time = time(0);
time_t game_time;
time_t current_time;

int main() {
    cout << "Rules:" << endl;
    cout << "Enter 'o', then enter values i and j to open cell [i][j]." << endl;
    cout << "Enter 'f', then enter values i and j to place or remove flag on cell [i][j]." << endl;
    cout << endl;
    if (!login(rollNumbers, leaderboardSize)) {
        registerUser(rollNumbers, scores, leaderboardSize);
    }
   loadfrom_file("leaderboard.txt", rollNumbers, scores, leaderboardSize);
displayLeaderboard(rollNumbers, scores, leaderboardSize);
saveto_file("leaderboard.txt", rollNumbers, scores, leaderboardSize);
    difficulty();
    return 0;
}
void updateLeaderboard(string rollNumber, int score, string rollNumbers[], int scores[], int &leaderboardSize) {
    if (leaderboardSize < max_players) {
        rollNumbers[leaderboardSize] = rollNumber;
        scores[leaderboardSize] = score;
        leaderboardSize++;
    }
    else {
        cout << "Full leaderboard." << endl;
    }
}
void displayLeaderboard(string rollNumbers[], int scores[], int leaderboardSize) {
    cout << "\nLeaderboard:\n";
    cout << setw(15) << "Roll Number" << setw(10) << "Score" << endl;
    for (int i = 0; i < leaderboardSize; i++) {
        cout << setw(15) << rollNumbers[i] << setw(10) << scores[i] << endl;
    }
}
bool login(string rollNumbers[], int leaderboardSize) {
    string rollNumber;
    cout << "Enter your roll number: ";
    getline(cin, rollNumber);

    for (int i = 0; i < leaderboardSize; i++){
        if (rollNumbers[i] == rollNumber) {
            cout << "Welcome back, player " << rollNumber << "!\n";
            return true;
        }
    }
    cout << "User not found. ";
    return false;
}
void registerUser(string rollNumbers[], int scores[], int &leaderboardSize) {
    cout << "Enter your roll number to register: ";
    getline(cin, rollNumber);
    rollNumbers[leaderboardSize] = rollNumber;
    scores[leaderboardSize] = 0;
    leaderboardSize++;
    cout << "Registration successful. Welcome, player " << rollNumber << "!\n";
}
void saveto_file(string file, string rollNumbers[], int scores[], int leaderboardSize) {
    ofstream fout("leaderboard.txt");
    if (fout.is_open()) {
        fout << "Roll Number,scores[leaderboardSize]\n";
        for (int i = 0; i < leaderboardSize; i++) {
            fout << rollNumbers[i] << "," << scores[i] << "\n";
        }
        fout.close();
    } else {
        cout << "Unable to open the file to save the data.\n";
    }
}
void loadfrom_file(string file, string rollNumbers[], int scores[], int &leaderboardSize){
    ifstream fin("leaderboard.txt");
    if (fin.is_open()){
        string line;
        getline(fin, line); 
        leaderboardSize = 0;
        while (getline(fin, line) && leaderboardSize < max_players) {
            int pos = line.find(',');
            if (pos != string::npos) {
                rollNumbers[leaderboardSize] = line.substr(0, pos);
			    scores[leaderboardSize] = atoi(line.substr(pos + 1).c_str());
            leaderboardSize++;
            }
        }
        fin.close();
    } else {
        cout << "Unable to open the file to load the data.\n";
    }
}
void cell_number(int i, int j, int size) {
    if (i >= 0 && i < size && j >= 0 && j < size && tablemine_position[i][j] != 'X')
        tablemine_position[i][j]++;
}
void createmine_positions(int size, int n) {
    int counter = 0;
    srand(time(0));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            tablemine_position[i][j] = '0';
    while (counter < n) {
        int i = rand() % size;
        int j = rand() % size;
        if (tablemine_position[i][j] == '0') {
            tablemine_position[i][j] = 'X';
            cell_number(i - 1, j, size);
            cell_number(i + 1, j, size);
            cell_number(i, j - 1, size);
            cell_number(i, j + 1, size);
            cell_number(i - 1, j - 1, size);
            cell_number(i - 1, j + 1, size);
            cell_number(i + 1, j - 1, size);
            cell_number(i + 1, j + 1, size);
            counter++;
        }
    }
}
void createtable(int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            table[i][j] = '*';
}
void printtable(char arr[][100], int size) {
    cout << "    ";
    for (int i = 0; i < size; i++)
        cout << setw(3) << i;
    cout << endl << "   ";
    for (int i = 0; i <= size * 3 ; i++)
        cout << "_";
    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << setw(3) << i << "|";
        for (int j = 0; j < size; j++)
            cout << setw(3) << arr[i][j];
        cout << endl;
    }
}
void opencell(int size){
    int i, j;
    do
        cin >> i >> j;
    while (i < 0 || i >= size || j < 0 || j >= size);

    if (tablemine_position[i][j] == 'X') {
        table[i][j] = 'X';
        endgame_lose = true;

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (tablemine_position[i][j] == 'X')
                    table[i][j] = 'X';
    }
    else
        reveal(i, j, size);
}

void placeflag(int size) {
    int i, j;
    do
        cin >> i >> j;
    while (i < 0 || i >= size || j < 0 || j >= size);

    if (table[i][j] == '*') {
        table[i][j] = 'F';
        flag_counter++;

        if (tablemine_position[i][j] == 'X')
            minesflagged_counter++;
    }
}
void removeflag(int size)
{    int i, j;
    do
        cin >> i >> j;
    while (i < 0 || i >= size || j < 0 || j >= size);
    if (table[i][j] == 'F') {
        table[i][j] = '*';
        flag_counter--;
        if (tablemine_position[i][j] == 'X')
            minesflagged_counter--;
 }
}
void input_symbol(int size){
    cin >> symbol;
    switch (symbol) {
    case 'o':
        opencell(size);
        break;
    case 'f':
        placeflag(size);
        break;
    case 'u':
    	removeflag(size);
    	break;
    default:
        input_symbol(size);
}
}
void reveal(int i, int j, int size){
    if (table[i][j] == '*' && tablemine_position[i][j] != 'X' && i >= 0 && i < size && j >= 0 && j < size) {
        table[i][j] = tablemine_position[i][j];
        if (tablemine_position[i][j] == '0') {
            table[i][j] = '-';
            reveal(i, j - 1, size);
            reveal(i, j + 1, size);
            reveal(i - 1, j - 1, size);
            reveal(i + 1, j - 1, size);
            reveal(i + 1, j + 1, size);
            reveal(i - 1, j + 1, size);
            reveal(i - 1, j, size);
            reveal(i + 1, j, size);
        }
    }
}
bool endgame_wincheck(int size, int totalMines) {
    int correctlyFlagged = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        	
            if (tablemine_position[i][j] == 'X' && table[i][j] == 'F') {
                correctlyFlagged++;
            }
        }
    }
    return correctlyFlagged == totalMines;
}
void difficulty() {
    char difficulty;
    cout << "Select difficulty (e for easy, m for medium, h for hard, c for custom): ";
    cin >> difficulty;
    int size, n;
    switch (difficulty) {
    case 'e':
       size = 6;
      n = 8;
        break;
    case 'm':
      size = 8;
        n = 12;
       break;
    case 'h':
       size = 10;
       n = 20;
      break;
    case 'c':
       cout << "Enter custom grid size(upto 38 size) : ";
        cin >> size;
        if (size>38)
        {
        	cout<<"(Invalid input)Please enter value upto 38."<<endl;
        	break;
		}
       cout << "Enter custom mine count: ";
        cin >> n;
      break;
    default:
      cout << "Invalid choice. Continuing with medium difficulty." << endl;
      size = 7;
       n = 10;
    }

    if (size < 1 || n < 1 || n > size * size){
       cout << "Invalid custom grid or mine count. Using default medium difficulty." << endl;
        size = 7;
        n = 10;
    }
    createtable(size);
    createmine_positions(size, n);
    game(size, n);
}
void game(int size, int n) {
    start_time = time(0); 
    while (!endgame_lose && !endgame_wincheck(size, n)) {
        current_time = time(0);  
        game_time = current_time - start_time;
        if (game_time >= 60) {
            endgame_lose = true;
            break;
    }
        printtable(table, size);
        cout << endl << "Flags: " << flag_counter << endl;
        cout << "Time: " << game_time << " seconds" << endl;  
        input_symbol(size);
    }
    if (endgame_lose) {
        printtable(table, size);
        cout << endl << "GAME OVER" << endl;
    }
    if (endgame_wincheck(size, n)) {
        cout << "Time to complete: " << game_time - start_time << " seconds" << endl << "YOU WIN!" << endl;
      updateLeaderboard(rollNumber, game_time - start_time, rollNumbers, scores, leaderboardSize);
        saveto_file("leaderboard.txt", rollNumbers, scores, leaderboardSize);
    }
}
