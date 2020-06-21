/**************************************************************************//**
* @file
*
* @brief contains all functions for the HashTable class
******************************************************************************/

#include "hash.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool sort_alphabetically(const HashNode &, const HashNode &);
bool sort_by_frequency(const HashNode &, const HashNode &);

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Default constructor for the HashTable class.
 *
 * @returns none
 *****************************************************************************/

HashTable::HashTable()
{}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: This is a constructor for the class HashTable. It takes in
 *                   the size as a parameter and creates a vector of type
 *                   HashNode. It then initializes the frequency and the
 *                   hashvalue of each node to 0. It also sets the current size
 *                   and the variable that stores the total number of words
 *                   read in to 0.
 *
 * @param[in]  size - the initial size of the vector
 *
 * @returns none
 *****************************************************************************/

HashTable::HashTable(int size)
{
  capacity = size;

  table.resize(capacity); //Resizes the vector to the given size

  //Initializes all the frequency and hash values of the nodes to 0
  for(int i = 0; i < capacity; i++)
  {
      table[i].frequency = 0;
      table[i].hash_value = 0;
  }
  current_size = 0;
  total_words = 0;

}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the word stored at the given position
 *
 * @param[in]  position - an integer storing the requested position of the node
 *
 * @returns the word stored at the given position in the vector
 *****************************************************************************/
string HashTable::getWord(int position)
{
  	return table[position].word;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the frequency stored at the given position
 *
 * @param[in]  position - an integer storing the requested position of the node
 *
 * @returns  the frequency stored at the given position in the vector
 *****************************************************************************/
int HashTable::getFrequency(int position)
{
  	return table[position].frequency;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the hash value stored at the given position
 *
 * @param[in]  position - an integer storing the requested position of the node
 *
 * @returns the hash value stored at the given position in the vector
 *****************************************************************************/
unsigned long HashTable::getHashValue(int position)
{
	return table[position].hash_value;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the current size (Number of filled nodes) of the
 *                   vector
 *
 * @returns current_size - number of filled nodes in the table
 *****************************************************************************/
int HashTable::getCurrentSize()
{
  return current_size;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Sets the current size of the vector to the provided size
 *
 * @param[in]  size - an integer storing the new size
 *
 * @returns none
 *****************************************************************************/
void HashTable::setCurrentSize(int size)
{
  current_size = size;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the total size of the vector
 *
 * @returns capacity - an integer storing the size of the table
 *****************************************************************************/
int HashTable::getCapacity()
{
  return capacity;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Sets the capacity of the vector to the provided size
 *
 * @param[in]  size - an integer storing the new size
 *
 * @returns none
 *****************************************************************************/
void HashTable::setCapacity(int size)
{
  capacity = size;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the number of words inserted in the table.
 *
 * @returns total_words - an integer storing the total number of words
 *****************************************************************************/
int HashTable::getTotalWords()
{
  return total_words;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Sets the total words to theh provided total
 *
 * @param[in]  total - an integer storing the new total
 *
 * @returns none
 *****************************************************************************/
void HashTable::setTotalWords(int total)
{
  total_words = total;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Increments the total words by 1
 *
 * @returns none
 *****************************************************************************/
void HashTable::incrementTotalWords()
{
  total_words++;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Returns the load factor of the table
 *
 * @returns load factor
 *****************************************************************************/
double HashTable::getLoadFactor()
{
  return current_size * 1.0 / capacity;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Calculates the hash value of the provided string. The
 *                   value is calculated by adding 37 and the ascii value of
 *                   each charecter of the string.
 *
 * @param[in, out]  key - a string holding the word
 *
 * @returns hashVal - the hash value of the key
 *****************************************************************************/
unsigned long HashTable:: hash(string &key)
{
    unsigned long hashVal = 0;

    for(char ch : key)
        hashVal = 37 + hashVal + ch;

    return hashVal;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Calculates the index of the word using the hash value
 *
 * @param[in]  hashVal - unsigned long storing the hash value
 *
 * @returns the index of the key
 *****************************************************************************/
unsigned long HashTable::getIndex(unsigned long hashVal)
{
  return hashVal % capacity;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Inserts the key in the table. The function first
 *                   calculates the hash value of the key and then gets the
 *                   index using the hash value. The function checks if the
 *                   word stored at the calculated index is empty. If it is,
 *                   the key is inserted there and the current size is
 *                   increased by 1. If it is not empty, it checks if the word
 *                   at the calculated index is the same as the key. If it is,
 *                   the frequency of the word is incremented by 1. If the word
 *                   is not the same as the key, the index is moved to the next
 *                   position and the process repeats itself till an empty node
 *                   or the same word is found.
 *
 * @param[in, out]  key - a string storing the new word
 *
 * @returns none
 *****************************************************************************/
void HashTable::insertKey(string &key)
{
	bool flag = false;

	unsigned long hash_value = hash(key); //gets the hash value
	unsigned long original_position = getIndex(hash_value); //gets the index
	unsigned long new_position = original_position;

  //If the word is empty
	if(table[new_position].word == "")
	{
		table[new_position].word = key;
		table[new_position].frequency = 1;
		table[new_position].hash_value = hash_value;
		current_size++;
	}
	else if(table[new_position].word != "")
	{
    //Keep checking until an empty word or the same word is found
		do
		{
			if(table[new_position].word == "")
			{
				table[new_position].word = key;
				table[new_position].frequency = 1;
				table[new_position].hash_value = hash_value;
				current_size++;
				flag = true;
			}
			else
			{
				if(table[new_position].word == key)
				{
					table[new_position].frequency++;
					flag = true;
				}
				else if(table[new_position].word != key)
				{
          //Moves to the next index with boundary checking
					new_position = (new_position + 1) % capacity;
					flag = false;
				}
			}
		} while(!flag && new_position != original_position);
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Re-inserts the word into the table. It works the same way
 *                   as the insertKey function except for the fact that it does
 *                   not check if the key is the same as the word at the
 *                   calculated since all the words being reinserted are unique
 *
 * @param[in, out]  key - a string storing the new word
 * @param[in]  frequency - the frequency of the word
 *
 * @returns none
 *****************************************************************************/
void HashTable::rehashInsertKey(string &key, int frequency)
{
	bool flag = false;

  unsigned long hash_value = hash(key); //gets the hash value
	unsigned long original_position = getIndex(hash_value); //gets the index
	unsigned long new_position = original_position;

  //Keep checking until an empty word or the same word is found
	if(table[new_position].word == "")
	{
		table[new_position].word = key;
		table[new_position].frequency = frequency;
		table[new_position].hash_value = hash_value;
	}
	else if(table[new_position].word != "")
	{
		do
		{
			if(table[new_position].word == "")
			{
				table[new_position].word = key;
				table[new_position].frequency = frequency;
				table[new_position].hash_value = hash_value;
				flag = true;
			}
			else
			{
				new_position = (new_position + 1) % capacity;
				flag = false;
			}
      //Moves to the next index with boundary checking
		} while(!flag && new_position != original_position);
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Checks if the word is present in the hash table
 *
 * @param[in, out]  target - a string storing the new word
 *
 * @returns true - if the word is found
 * @returns false - If the word is not present in the table
 *****************************************************************************/
bool HashTable::find(string &target)
{
	unsigned long hash_value = hash(target); //gets the hash value
	unsigned long original_position = getIndex(hash_value); //gets the index
	unsigned long new_position = original_position;

	do
	{
    //If found
		if(table[new_position].word == target)
		{
			return true;
		}
		else if(table[new_position].frequency > 0)
		{
      //Moves to the next index with boundary checking
			new_position = (new_position + 1) % capacity;
		}\
    //If not found
		else if(table[new_position].frequency == 0){
			return false;
		}
	} while(new_position != original_position);

	return false; //If not found
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Sorts the table in the decreasing order of the frequency
 *                   values. If the multiple words have the same frequency,
 *                   then the words are sorted alphabetically
 *
 * @returns none
 *****************************************************************************/
void HashTable::sortTable()
{
  vector<HashNode>::iterator it1 = table.begin();
  vector<HashNode>::iterator it2 = table.begin();

  //Sort the node in the decreasing order of their frequencies
  sort(table.begin(), table.end(), sort_by_frequency);

  //while not an empty node
  while((*it1).frequency > 0)
  {
    //Move the iterator to the last node with the same frequency
	  do
	  {
		    it2++;

    } while((*it1).frequency == (*it2).frequency);

    //Sort the words alphabetically
    sort(it1, it2 - 1, sort_alphabetically);
    it1 = it2; //Set the first iterator to the last iterator
  }
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Takes in two hashnodes and returns true if the frequency
 *                   of the first node is greater than the other
 *
 * @param[in, out]  lhs - hashnode 1
 * @param[in, out]  rhs - hashnode 2
 *
 * @returns true - if greater
 * @returns false - if not greater
 *****************************************************************************/
bool sort_by_frequency(const HashNode &lhs, const HashNode &rhs)
{
    return lhs.frequency > rhs.frequency;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Takes in two hashnodes and returns true if the word
 *                   of the first node is alphabetically greater than the other
 *
 * @param[in, out]  lhs - hashnode 1
 * @param[in, out]  rhs - hashnode 2
 *
 * @returns true - if greater
 * @returns false - if not greater
 *****************************************************************************/
bool sort_alphabetically(const HashNode &lhs, const HashNode &rhs)
{
	if(lhs.word < rhs.word)
		return true;
	return false;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Prints the contents of the table if the node is non-empty
 *
 * @returns none
 *****************************************************************************/
void HashTable::printTable()
{
	int i = 0;

	while(i < capacity)
	{
		if(table[i].frequency > 0)
		{
			cout << table[i].word << " " << table[i].frequency << endl;
		}
		i++;
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Resets the contents of the table to default and then
 *                   resizes it to the new size
 *
 * @param[in]  size - the new size for the table
 *
 * @returns none
 *****************************************************************************/
void HashTable::cleanandresize(int size)
{
  //Reset values to default
  for(int i = 0; i < capacity; i++)
  {
    //If the words are non empty
    if(table[i].frequency > 0)
    {
      table[i].word = "";
      table[i].frequency = 0;
      table[i].hash_value = 0;
    }
  }
  table.resize(size); // Resize the vector to the new resize
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description: Default destructor for the HashTable class.
 *
 *****************************************************************************/
HashTable::~HashTable()
{}
