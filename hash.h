/**************************************************************************//**
* @file
*
* @brief header file containing hashtable class to be used with all project 2
*	       files
******************************************************************************/

#ifndef _HASH_H
#define _HASH_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*!
 * @brief Holds the word and its frequency and hash value
*/
struct HashNode
{
	string word; /*!< word from the text file */
	int frequency; /*!< frequency of the word */
	unsigned long hash_value; /*!< hash value of the word */
};

/*!
 * @brief Provides access to a vector of hashnodes, number of distinct
 *        words, capacity and total words read
 */
class HashTable
{
protected:
  vector<HashNode> table;  /*!< Vector of hashnodes */
  int current_size;  /*!< current size */
  int capacity;  /*!< capacity */
  int total_words;  /*!< total words */

public:
  HashTable();
  HashTable(int);

  string getWord(int);
  int getFrequency(int);
  void setFrequency(int);
  unsigned long getHashValue(int);

  int getCurrentSize();
  void setCurrentSize(int);

  int getCapacity();
  void setCapacity(int);

  int getTotalWords();
  void setTotalWords(int);
  void incrementTotalWords();

  double getLoadFactor();

  unsigned long hash(string &);
  unsigned long getIndex(unsigned long);

  void insertKey(string &);
  void rehashInsertKey(string &, int);

  bool find(string &);

  void sortTable();

  void printTable();

  void cleanandresize(int);

  ~HashTable();
};

#endif
