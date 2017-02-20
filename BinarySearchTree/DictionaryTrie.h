#ifndef DICTIONARY_TRIE_H
#define DICTIONARY_TRIE_H

#include <vector>
#include <string>

/* ======== Trie Node ======== */

/** Node class to be used with DictionaryTrie */

class TrieNode
{
  public:
    /* Constructor */
    TrieNode(char label);
    
    /* Getters */
    char getLabel() const;
    TrieNode* getLeft() const;
    TrieNode* getMid() const;
    TrieNode* getRight() const;
    std::string getWord() const;
    unsigned int getFreq() const;

    /* Setters */
    void setLeft(char leftChar);
    void setMid(char midChar);
    void setRight(char rightChar);
    void setWord(std::string word);
    void setFreq(unsigned int frequency);
    void setWordFreq(std::string word, unsigned int frequency);

  private:
    char const label;
    TrieNode* left;
    TrieNode* mid;
    TrieNode* right;
    std::string word;
    unsigned int freq;
};

/* ======== Compare Trie Node ======== */

/** Comparator class to be used with priority queue of DictionaryTrie's predictCompletion function */

class CompareTrieNode
{
  public:
    bool operator()(const TrieNode* n1, const TrieNode* n2);
};

/* ======== Dictionary Trie ======== */

/** The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 */
class DictionaryTrie
{
  public:

    /* Create a new Dictionary that uses a Trie back end */
    DictionaryTrie();

    /* Insert a word with its frequency into the dictionary.
     * Return true if the word was inserted, and false if it
     * was not (i.e. it was already in the dictionary or it was
     * invalid (empty string) */
    bool insert(std::string word, unsigned int freq);

    /* Return true if word is in the dictionary, and false otherwise */
    bool find(std::string word) const;

    /* Return up to num_completions of the most frequent completions
     * of the prefix, such that the completions are words in the dictionary.
     * These completions should be listed from most frequent to least.
     * If there are fewer than num_completions legal completions, this
     * function returns a vector with as many completions as possible.
     * If no completions exist, then the function returns a vector of size 0.
     * The prefix itself might be included in the returned words if the prefix
     * is a word (and is among the num_completions most frequent completions
     * of the prefix)
     */
    std::vector<std::string>
    predictCompletions(std::string prefix, unsigned int num_completions);

    /* Destructor */
    ~DictionaryTrie();

  private:
    // Add your own data members and methods here
    TrieNode* root;
    static void deleteAll(TrieNode* n);
};

#endif // DICTIONARY_TRIE_H
