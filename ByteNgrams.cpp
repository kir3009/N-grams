#include "ByteNgrams.h"

ByteNgrams::ByteNgrams(int newNgramN,
                       const char *newInFileName,
                       const char *newOutFileName,
                       const char *newDelimiters,
                       const char *newStopChars) :
    Ngrams(newNgramN, newInFileName, newOutFileName, newDelimiters, newStopChars) {
  addTokens();
}

ByteNgrams::~ByteNgrams() {
}

void ByteNgrams::addTokens() {
  // get token string from input file
  string &inFileName = getInFileName();
  FILE *fp = inFileName.length() > 0 ? fopen(inFileName.c_str(), "rb") : stdin;

  if (fp == NULL) {
    printf("ByteNgrams:addTokens - failed to open file %s\n", inFileName.c_str());
  } else {

    int count = 0;
    char c[3];
    c[1] = 0;
    c[2] = 0;
    unsigned char buffer[1024 * 32];

    while (true) {
      size_t bytesRead = fread(buffer, 1, sizeof(buffer), fp);

      for (size_t i = 0; i < bytesRead; i++) {
        sprintf(c, "%02x", buffer[i]);
        addToken(c);
        count++;
      }

      if (bytesRead == 0) {
        break;
      }
    }
    // special processing need to be done, if less than NGRAM_N tokens in the whole input text.
    if (count < this->getN()) {
      preParse(count);
    }

    fclose(fp);
  }
}

void ByteNgrams::preParse(int count) {
  TokenNode *p, *newHead;
  p = newHead = head;
  string ngram;
  ngram.reserve(64);
  while (newHead) {
    p = newHead;
    ngram.empty();
    for (int i = 0; i < count; i++) {
      ngram += p->token;
      //printf("%d ngram %s.\n", i, ngram.c_str() );
      this->addNgram(ngram.c_str(), i + 1);
      if (p == tail) {
        break;
      }
      p = p->next;

    }
    newHead = newHead->next;
  }

}
void ByteNgrams::parse() {
  TokenNode *p, *newHead;
  p = newHead = head;
  string ngram;
  ngram.reserve(64);
  while (newHead) {
    int n = 0;
    p = newHead;
    ngram.empty();
    while (p) {
      ngram += p->token;
      n++;
      p = p->next;
    }
    //printf("%d ngram %s.\n", n, ngram.c_str() );
    if (n > 0) {
      this->addNgram(ngram.c_str(), n);
    }
    newHead = newHead->next;
  }
}

void ByteNgrams::output() {

  FILE *fp = getOutFileName().length() > 0 ? fopen(getOutFileName().c_str(), "r") : stdin;

  int ngramN = this->getN();

  fprintf(fp, "BEGIN OUTPUT BYTE NGRAMS\n");
  fprintf(fp, "Total %d unique ngrams in %d ngrams.\n", this->count(), this->total());
  fprintf(stderr, "Total %d unique ngrams in %d ngrams.\n", this->count(), this->total());

  for (int i = 1; i <= ngramN; i++) {
    // Get sorted item list
    Vector<NgramToken *> ngramVector;
    this->getNgrams(ngramVector, i);

    fprintf(fp, "\n%d-GRAMS ( Total %d unique ngrams in %d grams )\n", i, this->count(i), this->total(i));
    fprintf(stderr, "\n%d-GRAMS ( Total %d unique ngrams in %d grams )\n", i, this->count(i), this->total(i));
    fprintf(fp, "------------------------\n");

    size_t count = ngramVector.count();
    ngramVector.sort(INgrams::compareFunction);
    for (unsigned j = 0; j < count; j++) {
      NgramToken *ngramToken = ngramVector[j];

      fprintf(fp, "%s\t%d\n", ngramToken->ngram.c_str(), ngramToken->value.frequency);

      delete ngramToken;
    }
  }
}

void ByteNgrams::getNgrams(vector<NgramToken *> &ngramVector, int n) {
  // Get sorted item list
  //Vector<string> & keyVector = getKeys( );
  //Vector<NgramValue> & valueVector = getValues( );
  Vector<TstItem<NgramValue> *> &itemVector = getItems();
  size_t count = itemVector.count();
  for (unsigned i = 0; i < count; i++) {
    TstItem<NgramValue> *item = itemVector[i];
    if (item && item->value.n == n) {
      ngramVector.add(new NgramToken(item->key, item->value));
      //ngramVector.add( NgramToken( item->key, item->value ) );
    }
  }
}
