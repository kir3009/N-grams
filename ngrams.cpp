#include "ngrams.h"

Ngrams::Ngrams(int newNgramN,
               const char *newInFileName,
               const char *newOutFileName,
               const char *newDelimiters,
               const char *newStopChars) :
    ngramN(newNgramN), inFileName(newInFileName), outFileName(newOutFileName) {
  // initial queue
  head = tail = 0;
  tokenCount = 0;
  this->setDelimiters(newDelimiters);
  this->setStopChars(newStopChars);
  totals = new int[ngramN];
  uniques = new int[ngramN];
  memset(totals, 0, ngramN * sizeof(int));
  memset(uniques, 0, ngramN * sizeof(int));
}

void Ngrams::addToken(const string &token) {
  int count = this->pushQueue(token.c_str());

  if (count == this->ngramN) {
    this->parse();
    this->popQueue();
  } else if (count == this->ngramN - 1) {
    this->preParse(count);
  }

}

void Ngrams::addNgram(const char *ngram, int n) {
  assert(n > 0 && n <= ngramN);
  NgramValue *value = ngramTable.getValue(ngram);

  if (value) // existing ngram, increase frequent count by 1
  {
    ++value->frequency;
  } else // new ngram, add it
  {
    ngramTable.add(ngram, NgramValue(n, 1));
    ++uniques[n - 1];
  }
  ++totals[n - 1];
}

int Ngrams::pushQueue(const char *token) {
  TokenNode *tokenNode = new TokenNode(token);
  if (!head) {
    head = tokenNode;
  } else {
    this->tail->next = tokenNode;
  }
  this->tail = tokenNode;
  return ++tokenCount;
}

void Ngrams::popQueue() {
  if (head) {
    if (head == tail) {
      tail = 0;
    }
    TokenNode *p = head->next;
    delete head;
    head = p;
    --tokenCount;
  }
}
