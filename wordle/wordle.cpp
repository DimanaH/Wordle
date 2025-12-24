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


bool stringsEqual(const char a[], const char b[]) { // compares twо strings
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i]; 
}

bool userExists(const char username[]) {
	ifstream file("wordle/users.txt");// opens file for reading
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

	ofstream file("wordle/users.txt", ios::app);// ios adds new text at the end of the file
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

	ifstream file("wordle/users.txt");// opens file for reading
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
