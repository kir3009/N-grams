#ifndef _BYTE_NGRAMS_H_
#define _BYTE_NGRAMS_H_

#include "ngrams.h"

/**
* this class implements all byte(unsigned char) ngram related operations
*/

class ByteNgrams : public Ngrams {

 public:

  ByteNgrams(int newNgramN,
             const char *newInFileName,
             const char *newOutFileName,
             const char *newDelimiters = "",
             const char *newStopChars = "");

  virtual ~ByteNgrams();

  void addTokens();

  /**
  * sort ngrams by frequency/ngram/or both, then output
  */

  void output();

 private:

  /**
  * Generate ngrams when queue has NGRAM_N - 1 tokens.
  * the token queue need to be processed specially for the first NGRAM_N - 1 tokens,
  * also need to be called if less than NGRAM_N tokens in the whole input text.
  * @param	count - total items in the queue
  */
  void preParse(int count);

  /**
  * Once the queue is full, it will start to pop out ngrams
  * for each new token pushing in
  */

  void parse();

  /**
  * get all ngrams for given N
  * @return total number of ngrams for the N
  */
  void getNgrams(vector<NgramToken *> &ngramVector, int n);

};
#endif
