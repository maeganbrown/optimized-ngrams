#define HTSPACE 10000

class NgramList 
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

   public:
      NgramList(int ngramSz, const WordList & wl);
      ~NgramList();
      void sortByCount();
      friend int comparator(const void * p, const void * q);
      static int compare(const void * p, const void * q);
      friend std::ostream& operator<<(std::ostream& os, const NgramList & nl);
};

