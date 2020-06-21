/**************************************************************************//**
* @file
*
* @brief contains all of Sharvil's functions for project 2
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "hash.h"

using namespace std;

string wordFilter(char *);
string charToString(char *);
void convertToLower(string &);
char *getCsvFilename(char *);
void rehash(HashTable &);
int digits(int n);

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * This function opens the input file. It checks that the number of command
 * line arguments are 2. If the file fails to open, the function returns false,
 * else true.
 *
 * @param[in, out]  fin - an ifstream to open thr input file
 * @param[in, out] filename - contains the name of the input file
 *
 * @returns true - File opened successfully
 * @returns false - Files failed to open
 *****************************************************************************/

bool openInputFile(ifstream &fin, char *filename)
{
    fin.open(filename, ios::in);

    if(!fin.is_open())
    {
      cout << "Failed to open input file" << endl;
			return false;
    }
    return true;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * This function opens a comma separated file. The function creates the .csv
 * extension by manipulating the input file name.
 *
 * @param[in, out] filename - contains the name of the input file
 * @param[in, out]  csv_out - an ofstream to open the csv file
 *
 * @returns true - File opened successfully
 * @returns false - Files failed to open
 *****************************************************************************/
bool openCsvFile(char *filename, ofstream &csv_out)
{
	char temp[100] = {};
	int length = strlen(filename);
	strncpy(temp, filename, length - 3);
	strncat(temp, "csv", 3);

	csv_out.open(temp, ios::out|ios::trunc);

	if(!csv_out.is_open())
	{
		cout << "Failed to open .csv output file" << endl;
		return false;
	}
	return true;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * This function opeens a word file. The function creates the .wrd extension
 *  by manipulating the input file name.
 *
 * @param[in, out] filename - contains the name of the input file
 * @param[in, out]  wrd_out - an ofstream to open the wrd file
 *
 * @returns true - File opened successfully
 * @returns false - Files failed to open
 *****************************************************************************/
bool openWrdFile(char *filename, ofstream &wrd_out)
{
	char temp[100] = {};

	int length = strlen(filename);
	strncpy(temp, filename, length - 3);
	strncat(temp, "wrd", 3);

	wrd_out.open(temp, ios::out | ios::trunc);

	if(!wrd_out.is_open())
	{
		cout << "Failed to open .wrd output file" << endl;
		return false;
	}
	return true;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * This function opeens a word file. The function creates the .wrd extension
 *  by manipulating the input file name.
 *
 * @param[in, out] fin - an ifstream object that is used to open the input file
 * @param[in, out]  H - the hash table
 *
 * @returns none
 *****************************************************************************/
void readData(ifstream &fin, HashTable &H)
{
	string input;
	const char *delimiters = "\"\\/, .{}[]();:|-_!@#%^&*$?<>+=~1234567890";
	char *ptr;

  //While there is data to read
	while(fin >> input)
	{
		convertToLower(input); //convert the string to lower case

    //Tokenize the input string by converting the string to char * and then
    //getting the first word
		ptr = strtok(const_cast<char *>(input.c_str()), delimiters);

		while(ptr != NULL)
		{
				if(H.getLoadFactor() >= 0.75)
				{
					cout << "WARNING: Hash table (size " << H.getCapacity() <<
           ") is 75% full!" << endl;
          H.sortTable();
					rehash(H);
					cout << "Rehashing to size " << H.getCapacity() << " ... "
						<< "finished" << endl;
				}
      //Filter the word off whitespaces and ' at the front and end of the word
				string key = wordFilter(ptr);

        //If the word is non empty
				if(key != "")
				{
          //Insert the key into the hash table
					H.insertKey(key);
          //Increment the total words by 1
					H.incrementTotalWords();
				}

				ptr = strtok(NULL, delimiters); //get the next word
		}
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Rehash the data from the hash table of orginal size to one with more than
 * double the size. The function stores all the non empty words and their
 * frequencies into two temporary vectors. It then cleans teh hash table and
 * resizes it to the new size. It then re-inserts all the words and their
 * frequencies from the temporary vectors back into the hash table. It then
 * clears the temporary vectors.
 *
 * @param[in, out] H - the hash table
 *
 * @returns none
 *****************************************************************************/
void rehash(HashTable &H)
{
  int i = 0;

  //Temporary vectors to store words and frequencies
  vector<string> temp_words;
  vector<int> temp_freq;

  //store the words and their frequencies
  while(H.getFrequency(i) > 0)
  {
      temp_words.push_back(H.getWord(i));
      temp_freq.push_back(H.getFrequency(i));
      i++;
  }

	static int n = 0;

  //Array with some prime values
	int primes[10] = {2003, 4007, 8017, 16057, 32117, 64237, 128477, 256957,
    513917, 1027841};

  H.cleanandresize(primes[n]);
  H.setCapacity(primes[n]); //Sets teh capacity to the new capacity
  n++;

  //Rehash the words back into the hash table
  for(unsigned int i = 0; i < temp_words.size(); i++)
  {
      H.rehashInsertKey(temp_words[i], temp_freq[i]);

  }

  //Clean the temporary vectors
  temp_words.clear();
  temp_freq.clear();
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Filters the word fromm whitespaces and ' chaarecters that are present at the
 * beginning and the end of the word
 *
 * @param[in, out] ptr - the word
 *
 * @returns temp - filtered word
 *****************************************************************************/
string wordFilter(char *ptr)
{
	string temp = charToString(ptr); //Convert the word to string

  //Remove ' charecters from the ends
	while(temp.length() > 0 && (temp[0] == ' ' || temp[0] == '\''))
		temp = temp.substr(1, temp.length() - 1);

  //Remove white spaces from the ends
	while(temp.length() > 0 && (temp[temp.length() - 1] == ' ' ||
   temp[temp.length() - 1] == '\''))
		temp = temp.substr(0, temp.length() - 1);

	return temp;
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Filters the word fromm whitespaces and ' chaarecters that are present at the
 * beginning and the end of the word
 *
 * @param[in] input - the word
 *
 * @returns none
 *****************************************************************************/
void convertToLower(string &input)
{
  //Convert each charecter to lowercase
	for(string::iterator it = input.begin(); it != input.end(); it++)
		*it = tolower(*it);
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Print the filename, total number of words and the number of distinct words.
 * After this, print the rank, frequency and the rank x frequency values in the
 * respective columns for each non empty word in the table
 *
 * @param[in] filename - name of the input file
 * @param[in, out] csv_out - ofstream to print the data to a csv file
 * @param[in, out] H - the hashtable
 *
 * @returns none
 *****************************************************************************/
void writeToCsv(char *filename, ofstream &csv_out, HashTable &H)
{
	int i = 0;
	double rank = 0;
	int num = 0;
	csv_out << endl;
	csv_out << "Zipf's Law"  << endl;
	csv_out << "----------" << endl;
	csv_out << "File: " << filename << endl;
	csv_out << "Total number of words = " << H.getTotalWords()  << endl;
	csv_out << "Number of distinct words = " << H.getCurrentSize() << endl;
	csv_out << endl;
	csv_out << "    rank    freq   rank*freq" << endl;
	csv_out << "    ----    ----   ---------" << endl;

	while(i < H.getCurrentSize())
	{
		rank = i + 1;
    //While the words have the same frequency
		while(H.getFrequency(i) == H.getFrequency(i + 1))
		{
			num++;
			i++;
		}

    //Calculate the rank
		if(num > 0)
			rank = (rank + (rank + num)) / 2.0;
		csv_out << fixed << showpoint << setprecision(1);
		csv_out << rank << "," << H.getFrequency(i) << "," << double(rank  * H.getFrequency(i)) << "\n";
		num = 0;
		i++;
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Print the filename, total number of words and the number of distinct words.
 * After this, print the words and their ranks and average rank with proper
 * formatting
 *
 * @param[in, out] filename - name of the input file
 * @param[in, out] wrd_out - ofstream to print the data to a wrd file
 * @param[in, out] H - the hashtable
 *
 * @returns none
 *****************************************************************************/
void writeToWrd(char *filename, ofstream &wrd_out, HashTable &H)
{
	int i = 0;
	wrd_out << endl;
	wrd_out << "Zipf's Law" << endl;
	wrd_out << "----------" << endl;
	wrd_out << "File: " << filename << endl;
	wrd_out << "Total number of words: " << H.getTotalWords() << endl;
	wrd_out << "Number of distinct words: " << H.getCurrentSize() << endl;
	wrd_out << endl;
	wrd_out << "Word Frequencies" << setw(45) << "Ranks" << setw(13)
   << "Avg Rank" << endl;
	wrd_out << "----------------" << setw(45) << "-----" << setw(13)
   << "--------" << endl;
	while(i < H.getCurrentSize())
	{
    int j = 0;
    int freq = H.getFrequency(i);
    int num = digits(freq); //Calculate the number of digits
    wrd_out << left << setw(15) << "Words occurring " << left <<
     setw(num) << freq << left << setw(30 - num) << " times:" << setw(15)
     << right << i+1 << right << setw(13) << i + 1 << endl;
    wrd_out << left << setw(15) << H.getWord(i);
    i++;
    while(i < H.getCurrentSize() && freq == H.getFrequency(i))
    {
        wrd_out << " " << left << setw(15) << H.getWord(i);
        if(j % 2 == 0)
        {
            wrd_out << endl;
        }
        i++;
        j++;
    }
    wrd_out << endl;
    j = 0;
	}
}

/** ***************************************************************************
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * Calculates the number of digits in the number
 *
 * @param[in]  n - the number
 *
 * @returns i - number of digits
 *****************************************************************************/
int digits(int n)
{
  int i = 0;
  while(n != 0)
  {
    n /= 10;
    i++;
  }
  return i;
}
