/*************************************************************************************
 * Title: driver.cpp                                              
 * start date: March 20th, 2023
 * due date: April 3rd, 2023
 * Class: CSC 1310 - 001
 * Project: 3
 * Author: Behnjamin H. Barlow
 * Partner: Aidan Gillespie
 * Purpose: The header file contains two classes: a hash table class, and a private
 * 			nested class for an entry in the has table.
 * Description: This program is a hash table, which is used to store a list of users
 * 				and their login credentials with the hash table header file as a 
 * 				server and the driver as being a local client.
**************************************************************************************/


#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
using std::string;

class hashTable
{
	private:
		class entry; 		//nested class
		int tableSize; 		//array size
		entry** hashArray;  //array of entrys 
		int hash(string);
		entry* getEntry(string);

	public:
		hashTable(int);
		~hashTable();
		string generateSalt();
		string getSalt(string s);
		void addentry(string, string, string);
		bool validateLogin(string, string);
		bool removeUser(string, string);	
};

class hashTable::entry
{
	private:
		string uname;	 //username
		string salt; 	 //salt for user password
		string passHash; //hashed password
		
	public:
		entry* next;
		entry(string, string, string);
		string getUsername();
		string getSalt();
		string getHashedpwd();

};

#endif