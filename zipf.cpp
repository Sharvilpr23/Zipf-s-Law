/**************************************************************************//**
 * @file
 *
 * @brief start of the application containing main and error checking function
 *****************************************************************************/

 /**************************************************************************//**
 *
 * @mainpage Progject 2 - Zipf's Law
 *
 * @section course_section Course Information
 *
 * @authors Sharvil Pai Raiker, Jean Cruz
 *
 * @date April 7, 2020
 *
 * @par Instructor:
 *         Prof. Paul Hinker
 *
 * @par Course:
 *         CSC 315 - 10:00 am
 *
 * @section program_section Program Information
 *
 * @details
 * This program takes in a a .txt file from the command line as input. The
 * words are read by tokenizing each word from the file which are then inserted
 * in a hashtable. If the load factor of the table goes to 75% or over, the
 * words are rehashed into a table of more than double the previous size. Once
 * all the data is read in, the table is sorted in descending order of their
 * frequencies. The words with the same frequencies are sorted alphabetically.
 * A csv file is then opened to which the rank of the word, frequency and
 * rank x frequency is printed to. A word document is opened too to which the
 * words along with their rank and average rank are printed to.
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      None
 *
 * @par Usage:
   @verbatim
   c:\> zipf.exe inputfile.txt

   @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *  Unable to locate the one extra word in shakespeare.txt
 *  Need to work on the formattinng of wrd output file
 *  Need to work on optimization of the code
 *
 * @par Modifications and Development Timeline:
 * <a href =
 * "https://gitlab.mcs.sdsmt.edu/7509329/csc315_spring2020_project2/-/commits/master/"
 * target = "_blank"> Click here for the gitlab commit log </a>
 *
 *****************************************************************************/

#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "hash.h"

using namespace std;

bool openInputFile(ifstream &, char *);
bool openCsvFile(char *, ofstream &);
bool openWrdFile(char *, ofstream &);
string charToString(char *);
void readData(ifstream &, HashTable &);
void writeToCsv(char *filename, ofstream &, HashTable &);
void writeToWrd(char *, ofstream &, HashTable &);

/**************************************************************************//**
 * @author Sharvil Pai Raiker
 *
 * @par Description:
 * This is the starting point to the program. It will read in the command line
 * arguments and ensure that they are correct. Then, it will open the input
 * file and read in the words. A hashtable is created and is used to hash in
 * the words read from the file. Once all the data is read in, the table is
 * sorted and the data is writeen to a csv and a wrd file. The program then
 * exits returning 0.
 *
 * @param[in] argc - the number of arguments from the command prompt
 * @param[in] argv - a 2d array of characters containing the arguments
 *
 * @returns 0      - program ran successfully, or exited with an error
 *****************************************************************************/
int main(int argc, char **argv)
{
    ifstream fin;
    ofstream csv_out;
    ofstream wrd_out;

    if(argc != 2)
    {
      cout << "Invalid number of command line arguments" << endl;
      return 1;
    }

    if(openInputFile(fin, argv[1]))
    {

      HashTable H(1001);

      auto starttime = chrono::system_clock::now();

      readData(fin, H);

      auto endtime = chrono::system_clock::now();
    	chrono::duration<double> diffset = endtime - starttime;

    	cout << fixed << showpoint << setprecision(8);
    	cout << "Read time: " << diffset.count() << endl;

      auto startTime = chrono::system_clock::now();

      H.sortTable();

      auto endTime = chrono::system_clock::now();
    	diffset = endTime - startTime;

    	cout << fixed << showpoint << setprecision(8);
      cout << "Sort time: " << diffset.count() << endl;

      H.sortTable();
      //H.printTable();

      if(openCsvFile(argv[1], csv_out))
      {
          writeToCsv(argv[1], csv_out, H);
      }

      if(openWrdFile(argv[1], wrd_out))
      {
     	  writeToWrd(argv[1], wrd_out, H);
      }
    }

    fin.close();
    csv_out.close();
    wrd_out.close();

    return 0;
}
