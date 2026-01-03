/**
*
* Solution to course project # 09
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Dimana Haralambieva
* @idnumber 1MI0600660
* @compiler VC
*
* Wordle game
*
*/

#include <iostream>
#include <fstream>// for file operations
#include <cstdlib> // for rand() and srand()

using namespace std;

const int MAX_SIZE = 50;
const int MAX_USERS = 50;
const int MAX_ATTEMPTS = 6;  // maximum number of attempts
const int WORD_LENGTH = 5;   // length of the secret word

const char* COLOR_GREEN = "\033[1;32m"; //right letter, right position
const char* COLOR_YELLOW = "\033[1;33m"; // right letter, wrong position
const char* COLOR_WHITE = "\033[1;37m"; // wrong letter
const char* COLOR_RESET = "\033[0m";    //standard color
const char* COLOR_BLACK_TEXT = "\033[30m";
const char* COLOR_WHITE_BG = "\033[47m";

const int MAX_IGNORE = 10000;


/* ---------- Leaderboard structure ---------- */

struct Leaderboard {
    char usernames[MAX_USERS][MAX_SIZE];
    int games[MAX_USERS];
    int wins[MAX_USERS];
    int count;
};

/* ---------- String helper functions ---------- */

bool stringsEqual(const char a[], const char b[]) { // compares two strings
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void copyString(char destination[], const char source[]) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

/* ---------- Menus ---------- */

void showMainMenu()
{
    cout << COLOR_BLACK_TEXT << COLOR_WHITE_BG
        << "--- Wordle Game ---\n" << COLOR_RESET;
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
}

int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        if (cin >> choice && choice >= min && choice <= max) {
            return choice;
        }
        cout << "Invalid input. Enter number between "
            << min << " and " << max << ": ";
        cin.clear();
        cin.ignore(MAX_IGNORE, '\n');
    }
}

void showUserMenu() {
    cout << COLOR_BLACK_TEXT << COLOR_WHITE_BG
        << "--- User Menu ---\n" << COLOR_RESET;
    cout << "1. Play Wordle\n";
    cout << "2. View Leaderboard\n";
    cout << "3. Logout\n";
    cout << "Choice: ";
}

/* ---------- User management functions ---------- */

bool userExists(const char username[]) {
    ifstream file("users.txt");// opens file for reading
    if (!file.is_open()) return false;

    char fileUsername[MAX_SIZE];
    char filePassword[MAX_SIZE];

    while (file >> fileUsername >> filePassword) { // reads username and password from file
        if (stringsEqual(fileUsername, username)) {
            file.close();// close file before returning
            return true;
        }
    }

    file.close();
    return false;
}

void registerUser() {
    char username[MAX_SIZE];
    char password[MAX_SIZE];

    cout << "Enter username: ";
    cin >> username;

    if (userExists(username)) {// check if username already exists
        cout << "Username already exists\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);// ios adds new text at the end of the file
    if (file.is_open()) {
        file << username << " " << password << "\n"; // writes username and password in the file
        file.close();
        cout << "Registration successful\n";
    }
    else {
        cout << "Error opening file\n";
    }
}

bool loginUser(char loggedUsername[]) {// returns true if login is successful
    char username[MAX_SIZE];
    char password[MAX_SIZE];

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users.txt");// opens file for reading
    if (!file.is_open()) {
        cout << "Error opening file\n";
        return false;
    }

    char fileUsername[MAX_SIZE];
    char filePassword[MAX_SIZE];

    while (file >> fileUsername >> filePassword) {// reads username and password from file
        if (stringsEqual(fileUsername, username) &&
            stringsEqual(filePassword, password)) {
            copyString(loggedUsername, username);
            file.close();
            cout << "Login successful\n\n";
            return true;
        }
    }

    file.close();
    cout << "Invalid username or password\n";
    return false;
}

/* ---------- Leaderboard functions ---------- */

void loadLeaderboard(Leaderboard& lb) {
    ifstream file("leaderboard.txt");
    lb.count = 0;
    if (!file.is_open()) return;

    while (file >> lb.usernames[lb.count]
        >> lb.games[lb.count]
        >> lb.wins[lb.count]) {
        lb.count++;
        if (lb.count >= MAX_USERS) break;
    }
    file.close();
}

void saveLeaderboard(const Leaderboard& lb) {
    ofstream file("leaderboard.txt");
    for (int i = 0; i < lb.count; i++) {
        file << lb.usernames[i] << " "
            << lb.games[i] << " "
            << lb.wins[i] << "\n";
    }
    file.close();
}

int findUserIndex(const Leaderboard& lb, const char username[]) {
    for (int i = 0; i < lb.count; i++) {
        if (stringsEqual(lb.usernames[i], username)) return i;
    }
    return -1;
}

void updateLeaderboard(Leaderboard& lb, const char username[], bool won) {
    int index = findUserIndex(lb, username);// check if user exists

    if (index == -1 && lb.count < MAX_USERS) {
        copyString(lb.usernames[lb.count], username);// first game
        lb.games[lb.count] = 1;
        lb.wins[lb.count] = won ? 1 : 0;
        lb.count++;
    }
    else if (index != -1) {
        lb.games[index]++;
        if (won) lb.wins[index]++;
    }

    saveLeaderboard(lb);
}

void swapPlayers(Leaderboard& lb, int i, int j) {
    char tempName[MAX_SIZE];
    int tempGames = lb.games[i];
    int tempWins = lb.wins[i];

    copyString(tempName, lb.usernames[i]);

    copyString(lb.usernames[i], lb.usernames[j]);
    lb.games[i] = lb.games[j];
    lb.wins[i] = lb.wins[j];

    copyString(lb.usernames[j], tempName);
    lb.games[j] = tempGames;
    lb.wins[j] = tempWins;
}

/* ---------- Main ---------- */

int main() {
    Leaderboard leaderboard;
    loadLeaderboard(leaderboard);

    int choice = 0;
    char loggedUser[MAX_SIZE];

    while (choice != 3) {
        showMainMenu();
        choice = getValidChoice(1, 3);

        switch (choice) {
        case 1:
            if (loginUser(loggedUser)) {
                cout << "Welcome, " << loggedUser << "!\n";
            }
            break;
        case 2:
            registerUser();
            break;
        case 3:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice\n";
        }

        cout << "\n";
    }

    return 0;
}
