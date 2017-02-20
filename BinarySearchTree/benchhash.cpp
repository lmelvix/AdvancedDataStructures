#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "fstream"
using namespace std;

/* Templates for Hash Functions (Citations below) */
unsigned int hash_func1(string key, unsigned int M);
unsigned int hash_func2(string key, unsigned int M);

/* // The commented parts of this code were used to test for hash functions correctness 
   unsigned djb_hash(unsigned char *key, int len);
   unsigned oat_hash(unsigned char *key, int len);
   */

int main(int argc, char** argv)
{
  string dictfile = argv[1];
  unsigned int num_words = stoi(argv[2]);
  unsigned int M = 2 * num_words;

  ifstream inFile(dictfile);
  auto num_lines = 
    count(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>(), '\n');

  if(num_lines < num_words)
  {
    cout << "Not enough lines in file." << endl;
    return -1;
  }

  ifstream in;
  in.open(dictfile, std::ios::binary);
  string data;
  vector<string> keys;
  for(unsigned int i = 0; i < num_words; i++)
  {
    getline(in, data);
    keys.push_back(data);
  }

  in.close();

  // Create array of length M with all 0's. These will stimulate hashtables.
  // They store #hits for each hashtable slot
  vector<unsigned int> hashtable1(M, 0); 
  vector<unsigned int> hashtable2(M, 0);

  for(auto key : keys)
  {
    // Hash each key
    auto hash_val1 = hash_func1(key, M);
    auto hash_val2 = hash_func2(key, M);

    // 'Put' the key into the hashtables
    hashtable1[hash_val1]++;
    hashtable2[hash_val2]++;
  }

  auto max_hit1 = *max_element(hashtable1.begin(), hashtable1.end());
  auto max_hit2 = *max_element(hashtable2.begin(), hashtable2.end());

  // Create new array to store #slots receiving each #hits
  vector<unsigned int> count_hits1(max_hit1 + 1, 0);
  vector<unsigned int> count_hits2(max_hit2 + 1, 0);

  // Linear scan of the two hashtables
  for(unsigned int i = 0; i < M; i++)
  {
    // Get #hits
    unsigned int num_hit1 = hashtable1[i];
    unsigned int num_hit2 = hashtable2[i];

    // Count slots with that #hits
    count_hits1[num_hit1]++;
    count_hits2[num_hit2]++;
  }

  // Calculate average #steps
  float avg1 = 0;
  for(unsigned int i = 0; i < max_hit1 + 1; i++)
  {
    unsigned int sum = 0;
    for(unsigned int j = i; j < max_hit1 + 1; j++)
      sum += count_hits1[j];
    avg1 += i*sum;
  }
  avg1 /= num_words;

  float avg2 = 0;
  for(unsigned int i = 0; i < max_hit2 + 1; i++)
  { 
    unsigned int sum = 0;
    for(unsigned int j = i; j < max_hit2 + 1; j++)
      sum += count_hits2[j];
    avg2 += i*sum;
  }
  avg2 /= num_words;

  // Print the output
  cout << "Bernstein Hash" << endl;
  cout << "Printing statistics for hash_func1 with hashtable size " << M << endl; 
  cout << "#hits" << '\t' << "#slots receiving that #hits" << endl;
  for(unsigned int i = 0; i < max_hit1 + 1; i++)
  {
    if(count_hits1[i] != 0)
      cout << i << '\t' << count_hits1[i] << endl;
  }
  cout << "The average #steps for a successful search for hash_func1 is "
    << avg1 << endl;
  cout << "The worst case #steps to find a word is " 
    << max_hit1 << endl; 

  cout << endl;

  cout << "One-at-a-Time Hash" << endl;
  cout << "Printing statistics for hash_func2 with hashtable size " << M << endl; 
  cout << "#hits" << '\t' << "#slots receiving that #hits" << endl;
  for(unsigned int i = 0; i < max_hit2 + 1; i++)
  {
    if(count_hits2[i] != 0)
      cout << i << '\t' << count_hits2[i] << endl;
  }
  cout << "The average #steps for a successful search for hash_func2 is "
    << avg2 << endl;
  cout << "The worst case #steps to find a word is " 
    << max_hit2 << endl; 

  /* // Code used for testing hash function correctness
     unsigned char str1[] = "ueai";
     unsigned char* p1 = str1;
     int len1 = 4;

     unsigned char str2[] = "test with spaces";
     unsigned char* p2 = str2;
     int len2 = 16;

     unsigned char str3[] = "ueaii";
     unsigned char* p3 = str3;
     int len3 = 5;

     cout << "Test for str1 = ueai" << endl;
     cout << "djb " << '\t' << '\t' << djb_hash(p1, len1) << endl;
     cout << "hash_func1 " << '\t' << hash_func1("ueai", 1) << endl;

     cout << "oat " << '\t' << '\t' << oat_hash(p1, len1) << endl;
     cout << "hash_func2 " << '\t' << hash_func2("ueai", 1) << endl;

     cout << endl;

     cout << "Test for str2 = test with spaces" << endl;
     cout << "djb " << '\t' << '\t' << djb_hash(p2, len2) << endl;
     cout << "hash_func1 " << '\t' << hash_func1("test with spaces", 1) << endl;

     cout << "oat " << '\t' << '\t' << oat_hash(p2, len2) << endl;
     cout << "hash_func2 " << '\t' << hash_func2("test with spaces", 1) << endl;

     cout << endl;

     cout << "Test for str3 = ueaii" << endl;
     cout << "djb " << '\t' << '\t' << djb_hash(p3, len3) << endl;
     cout << "hash_func1 " << '\t' << hash_func1("ueaii", 1) << endl;

     cout << "oat " << '\t' << '\t' << oat_hash(p3, len3) << endl;
     cout << "hash_func2 " << '\t' << hash_func2("ueaii", 1) << endl;
     */
  return 1; 
}

/** Bernstein Hash AKA djb2
 *  Implementations obtained from 
 *  http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx 
 *  Modified h = 5381 instead of h = 0. 
 *  Because other websites refer to this hashing function with 5381 and 33.
 *  http://www.cse.yorku.ca/~oz/hash.html
 *  http://stackoverflow.com/questions/10696223/reason-for-5381-number-in-djb-hash-function/13809282#13809282
 */
unsigned int hash_func1(string key, unsigned int M)
{
  unsigned int h = 5381;

  for (unsigned int i = 0; i < key.length(); i++)
  {
    h = 33 * h + key[i];
  }

  // return h;
  return h%M;
}

/** One-at-a-Time Hash
 *  Implementations obtained from 
 *  http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx 
 */
unsigned int hash_func2(string key, unsigned int M)
{
  unsigned int h = 0;

  for (unsigned int i = 0; i < key.length(); i++)
  {
    h += key[i];
    h += (h << 10);
    h ^= (h >> 6);
  }

  h += (h << 3);
  h ^= (h >> 11);
  h += (h << 15);

  // return h;
  return h%M;
}

/* // Code used for testing hash function correctness
   unsigned djb_hash(unsigned char *key, int len)
   { 
   unsigned char *p = key;
   unsigned h = 5381;
   int i;

   for (i = 0; i < len; i++)
   { 
   h = 33 * h + p[i];
   }

   return h;
   }

   unsigned oat_hash(unsigned char *key, int len)
   { 
   unsigned char *p = key;
   unsigned h = 0;
   int i;

   for (i = 0; i < len; i++)
   { 
   h += p[i]; 
   h += (h << 10);
   h ^= (h >> 6);
   }

   h += (h << 3);
   h ^= (h >> 11);
   h += (h << 15);

   return h;
   }
   */
