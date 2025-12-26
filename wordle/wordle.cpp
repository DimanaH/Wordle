/**
*
* Solution to course project # 09
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Dimana Haralambieva
* @idnumber 1Mi0600660
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




bool stringsEqual(const char a[], const char b[]) { // compares twо strings
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


void showMainMenu()
{
    cout <<COLOR_BLACK_TEXT<<COLOR_WHITE_BG<< "--- Wordle Game ---\n"<< COLOR_RESET;
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
        cin.ignore(10000, '\n');
    }
}
//User management functions ->


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

bool loginUser(char loggedUsername[]) {// returns true if login is successful and copies username to loggedUsername
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
    bool found = false;

	while (file >> fileUsername >> filePassword) {// reads username and password from file
		if (stringsEqual(fileUsername, username) && stringsEqual(filePassword, password)) {// compares input with file data
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "Login successful\n\n";
        copyString(loggedUsername, username); 
        return true;
    }

    else {
        cout << "Invalid username or password\n";
        return false;
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






//leadboard functions ->

char lbUsernames[MAX_USERS][MAX_SIZE];
int lbGames[MAX_USERS];
int lbWins[MAX_USERS];
int lbCount = 0;


void showLeaderboardMenu() {
    cout << COLOR_BLACK_TEXT << COLOR_WHITE_BG<<"--- Leaderboard ---\n"<<COLOR_RESET;
    cout << "1. Sort by winrate\n";
    cout << "2. Sort by games played\n";
    cout << "3. Back\n";
    cout << "Choice: ";
}

void loadLeaderboard() {
    ifstream file("leaderboard.txt");
    if (!file.is_open()) return;

    lbCount = 0;
    while (file >> lbUsernames[lbCount] >> lbGames[lbCount] >> lbWins[lbCount]) {
        lbCount++;
        if (lbCount >= MAX_USERS) break;
    }
    file.close();
}

void saveLeaderboard() {
    ofstream file("leaderboard.txt");//if the file exists, it will be overwritten, otherwise it creats it
    for (int i = 0; i < lbCount; i++) {
        file << lbUsernames[i] << " " << lbGames[i] << " " << lbWins[i] << "\n";
    }
    file.close();
}

int findUserIndex(const char username[]) {
    for (int i = 0; i < lbCount; i++) {
        if (stringsEqual(lbUsernames[i], username)) return i;
    }
    return -1;
}

void updateLeaderboard(const char username[], bool won) {
    int index = findUserIndex(username);// check if user exists in leaderboard
    if (index == -1) {
        copyString(lbUsernames[lbCount], username);// the user plays for the first time
        lbGames[lbCount] = 1;
        lbWins[lbCount] = won ? 1 : 0;
        lbCount++;
    }
    else { // user already exists in leaderboard
        lbGames[index]++;
        if (won) lbWins[index]++;
    }
    saveLeaderboard();
}

void swapPlayers(int i, int j) {
    char tempName[MAX_SIZE];
    int tempGames = lbGames[i];
    int tempWins = lbWins[i];

    copyString(tempName, lbUsernames[i]);

    copyString(lbUsernames[i], lbUsernames[j]);
    lbGames[i] = lbGames[j];
    lbWins[i] = lbWins[j];

    copyString(lbUsernames[j], tempName);
    lbGames[j] = tempGames;
    lbWins[j] = tempWins;
}

void printLeaderboardByGames() {
    cout << "--- Leaderboard (by games played) ---\n";

    for (int i = 0; i < lbCount - 1; i++) {
        for (int j = 0; j < lbCount - i - 1; j++) {
            if (lbGames[j] < lbGames[j + 1]) {
                swapPlayers(j, j + 1);
            }
        }
    }

    for (int i = 0; i < lbCount; i++) {
        double rate = lbGames[i] > 0 ? (double)lbWins[i] / lbGames[i] * 100 : 0;
        cout << lbUsernames[i] << ": "
            << lbGames[i] << " games, "
            << lbWins[i] << " wins ("
            << rate << "% winrate)\n";
    }
}
void printLeaderboardByWinrate() {
    cout << "--- Leaderboard (by winrate) ---\n";

    for (int i = 0; i < lbCount - 1; i++) {
        for (int j = 0; j < lbCount - i - 1; j++) {
            double rate1 = lbGames[j] > 0 ? (double)lbWins[j] / lbGames[j] : 0;
            double rate2 = lbGames[j + 1] > 0 ? (double)lbWins[j + 1] / lbGames[j + 1] : 0;

            if (rate1 < rate2) {
                swapPlayers(j, j + 1);
            }
        }
    }

    for (int i = 0; i < lbCount; i++) {
        double rate = lbGames[i] > 0 ? (double)lbWins[i] / lbGames[i] * 100 : 0;
        cout << lbUsernames[i] << ": "
            << lbGames[i] << " games, "
            << lbWins[i] << " wins ("
            << rate << "% winrate)\n";
    }
}

double GetWinrate() {
    if (lbGames[0] == 0) return 0;
    return (double)lbWins[0] / lbGames[0] * 100;
}


void viewLeaderboard() {
    int choice = 0;

    while (choice != 3) {
        showLeaderboardMenu();
        choice = getValidChoice(1, 3);

        switch (choice) {
        case 1:
            printLeaderboardByWinrate();
            break;
        case 2:
            printLeaderboardByGames();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice\n";
        }

        cout << "\n";
    }
}


//Admin functions ->

bool isAdmin(const char username[]) {
    return stringsEqual(username, "admin");
}


bool wordExists(const char wordToCheck[]) {
    ifstream file("words.txt");
    if (!file.is_open()) return false;

    char word[MAX_SIZE];
    while (file >> word) {
        if (stringsEqual(word, wordToCheck)) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}


void addWord() {
    char newWord[MAX_SIZE];
    cout << "Enter word to add: ";
    cin >> newWord;

    int length = 0;
    while (newWord[length] != '\0') length++;
    if (length != WORD_LENGTH) {
        cout << "Word must be exactly " << WORD_LENGTH << " letters long.\n";
        return;
    }

    if (wordExists(newWord)) {
        cout << "Word already exists.\n";
        return;
    }

    ofstream file("words.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening words file.\n";
        return;
    }

    file << newWord << "\n";
    file.close();
    cout << "Word added successfully.\n";
}


bool copyWordsExcluding(const char excludeWord[]) {
    ifstream inputFile("words.txt");
    ofstream tempFile("temp.txt");
    if (!inputFile.is_open() || !tempFile.is_open()) return false;

    char word[MAX_SIZE];
    while (inputFile >> word) {
        if (!stringsEqual(word, excludeWord)) { //write all words except the one to exclude
            tempFile << word << "\n";
        }
    }

    inputFile.close();
    tempFile.close();
    return true;
}


void removeWord() {
    char wordToRemove[MAX_SIZE];
    cout << "Enter word to remove: ";
    cin >> wordToRemove;

    if (!wordExists(wordToRemove)) {
        cout << "Word not found.\n";
        return;
    }

    if (!copyWordsExcluding(wordToRemove) || //if the removal process fails
        remove("words.txt") != 0 ||     // delete original file
        rename("temp.txt", "words.txt") != 0) {  // rename temp file to original file name
        cout << "Error processing file.\n";
        return;
    }

    cout << "Word removed successfully.\n";
}


void showAdminMenu() {
    cout << COLOR_BLACK_TEXT << COLOR_WHITE_BG
        << "--- Administrator Menu ---\n" << COLOR_RESET;
    cout << "1. Add word\n";
    cout << "2. Remove word\n";
    cout << "3. View Leaderboard\n";
    cout << "4. Back\n";
    cout << "Choice: ";
}

void adminMenu() {
    int choice = 0;
    while (choice != 4) {
        showAdminMenu();
        choice = getValidChoice(1, 4);

        switch (choice) {
        case 1:
            addWord();
            break;
        case 2:
            removeWord();
            break;
        case 3:
            viewLeaderboard();
            break;
        case 4:
            break;
        default:
            cout << "Invalid choice\n";
        }

        cout << "\n";
    }
}



//Main logic game functions ->

void loadRandomWord(char word[]) {
    ifstream file("words.txt");
    if (!file.is_open()) {
        cout << "Error opening words file\n";
        word[0] = '\0';
        return;
    }

    char temp[MAX_SIZE];
    int count = 0;
	word[0] = '\0';  // initialize word as empty


    
	srand(GetWinrate());       // seed random number generator, using winrate for more randomness
    //srand(82);

    while (file >> temp) {
        count++;
		if (rand() % count == 0) { // reservoir sampling
            copyString(word, temp);
        }
    }

    file.close();

    if (word[0] == '\0') {
        cout << "No words available in the file.\n";
    }
}




bool isValidWord(const char guess[]) {
	// check length
    int len = 0;
    while (guess[len] != '\0') len++;
    if (len != WORD_LENGTH) return false;

	// check all lowercase letters
    for (int i = 0; i < len; i++) {
		if (!(guess[i] >= 'a' && guess[i] <= 'z')) return false;
    }

    ifstream file("words.txt");
    if (!file.is_open()) return false;

    char word[MAX_SIZE];
    while (file >> word) {
        if (stringsEqual(word,guess)) {  
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}


void printColoredResult(const char secret[], const char guess[]) {
	bool used[WORD_LENGTH] = { false }; // tracks which letters in secret have been matched


    
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            cout << COLOR_GREEN << guess[i] << COLOR_RESET;
            used[i] = true;
        }
        else {
			cout << "_"; // placeholder for second pass
        }
    }
	cout << "\r"; // return to the beginning of the line

	// Second pass for yellow and white letters
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            cout << COLOR_GREEN << guess[i] << COLOR_RESET;
        }
        else {
            bool found = false;
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (!used[j] && guess[i] == secret[j]) {
                    found = true;
                    used[j] = true;
                    break;
                }
            }
            cout << (found ? COLOR_YELLOW : COLOR_WHITE)
                << guess[i] << COLOR_RESET;
        }
    }

    cout << endl;
}


void getValidGuess(char guess[]) {
    while (true) {
        cin >> guess;
        if (isValidWord(guess))
            break;
        cout << "Invalid word. Make sure it has " << WORD_LENGTH
            << " letters and is in the word list.\n";
    }
}

bool isWordGuessed(const char secret[], const char guess[]) {// checks if the guessed word matches the secret word
    return stringsEqual(secret, guess);
}


void playWordle(const char loggedUser[]) {
	char secret[WORD_LENGTH + 1];// +1 for null terminator
    loadRandomWord(secret);
    cout << "New game started! Guess the " << WORD_LENGTH << "-letter word.\n";

    char guess[MAX_SIZE];
    int attempts = 0;
    bool won = false;

    while (attempts < MAX_ATTEMPTS) {
        cout << "Attempt " << (attempts + 1) << "/" << MAX_ATTEMPTS << ": ";
        getValidGuess(guess);
        printColoredResult(secret, guess);

        if (isWordGuessed(secret, guess)) {
            cout << "Congratulations! You guessed the word!\n";
            won = true;
            break;
        }

        attempts++;
    }

    if (!won)
        cout << "Game over! The correct word was: " << secret << endl;

    updateLeaderboard(loggedUser, won);


}




void userMenu(const char loggedUser[]) {
    int choice = 0;

    while (choice != 3) {
        showUserMenu();
        choice = getValidChoice(1, 3);

        switch (choice) {
        case 1:
            playWordle(loggedUser);
            break;
        case 2:
            viewLeaderboard();
            break;
        case 3:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice\n";
        }

        cout << "\n";
    }
}




int main() {
    loadLeaderboard();
    int choice = 0;
    char loggedUser[MAX_SIZE];

    while (choice != 3) {
        showMainMenu();
        choice = getValidChoice(1, 3);  

        switch (choice) {
        case 1:  // Login
            if (loginUser(loggedUser)) {
                if (isAdmin(loggedUser)) {
                    adminMenu();  
                }
                else {
                    userMenu(loggedUser);  
                }
            }
            break;

        case 2:  // Register
            registerUser();
            break;

        case 3:  // Exit
            cout << "Goodbye!\n";
            break;

        default:
            cout << "Invalid choice, try again.\n";
        }

        cout << "\n";
    }

    return 0;
}

