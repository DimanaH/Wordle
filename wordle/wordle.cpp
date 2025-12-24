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

using namespace std;

const int MAX_SIZE = 50;

const char* COLOR_GREEN = "\033[1;32m"; //right letter, right position
const char* COLOR_YELLOW = "\033[1;33m"; // right letter, wrong position
const char* COLOR_WHITE = "\033[1;37m"; // wrong letter
const char* COLOR_RESET = "\033[0m";    //standard color



//User management functions ->

bool stringsEqual(const char a[], const char b[]) { // compares twо strings
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i]; 
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


bool loginUser() {
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

	if (found) {// if user is found
        cout << "Login successful\n";
        return true;
    }
    else {
        cout << "Invalid username or password\n";
        return false;
    }
}
//Main logic game functions ->

void loadSecretWord(char word[])
{

    ifstream file("words.txt");
    if (!file.is_open())
    {
        cout << "Error opening words file\n";
        return;
    }

    file >> word; 
    file.close();
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

    char testWord[MAX_SIZE];
    loadSecretWord(testWord);
    cout << "Secret word (test): " << testWord << endl;
  


    int choice = 0;

    while (choice != 3)
    {

        showMainMenu();
       
		cin >> choice;
        switch (choice)
        {
        case 1:
            loginUser();
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
