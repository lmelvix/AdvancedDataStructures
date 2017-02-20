#include "util.h"
#include "DictionaryBST.h"

/* Create a new Dictionary that uses a BST back end */
DictionaryBST::DictionaryBST(){}

/* Insert a word into the dictionary. */
bool DictionaryBST::insert(std::string word)
{
  auto pair = bst.insert(word);
  return pair.second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryBST::find(std::string word) const
{
  return bst.find(word) != bst.end();
}

/* Destructor */
DictionaryBST::~DictionaryBST(){}
