#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <cstdlib>
#include "util.h"
#include "DictionaryTrie.h"
#include "DictionaryBST.h"
#include "DictionaryHashtable.h"
#include "fstream"
#define LETTERS 26
using namespace std;

int main(int argc, char** argv)
{
  if(!(argv[1] && argv[2] && argv[3] && argv[4]))
  {
    cout << "Invalid arguments. Please try again." << endl;
    return -1;
  }
  unsigned int min_size = stoi(argv[1]);
  unsigned int step_size = stoi(argv[2]);
  unsigned int num_iterations = stoi(argv[3]);
  string dictfile = argv[4];
  unsigned int dictsize;
  unsigned int num_find_iterations = 20; // #times to repeat the finding process
 
  ifstream inFile(dictfile); 
  auto num_lines = 
    count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');
     
  cout << "DictionaryTrie" << endl; 
  for(unsigned int i = 0; i < num_iterations; i++)
  {
    // New dictionary object and reset the istream
    DictionaryTrie* Dt = new DictionaryTrie();
    Timer timer;
    ifstream in;
    dictsize = min_size + i*step_size;

    // Print warning message if dictsize too large 
    if(dictsize > num_lines)
      cout << "Warning: Not enough words in the file." << endl;
    else if((dictsize + 100) > num_lines)
      cout << "Warning: Not enough words to test for 100 unsuccessful finds." << endl;

    // Add words into our data structure
    in.open(dictfile, std::ios::binary);
    Utils::load_dict(*Dt, in, dictsize);

    // Read the next 100 words and put into vector
    string data;
    vector<string> test_words;
    for(unsigned int j = 0; j < 100; j++)
    {
      getline(in, data);
      test_words.push_back(data);
    }

    in.close();
    
    // Loop to compute the time to find those 100 words in the dictionary object
    unsigned int avg_time = 0;
    for(unsigned int k = 0; k < num_find_iterations + 1; k++)
    {
      timer.begin_timer();
      for(auto item : test_words)
        Dt->find(item);
      auto end_time = timer.end_timer();
      
      if(k != 0) // Ignore the first run
        avg_time += end_time;
    }

    delete Dt;
    avg_time /= num_find_iterations; // Ignore the first run
    cout << dictsize << "\t" << avg_time << endl;
  }

  cout << "DictionaryBST" << endl;
  for(unsigned int i = 0; i < num_iterations; i++)
  {
    // New dictionary object and reset the istream
    DictionaryBST* Dt = new DictionaryBST();
    Timer timer;
    ifstream in;
    dictsize = min_size + i*step_size;

    // Print warning message if dictsize too large 
    if(dictsize > num_lines)
      cout << "Warning: Not enough words in the file." << endl;
    else if((dictsize + 100) > num_lines)
      cout << "Warning: Not enough words to test for 100 unsuccessful finds." << endl;
    
    // Add words into our data structure
    in.open(dictfile, std::ios::binary);
    Utils::load_dict(*Dt, in, dictsize);

    // Read the next 100 words and put into vector
    string data;
    vector<string> test_words;
    for(unsigned int j = 0; j < 100; j++)
    {
      getline(in, data);
      test_words.push_back(data);
    }

    in.close();

    // Loop to compute the time to find those 100 words in the dictionary object
    unsigned int avg_time = 0;
    for(unsigned int k = 0; k < num_find_iterations + 1; k++)
    {
      timer.begin_timer();
      for(auto item : test_words)
        Dt->find(item);
      auto end_time = timer.end_timer();

      if(k != 0) // Ignore the first run
        avg_time += end_time;
    }

    delete Dt;
    avg_time /= num_find_iterations; // Ignore the first run
    cout << dictsize << "\t" << avg_time << endl;
  }

  cout << "DictionaryHashtable" << endl;
  for(unsigned int i = 0; i < num_iterations; i++)
  {
    // New dictionary object and reset the istream
    DictionaryHashtable* Dt = new DictionaryHashtable();
    Timer timer;
    ifstream in;
    dictsize = min_size + i*step_size;

    // Print warning message if dictsize too large 
    if(dictsize > num_lines)
      cout << "Warning: Not enough words in the file." << endl;
    else if((dictsize + 100) > num_lines)
      cout << "Warning: Not enough words to test for 100 unsuccessful finds." << endl;
    
    // Add words into our data structure
    in.open(dictfile, std::ios::binary);
    Utils::load_dict(*Dt, in, dictsize);
    
    // Read the next 100 words and put into vector
    string data;
    vector<string> test_words;
    for(unsigned int j = 0; j < 100; j++)
    {
      getline(in, data);
      test_words.push_back(data);
    }

    in.close();

    // Loop to compute the time to find those 100 words in the dictionary object
    unsigned int avg_time = 0;
    for(unsigned int k = 0; k < num_find_iterations + 1; k++)
    {
      timer.begin_timer();
      for(auto item : test_words)
        Dt->find(item);
      auto end_time = timer.end_timer();

      if(k != 0) // Ignore the first run
        avg_time += end_time;
    }

    delete Dt;
    avg_time /= num_find_iterations; // Ignore the first run
    cout << dictsize << "\t" << avg_time << endl;
  }
}
