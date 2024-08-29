/*************************************************************************************
 * Title: driver.cpp                                              
 * start date: March 20th, 2023
 * due date: April 3rd, 2023
 * Class: CSC 1310 - 001
 * Project: 3
 * Author: Behnjamin H. Barlow
 * Partner: Aidan Gillespie
 * Purpose: The source file for the hash table class.
 * Description: This program is a hash table, which is used to store a list of users
 * 				and their login credentials with the hash table header file as a 
 * 				server and the driver as being a local client.
**************************************************************************************/


#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "hashTable.h"
#include "sha256.cpp"
using std::string;
using std::cout;
using std::endl;


//----------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------HASHTABLE FUNCTIONS-----------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------------------------------PUBLIC------------------------------------------------------------------------//
    /* 
        Function: hashTable
        Purpose: dynamically allocates a new array of entry pointers and sets every array element to null. 
				 Also seeds the random number generator that we’ll use to generate the users’ salts.                 
    */
hashTable::hashTable(int size)
{	
	tableSize = size;
	hashArray = new entry*[size];//dynamically allocates array
	for(int i = 0; i < size; i++)
	{
		hashArray[i] = NULL;//sets all values to NULL
	}
	srand (time(0));//so every user doesn't gets the same salt
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: ~hashTable
        Purpose: deletes every user from the has table by looping through the array and deleting the linked list in every bucket. 
    */
hashTable::~hashTable()
{	
	entry* head = NULL;
	
    for(int i=0; i<tableSize; i++)
    {	
		head = hashArray[i];
		if(head == NULL)
		{
			continue; //keeps itterating if bucket is empty
		}
		else{
			while(head)
			//while head not NULL
			{
				hashArray[i] = head->next;
				delete head; // deletes node
				head = hashArray[i];
			}
		}
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------//

	/* 
        Function: generateSalt
        Purpose: Do Not Touch            
    */
string hashTable::generateSalt()
{
	string s = "";
	
	for(int i = 0; i < tableSize; i++)
		s = s + char('!' + (rand() % 93));
	return s;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: getSalt
        Purpose: This call the getEntry function to find the user associated with the key we got as a parameter.
				 Once we have a user, we can call the getSalt function for that user.
    */
string hashTable::getSalt(string userStr)
{
	entry* user = getEntry(userStr); // accesing users node
	return user->getSalt();	
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: addentry
        Purpose: dynamically allocate a new entry, call the hash function to determine which bucket the new user will be in,
				 and add them to the list in that bucket. Checks top make sure I am not adding a username that already exists in the table.            
    */
void hashTable::addentry(string name, string salt, string hashPass)
{
	entry* newEntry = new entry(name,salt,hashPass); // dynamically allocates space for it
	int bucket = hash(newEntry->getUsername()); // getting which bucket its in
	entry* temp = hashArray[bucket]; 

	if(temp == NULL) 
	//if it empty is inserts it at the front
	{
		hashArray[bucket] = newEntry;
	}
	else{
		while(temp->next != NULL)     
		{
			if(newEntry->getUsername() == temp->getUsername()) 
			// return if username is taken 
			{
				cout << "This username is already taken.\n";
				return;
			}
			else
			{
				//itterates 
				temp = temp->next;
			}
		}
		//assigning to place 
		temp->next = newEntry;
	}
	
}

//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: validateLogin
        Purpose: checks to see if the user has entered the correct login credentials calling the getEntry function to find the data that goes
				 with the username they entered, then checks if the password hash that was sent matches the password hash that was saved.
    */
bool hashTable::validateLogin(string userName, string userPass)
{
	entry* validate = getEntry(userName); // getting the node that stores this users data

	if(validate)
	{
		string temp = sha256(userPass + validate->getSalt());
		//checking to see if what the user inputed once its hashed with its salt is = to what is stored in that node
		if(validate->getHashedpwd() == temp)
		{	
			return true;
		}
	}
	return false;
	
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: removeUser
        Purpose: Remove User by finding the user who’s trying to remove themself. Then verify their login 
				 credentials, then we can remove their entry from the table.
    */
bool hashTable::removeUser(string userName, string userPass)
{
	entry* remove = getEntry(userName); // accesses node that stores user data
	int bucket = hash(remove->getUsername());//gets Bucket of where its stored
	entry* prevNode = remove;

	if(remove->getUsername() == userName)
	 //checks to see if there is only one node
	{
		hashArray[bucket] = remove->next;
		delete remove; // deletes Node
		return true;
	}
	else{
		remove = remove->next;
		while(remove){
			if((remove->getUsername() == userName )) 
			//itterates till it finds the one the user wants to delete and removes it
			{
				prevNode->next = remove->next;
				delete remove; // deletes Node
				return true;
			}
			prevNode = remove;
			remove = remove->next;
		}
	}
	return false;
}
//-----------------------------------------------------------------------PRIVATE----------------------------------------------------------------------//
	/* 
        Function: getEntry
        Purpose: Retrive a user by accessing the head node assosiated with it. Apply the hash function to the key to figure out which bucket
				 we need to look in, traverse the list in that bucket until we (find/or don't) the node with a matching key, then return that (node/NULL). 
    */
hashTable::entry* hashTable::getEntry(string key)
{
	int bucket = hash(key);//gets Bucket of where its stored
	entry* temp = hashArray[bucket];

	while(temp->getUsername() != key && temp)  // itterating through loop 
	{
		temp = temp->next;
	}
	return temp;

}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: hash
        Purpose: To map a key onto a bucket to figure out where it belongs in the hash table by
				 adding together the ascii values of each character in the string.
    */
int hashTable::hash(string key)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         h(string key)
{
	int sum = 0;
	for(int i = 0; i < key.size(); i++)
		sum += key[i];
	
	return sum % tableSize;
}



//----------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------ENTRY FUNCTION--------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: entry
        Purpose: To assigns each attribute to the given parameter, and sets the next pointer to null.       
    */
hashTable::entry::entry(string userName, string s, string hashPass)
{

	uname = userName;
	salt = s;
	passHash = sha256(hashPass + s); // hashes the password and stores it
	next = NULL;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: getUsername()
        Purpose: To return the username          
    */
string hashTable::entry::getUsername()
{
	return uname;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: getSalt()
        Purpose: To return the salt for hashing the password         
    */
string hashTable::entry::getSalt()
{
	return salt;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------//
	/* 
        Function: getHashedpwd()
        Purpose: To return the hashed password        
    */
string hashTable::entry::getHashedpwd()
{
	return passHash;
}