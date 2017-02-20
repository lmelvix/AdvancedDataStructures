/** Natchuta Wattanapenpaiboon
 *  Oct 9, 2016
 *  Dictionary ADT implemented with Ternary Search Tree Class Implementation 
 *  CSE100 Fall 2016 PA2
 */

#include "util.h"
#include "DictionaryTrie.h"
#include <queue>

/* ======== Trie Node ======== */

/* Constructor */
TrieNode::TrieNode(char label) : 
  label(label), left(nullptr), mid(nullptr), right(nullptr), word(""), freq(0){}

/* Getters */
char TrieNode::getLabel() const 
{
  return label;
}

TrieNode* TrieNode::TrieNode::getLeft() const
{
  return left;
}

TrieNode* TrieNode::getMid() const 
{
  return mid;
}

TrieNode* TrieNode::getRight() const
{ 
  return right;
}

std::string TrieNode::getWord() const
{
  return word;
}

unsigned int TrieNode::getFreq() const
{
  return freq;
}
    
/* Setters */
void TrieNode::setLeft(char leftChar)
{
  if(this->left)
    delete this->left;
  this->left = new TrieNode(leftChar);
}

void TrieNode::setMid(char midChar)
{
  if(this->mid)
    delete this->mid;
  this->mid = new TrieNode(midChar);
}

void TrieNode::setRight(char rightChar)
{
  if(this->right)
    delete this->right;
  this->right = new TrieNode(rightChar);
}

void TrieNode::setWord(std::string word)
{
  this->word = word;
}

void TrieNode::setFreq(unsigned int frequency)
{
  this->freq = frequency;
}

void TrieNode::setWordFreq(std::string word, unsigned int frequency)
{
  this->setWord(word);
  this->setFreq(frequency);
}

/* ======== Compare Trie Node ======== */

bool CompareTrieNode::operator()(const TrieNode* n1, const TrieNode* n2)
{ 
  if (n1->getFreq() < n2->getFreq())
    return true;
  return false;
}

/* ======== Dictionary Trie ======== */

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie() : root(nullptr) {}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
  if(word == "")
    return false;

  if(!root) 
  {
    root = new TrieNode(word[0]);
    auto curr = root;
    for(unsigned int i = 1; i < word.length(); i++)
    {
      curr->setMid(word[i]);
      curr = curr->getMid();
    }
    curr->setWordFreq(word, freq);
    return true;
  } 

  auto curr = root;
  unsigned int i = 0;
  while(i < word.length())
  {
    if(curr->getLabel() < word[i])
    {
      if(curr->getRight() == nullptr)
      {
        curr->setRight(word[i]);
        curr = curr->getRight();
        i++;
        while(i < word.length())
        { 
          curr->setMid(word[i]);
          curr = curr->getMid();
          i++;
        }
        break;
      }
      curr = curr->getRight();
    }
    else if(word[i] < curr->getLabel())
    { 
      if(curr->getLeft() == nullptr)
      {
        curr->setLeft(word[i]);
        curr = curr->getLeft();
        i++;
        while(i < word.length())
        { 
          curr->setMid(word[i]);
          curr = curr->getMid();
          i++;
        }
        break;
      }
      curr = curr->getLeft();
    }
    else // curr->getLabel() == word[i]
    { 
      if(i == word.length()-1) 
        break; // To make curr = last node of word
      if(curr->getMid() == nullptr)
      {
         while(i < word.length()-1)
         { 
           curr->setMid(word[i+1]);
           curr = curr->getMid();
           i++;
         }
         break;
      }
      curr = curr->getMid();
      i++;
    }
  }  
  
  // Curr = last node of word
  if(curr->getWord() == "" && curr->getFreq() == 0)
  {
    curr->setWordFreq(word, freq);
    return true;
  }
  else if(curr->getWord() == word && curr->getFreq() < freq)
  {
    curr->setFreq(freq);
    return false;
  }
  else
  {
    return false;
  }
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
  if(!root)
    return false;

  auto curr = root;
  unsigned int i = 0;
  while(i < word.length())
  { 
    if(curr->getLabel() < word[i])
    { 
      if(curr->getRight() == nullptr)
        return false;
      
      curr = curr->getRight();
    }
    else if (word[i] < curr->getLabel())
    { 
      if(curr->getLeft() == nullptr)
        return false;
        
      curr = curr->getLeft();
    }
    else
    { 
      if(i == word.length()-1)
        break;
      if(curr->getMid() == nullptr)
        return false;
        
      curr = curr->getMid();
      i++;
    }
  }

  // Curr is the last node of word - check if word-node
  if(curr->getWord() == word)
    return true;
  return false;
}

/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions* of the prefix)
 * of the prefix)
 */
std::vector<std::string> 
DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
  std::vector<std::string> words;

  // Saves time from traversals
  if(num_completions == 0)
    return words;

  /* Handling invalid prefixes */
  // Empty string
  if(prefix == "")
  { 
    std::cout << "Invalid Input. Please retry with correct input" << std::endl;
    return words;
  }

  // Non-dictionary character
  for(unsigned int i = 0; i < prefix.length(); i++) 
  {
    if(prefix[i] == ' ')
      break;
    if((int)prefix[i] < (int)'a' || (int)'z' < (int)prefix[i])
    {
      std::cout << "Invalid Input. Please retry with correct input" << std::endl;
      return words;
    }
  }

  /* At this point, prefix is sure to be valid */
  // Find algorithm
  if(!root)
    return words;

  auto curr = root;
  unsigned int i = 0;
  while(i < prefix.length())
  {
    if(curr->getLabel() < prefix[i])
    {
      if(curr->getRight() == nullptr)
        return words; // No node with such prefix, return empty vector

      curr = curr->getRight();
    }
    else if (prefix[i] < curr->getLabel())
    {
      if(curr->getLeft() == nullptr)
        return words; // No node with such prefix, return empty vector

      curr = curr->getLeft();
    }
    else
    {
      if(i == prefix.length()-1)
        break;
      if(curr->getMid() == nullptr)
        return words; // No node with such prefix, return empty vector

      curr = curr->getMid();
      i++;
    }
  }
  
  // At this point, curr = last letter of prefix
  // Do a breadth first search, put word-nodes into priority queue
  std::priority_queue<TrieNode*, std::vector<TrieNode*>, CompareTrieNode> pq;
  
  // If prefix is also a word, add to pq
  if(curr->getWord() != "")
    pq.push(curr);  

  std::queue<TrieNode*> q;

  // Start search from curr's mid child. We don't care about curr's left or right child
  if(curr->getMid())
    q.push(curr->getMid());
  
  while(!q.empty())
  {
    auto n = q.front();
    if(n->getWord() != "") // If front node is a word-node
      pq.push(n); // Add it to the pq
    
    // Add children of front node to queue
    if(n->getLeft())
      q.push(n->getLeft());
    if(n->getMid())
      q.push(n->getMid());
    if(n->getRight())
      q.push(n->getRight());

    q.pop();
  }

  // At this point, all nodes under curr should be visited, and all word-nodes added to pq
  // Pop words out until pq is empty, or when num_completions is reached
  unsigned int count = 0;
  while(!pq.empty() && count < num_completions)
  {
    auto n = pq.top();
    words.push_back(n->getWord());
    count++;
    pq.pop();
  }

  return words;
}

/* Destructor */
DictionaryTrie::~DictionaryTrie()
{
  deleteAll(root);
}

void DictionaryTrie::deleteAll(TrieNode* n) 
{
  if (!n)
    return;

  deleteAll(n->getLeft());
  deleteAll(n->getMid());
  deleteAll(n->getRight());
  delete n;
}
