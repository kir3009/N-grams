#ifndef _INGRAMS_H_
#define _INGRAMS_H_
#include "mystring.h"

/**
* Define the interface for ngram classes
*/

class INgrams {
 public:
  struct NgramValue {
    int n; // N of ngram
    int frequency;
    NgramValue() : n(0), frequency(0) {
    }
    NgramValue(int newN, int newFrequency) : n(newN), frequency(newFrequency) {
    }
  };

  struct NgramToken {
    NgramToken() {
    }
    NgramToken(string &newNgram, NgramValue &newValue) {
      ngram = newNgram;
      value = newValue;
    }
    // Copy constructor
    NgramToken(const NgramToken &copy) {
      ngram = copy.ngram;
      value = copy.value;
    }
    string ngram;
    NgramValue value;

    void operator=(const NgramToken &ngramToken) {
      ngram = ngramToken.ngram;
      value = ngramToken.value;
    }
    /**
    * use following operators if we need to order the ngrams by frequency
    */
    bool operator>(const NgramToken &ngramToken) const {
      return value.frequency > ngramToken.value.frequency;
    }
    bool operator>=(const NgramToken &ngramToken) const {
      return value.frequency >= ngramToken.value.frequency;
    }
    bool operator==(const NgramToken &ngramToken) const {
      return value.frequency == ngramToken.value.frequency;
    }
    bool operator<(const NgramToken &ngramToken) const {
      return value.frequency < ngramToken.value.frequency;
    }
    bool operator<=(const NgramToken &ngramToken) const {
      return value.frequency <= ngramToken.value.frequency;
    }
    /**
    * use following operators if we need to order the ngrams by ngram
    */
    /*
    bool operator>( const NgramToken & ngramToken ) const
    {
    return ngram < ngramToken.ngram;
    }
    bool operator>=( const NgramToken & ngramToken ) const
    {
    return ngram >= ngramToken.ngram;
    }
    bool operator==( const NgramToken & ngramToken ) const
    {
    return ngram == ngramToken.ngram;
    }
    bool operator<( const NgramToken & ngramToken ) const
    {
    return ngram < ngramToken.ngram;
    }
    bool operator<=( const NgramToken & ngramToken ) const
    {
    return ngram <= ngramToken.ngram;
    }
    */

    /**
    * use following operators if we need to order the ngrams by frequency and ngram
    */
    /*
    bool operator>( const NgramToken & ngramToken ) const
    {
    return value.frequency > ngramToken.value.frequency || value.frequency == ngramToken.value.frequency && ngram < ngramToken.ngram ;
    }
    bool operator>=( const NgramToken & ngramToken ) const
    {
    return value.frequency >= ngramToken.value.frequency || value.frequency == ngramToken.value.frequency && ngram <= ngramToken.ngram;
    }
    bool operator==( const NgramToken & ngramToken ) const
    {
    return value.frequency == ngramToken.value.frequency && ngram == ngramToken.ngram;
    }
    bool operator<( const NgramToken & ngramToken ) const
    {
    return value.frequency < ngramToken.value.frequency || value.frequency == ngramToken.value.frequency && ngram > ngramToken.ngram;
    }
    bool operator<=( const NgramToken & ngramToken ) const
    {
    return value.frequency <= ngramToken.value.frequency || value.frequency == ngramToken.value.frequency && ngram > ngramToken.ngram;
    }
    */
  };

  /**
   * compare function to compare two NgramToken pointers
   * - negative, if item 1 at address itemAddr1 less than item 2 at address itemAddr2.
   * - zero, equal
   * - positive, if item 1 > item 2nction
   */
#if 0
  static int compareFunction (const void * a, const void * b)		/* sort by alphabet */
  {
      /*printf("comparing %s - %s.\n", ((Element*)a)->key, ((Element*)b)->key ); */
      return strcmp( ( ( *( NgramToken** )a ) )->ngram.c_str(), ( *( ( NgramToken** )b ) )->ngram.c_str() );
      /*String * x = *( ( String ** ) a );
      String * y = *( ( String ** ) b );
      return *x > *y ? 1 : *x == *y ? 0 : -1;
      */
  }
#endif
  static int compareFunction(const void *a, const void *b)        /* sort by frequency and word */
  {
    int freq1 = (*((NgramToken **) a))->value.frequency;
    int freq2 = (*((NgramToken **) b))->value.frequency;
    return freq1 > freq2 ? -1 : freq1 == freq2 ? strcmp(((*(NgramToken **) a))->ngram.c_str(),
                                                        (*((NgramToken **) b))->ngram.c_str()) : 1;

  }
  /**
  * constructor
  */
  INgrams();

  /**
  * destructor
  */
  virtual ~INgrams();

  /**
  * feed a token in, the token will be processed internally to generating ngram
  *
  * for word ngram, the token will be word string,
  * for character ngram, the token will be a string of the character.
  */

  virtual void addToken(const string &token) = 0;

  /**
  * sort ngrams by frequency/ngram/or both, then output
  */
  virtual void output() = 0;

  virtual void setDelimiters(const char *newDelimiters) = 0;

  /**
  * get total number of ngrams
  */
  virtual int count() = 0;

  /**
  * get total number ngrams for given N
  */
  virtual int count(int n) = 0;

};

#endif
