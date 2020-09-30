
#include <iostream>
#include <string>
#include <string.h>
#include <tr1/unordered_map>
#include <vector>
#include "WordList.h"
#include "NgramList.h"

using namespace std;

/*
 * NgramList
 *
 * takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and builds a linked list of 
 * ngrams.
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
NgramList::NgramList(int ngramSz, const WordList & wl)
{
   numOfNodes = 0;
   //initialize hashtable cells to NULL;
   memset(hashtable, 0, (HTSPACE * sizeof(Ngram_t *)));

   this->ngramSz = ngramSz;
   WordList::const_iterator p;
   p = wl.begin();
   while (p != wl.end())
   {
      std::string ngram = getNextNgram(p, wl.end());
      p++;
      if (!ngram.empty()) insertNgram(ngram);
   }
}

/*
 * NgramList destructor
 *
 * automatically called when NgramList object goes out of scope
 * deletes the hash table and the space allocated for arrayRep
 */
NgramList::~NgramList()
{
    //delete the hash table and its contents
    Ngram_t * nextNgram;
    Ngram_t * curr;
    for (int i = 0; i < HTSPACE; i++)
    {
        curr = hashtable[i];
        while (curr != NULL)
        {
            nextNgram = curr->next;
            delete curr;
            curr = nextNgram;
        }
    }

    //also delete the mem allocated to arrayRep
    free(arrayRep);
}

/*
 * getNextNgram
 *
 * takes as input an iterator that points to the word to start the
 * ngram and an iterator that points to the end of the word list
 * and builds and returns the ngram. returns empty string if no
 * ngram can be built, for example, if there are insufficient words
 * to build one.
 *
 * param: WordList::const_iterator start - points to first word in ngram
 * param: WordList::const_iterator end - points to end of word list
 * return: std::string - returns ngram or empty string
 */
std::string NgramList::getNextNgram(WordList::const_iterator start, 
                                    WordList::const_iterator end)
{
   int i, len;
   string ngram = "";
   for (i = 0; i < ngramSz && start != end; i++)
   {
      std::string word = (*start);
      //see if the string ends with punctuation
      //don't create ngrams that continue after punctuation
      if (!isalpha(word[word.length()-1]) && i < ngramSz - 1) return "";

      //take off all ending punctuation
      len = word.length() - 1;
      while (len >= 0 && !isalpha(word[len])) 
      {
         //remove last character
         word = word.substr(0, word.length() - 1);
         len--;
      }
      if (len < 0) return "";  //give up

      //is the first word in the ngram?
      if (ngram == "") ngram = word;
      else ngram = ngram + " " + word;

      start++;
   }

   //could we build a long enough ngram?
   if (i < ngramSz) return "";

   //take off beginning punctuation
   while (ngram.length() > 0 && !isalpha(ngram[0])) 
      ngram = ngram.substr(1, ngram.length());
   return ngram;
}

/*
 * hash
 *
 * generates the hash key
 *
 * param: std::string s - the string to be hashed
 * return: int - the generated key
 */
int NgramList::hash(std::string s)
{
    int key = 0;
    for (int i = 0; i < s.length(); i++)
    {
        key = (((key << 5) + key) ^ (int)s[i]) % HTSPACE;
    }
    return key;
}

/*
 * find
 * 
 * find a string in the hashtable
 * 
 * param: std::string s - the string to search for
 * param: Ngram_t ** found - holds + returns the node 
 *                           corresponding to the found s
 * return: int - indicates whether or not string was found:
 *               if found -> 0
 *               if not found -> -1
 */
int NgramList::find(std::string s, Ngram_t ** found)
{
    int hcode = hash(s);
    (*found) = hashtable[hcode];
    while ((*found) != NULL)
    {
        if (((*found)->ngram).compare(s) == 0)
        {
            return 0;
        }
        (*found) = (*found)->next;
    }
    return -1;
}

/*
 * insertNgram
 *
 * looks for the ngram to be inserted. If it is already in
 * the hash table,  it increments the count. If not, it 
 * inserts it into the hash table.
 *
 * param: std::strsng s - ngram to be inserted
 * return: none
 */
void NgramList::insertNgram(std::string s)
{    
    //if ngram is already present, inc count
    int hcode = hash(s);
    Ngram_t * found = NULL;
    
    if (find(s, &found) == -1)
    {
        Ngram_t * newNode = new Ngram_t();
        newNode->ngram = s;
        newNode->count = 1;
        
        //insert newNode at beginning of chain
        newNode->next = hashtable[hcode];
        hashtable[hcode] = newNode;
        numOfNodes++;
    }
    else
    {
        found->count = found->count + 1;
    }
}

/*
 *  comparator
 *
 *  necessary function used in qsort call
 *  type Ngram_t cannot be accessed from friend functions,
 *  so this function is essentially a wrapper for
 *  int NgramList::compare(const void * p, const void * q)
 *      (see param + return comments there)
 */
int comparator(const void * p, const void * q)
{
    return NgramList::compare(p, q);
}

/*
 * sortByCount
 *
 * performs a quicksort on an array representation of the
 * hashtable of ngrams, sorting by count
 *
 * param: none
 * return: none (modified private array)
 */
void NgramList::sortByCount()
{
    //transfer hash table elements to an array
    arrayRep = (Ngram_t **)malloc(numOfNodes * sizeof(Ngram_t));
    Ngram_t * curr;
    int count = 0;
    for (int i = 0; i < HTSPACE; i++)
    {
        curr = hashtable[i];
        while (curr != NULL)
        {
            arrayRep[count] = curr;
            curr = curr->next;
            count++;
        }
    }
    
    //use qsort function on array
    qsort(arrayRep, numOfNodes, sizeof(arrayRep[0]), comparator);   
}

/*
 * compare
 *
 * actual compare function to be used by qsort
 * 
 * param: const void * p - element left
 * param: const void * q - element right
 * return: int - indicating which element has larger count:
 *          left bigger? return < 0
 *          right bigger? return > 0
 *          same? return == 0
 */

int NgramList::compare(const void * p, const void * q)
{    
    Ngram_t ** l = (Ngram_t **)p;
    Ngram_t ** r = (Ngram_t **)q;
 
    return ((*r)->count - (*l)->count);
}


/*
 * operator<<
 *
 * prints the list of ngrams
 *
 * param: std::ostream & os - output stream to direct the output to
 * param: const NgramList & nglst - ngram list object
 * return: std::ostream & - output stream
 */  
std::ostream& operator<<(std::ostream& os, const NgramList & nglst)
{
    cout << "List of " << nglst.ngramSz << " word ngrams and counts\n";
    cout << "--------------------------------\n";
    NgramList::Ngram_t ** aRep = nglst.arrayRep;
    int numOfNodes = nglst.numOfNodes;
    for (int i = 0; i < numOfNodes; i++)
    {
        cout << (aRep[i])->ngram << ", " << (aRep[i])->count << endl;
    }
    return os;
}

