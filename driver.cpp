/*************************************************************************************
 * Title: driver.cpp                                              
 * start date: March 20th, 2023
 * due date: April 3rd, 2023
 * Class: CSC 1310 - 001
 * Project: 3
 * Author: Behnjamin H. Barlow
 * Partner: Aidan Gillespie
 * Purpose: The driver will read a list of sample users, and allower the user to 
 * 			attempt to login, create a new account, or delete their account.
 * Description: This program is a hash table, which is used to store a list of users
 * 				and their login credentials with the hash table header file as a 
 * 				server and the driver as being a local client.
**************************************************************************************/

#include <iostream>
#include <fstream>
#include "sha256.h"
#include "hashTable.h"
using namespace std;

int main()
{
	
	ifstream file("testlist.txt");
	int menuChoice;
	int numUsers = 0;
	string user, pwd, salt;
	string username, password;
	hashTable* hashTab; 

	if(file.is_open()) {
		file >> numUsers;
		file.ignore(1, '\n');
		hashTable* temp = new hashTable(int(numUsers)); //dynamically allocate table
		hashTab = temp;
		while(getline(file, user))
		{
			getline(file, pwd);
			salt = hashTab->generateSalt(); // generate salt
			hashTab->addentry(user,salt,pwd); // add user to hash table
		}
		

	}
		
	do
	{
		cout << "\nWhat would you like to do?\n";
		cout << "1.  Login.\n";
		cout << "2.  Create account.\n";
		cout << "3.  Delete account.\n";
		cout << "4.  Exit.\n";
		cout << "CHOOSE 1-4:  ";
		cin >> menuChoice;
		while(menuChoice < 1 || menuChoice > 4)
		{
			cout << "That is not a valid choice.\n";
			cout << "CHOOSE 1-4:  ";
			cin >> menuChoice;
		}
		
		switch(menuChoice)
		{
			case 1:	cout << "enter your username:  ";
					cin >> username;
					cout << "enter your password:  ";
					cin >> password;
					
					if(hashTab->validateLogin(username,password))//check if the user's credentials are correct
						cout << "login successful\n";
					else
						cout << "invalid credentials\n";
					
					break;
					
			case 2: cout << "enter your new username:  ";
					cin >> username;
					cout << "enter your new password:  ";
					cin >> password;
				
					salt = hashTab->generateSalt(); // generate salt
					hashTab->addentry(username,salt,password); // add user to hash table
					
					break;
					
			case 3:	cout << "enter your username:  ";
					cin >> username;
					cout << "enter your password:  ";
					cin >> password;
					
					if(hashTab->removeUser(username,password))//remove the user from the table and check if they were removed successfully
						cout << "user removed successfully.\n";
					else
						cout << "invalid credentials, cannot remove user.\n";
					break;
					
			case 4:	cout << "goodbye" << endl;
					
					delete hashTab;//delete the hash table
					break;
					
		}
		
	} while(menuChoice != 4);

	return 0;
}