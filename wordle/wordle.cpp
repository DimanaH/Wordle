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

const char* COLOR_GREEN = "\033[1;32m"; //right letter, right position
const char* COLOR_YELLOW = "\033[1;33m"; // right letter, wrong position
const char* COLOR_WHITE = "\033[1;37m"; // wrong letter
const char* COLOR_RESET = "\033[0m";    //standard color

const int MAX_ATTEMPTS = 6;  // maximum number of attempts
const int WORD_LENGTH = 5;   // length of the secret word


//User management functions ->

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

bool loginUser(char loggedUsername[]) {
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
        cout << "Login successful\n";
        copyString(loggedUsername, username);
        return true;
    }

    else {
        cout << "Invalid username or password\n";
        return false;
    }
}

//Main logic game functions ->

bool isValidWord(const char guess[]) {
	//check length of the word
    int length = 0;
    while (guess[length] != '\0') length++;
    if (length != WORD_LENGTH) return false;

	// check if the word exists in the file
    ifstream file("words.txt");
    if (!file.is_open()) return false;

    char word[MAX_SIZE];
    bool found = false;
    while (file >> word) {
        int i = 0;
        while (word[i] != '\0' && guess[i] != '\0' && word[i] == guess[i]) i++;
        if (word[i] == '\0' && guess[i] == '\0') {
            found = true;
            break;
        }
    }
    file.close();
    return found;
}


void loadRandomWord(char word[]) {
    ifstream file("words.txt");
    if (!file.is_open()) {
        cout << "Error opening words file\n";
        word[0] = '\0';
        return;
    }

    const int MAX_WORDS = 2342;
	char words[MAX_WORDS][MAX_SIZE];// array to store words from file
    int count = 0;

	while (file >> words[count] && count < MAX_WORDS) {//reads words from file and store them in array 
        count++;
    }
    file.close();

    if (count == 0) {
        cout << "No words available in the file.\n";
        word[0] = '\0';
        return;
    }

	srand(82);// seed for random number generator
	int index = rand() % count;// get random index

	copyString(word, words[index]);// copy selected word to output
}

void printColoredResult(const char secret[], const char guess[]) { // prints the guess with colors based on correctness
    int i = 0;
    while (guess[i] != '\0' && secret[i] != '\0') { 
		if (guess[i] == secret[i]) { // correct letter and position
            cout << COLOR_GREEN << guess[i] << COLOR_RESET;
        }
        else {
            bool found = false;
            int j = 0;
			while (secret[j] != '\0') { // check if letter exists in secret word
                if (guess[i] == secret[j]) {
                    found = true;
                    break;
                }
                j++;
            }
            cout << (found ? COLOR_YELLOW : COLOR_WHITE) << guess[i] << COLOR_RESET;
        }
        i++;
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
    int i = 0;
    while (guess[i] != '\0') {
        if (guess[i] != secret[i])
            return false;
        i++;
    }
    return true;
}


void playWordle() {
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

    if (!copyWordsExcluding(wordToRemove) ||
		remove("words.txt") != 0 ||     // delete original file
		rename("temp.txt", "words.txt") != 0) {  // rename temp file to original file name
        cout << "Error processing file.\n";
        return;
    }

    cout << "Word removed successfully.\n";
}


void showAdminMenu() {
    cout << "--- Administrator Menu ---\n";
    cout << "1. Add word\n";
    cout << "2. Remove word\n";
    cout << "3. Back\n";
    cout << "Choice: ";
}

void adminMenu() {
    int choice = 0;
    while (choice != 3) {
        showAdminMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            addWord();
            break;
        case 2:
            removeWord();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice\n";
        }
    }
}

//Menu functions ->

void showMainMenu()
{
    cout << "--- Wordle Game ---\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
}


int main()
{
    int choice = 0;
    char loggedUser[MAX_SIZE];

    while (choice != 3)
    {
        showMainMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (loginUser(loggedUser)) {
                if (isAdmin(loggedUser)) {
                    adminMenu();
                }
                else {
                    playWordle();
                }
            }
            break;

        case 2:
            registerUser();
            break;

        case 3:
            cout << "Exitin\n";
            break;

        default:
            cout << "Invalid choice, try again.\n";
        }

        cout << "\n";
    }

    return 0;
}
