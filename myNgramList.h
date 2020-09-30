#define HTSPACE 10000

class myNgramList 
{
   struct Ngram
   {
      std::string ngram;
      int count;
      struct Ngram * next;
   };

   typedef struct Ngram Ngram_t;

   private:
      int ngramSz;
      int numOfNodes;
      Ngram_t ** arrayRep;
      Ngram_t * hashtable[HTSPACE];
      int hash(std::string s);
      int find(std::string s, Ngram_t ** found);
      void insertNgram(std::string s);
      std::string getNextNgram(WordList::const_iterator start, 
                               WordList::const_iterator end);
      void swap(int index1, int index2);
      int partition(int left, int right, Ngram_t * pivot);
      void quicksort(int left, int right);

   public:
      myNgramList(int ngramSz, const WordList & wl);
      ~myNgramList();
      void sortByCount();
      int compare(Ngram_t * p, Ngram_t * q);
      friend std::ostream& operator<<(std::ostream& os, const myNgramList & nl);
};

