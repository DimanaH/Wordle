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
using namespace std;



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
            cout << "Login\n";
            break;
        case 2:
            cout << "Register\n";
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
