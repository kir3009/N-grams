#include "WordNgrams.h"

WordNgrams::WordNgrams(int newNgramN,
                       const char *newInFileName,
                       const char *newOutFileName,
                       const char *newDelimiters,
                       const char *newStopChars) :
    Ngrams(newNgramN, newInFileName, newOutFileName, newDelimiters, newStopChars) {
  addTokens();

}

WordNgrams::~WordNgrams() {
}

void WordNgrams::addTokens() {
  // get token string from input file
  string &inFileName = getInFileName();
  FILE *fp = inFileName.length() > 0 ? fopen(inFileName.c_str(), "r") : stdin;

  if (fp == NULL) {
    printf("WordNgrams:addTokens - failed to open file %s\n", inFileName.c_str());
  } else {
    int count = 0;
    char c;
    bool isSpecialChar = false;
    string token;
    token.reserve(256);
    while ((c = (char) fgetc(fp)) != EOF) {
      if (isDelimiter(c) || isStopChar(c)) {
        if (!isSpecialChar && token.length() > 0) {
          this->addToken(token);
          token.empty();
          ++count;
          isSpecialChar = true;
        } else {
          isSpecialChar = false;
        }

      } else {
        token.append(c);
        isSpecialChar = false;
      }
    }
    if (token.length() > 0) {
      ++count;
      this->addToken(token);
    }
    // special processing need to be done, if less than NGRAM_N tokens in the whole input text.
    if (count < this->getN()) {
      preParse(count);
    }

    fclose(fp);
  }
}

void WordNgrams::addToken(const string &token) {
  char buff[32];

  this->encodeInteger(this->AddToWordTable(token.isNumber() ? "<NUMBER>" : token.c_str()), ENCODE_BASE, buff);

  this->Ngrams::addToken(buff);
}

void WordNgrams::preParse(int count) {
  TokenNode *p, *newHead;
  p = newHead = head;
  string ngram;
  ngram.reserve(256);
  while (newHead) {
    p = newHead;
    ngram.empty();
    for (unsigned short i = 0; i < count; i++) {
      ngram += p->token;
      //printf("%d ngram %s.\n", i, ngram.c_str() );
      this->addNgram(ngram.c_str(), i + 1);
      if (p == tail) {
        break;
      } else {
        ngram += ENCODE_WORD_DELIMITER;
      }
      p = p->next;

    }
    newHead = newHead->next;
  }
}

void WordNgrams::parse() {
  TokenNode *p, *newHead;
  p = newHead = head;
  string ngram;
  ngram.reserve(256);
  while (newHead) {
    unsigned short n = 0;
    p = newHead;
    ngram.empty();
    while (p) {
      ngram += p->token;
      ++n;
      p = p->next;
      if (p)
        ngram += ENCODE_WORD_DELIMITER;

    }
    //printf("%d ngram %s.\n", n, ngram.c_str() );
    if (n > 0) {
      this->addNgram(ngram.c_str(), n);
    }
    newHead = newHead->next;
  }
}

unsigned WordNgrams::AddToWordTable(const char *word) {
  unsigned id;
  unsigned *value = wordTable.getValue(word);
  if (value) {
    id = *value;
  } else {
    id = wordTable.count();
    wordTable.add(word, id);
  }
  return id;
}

void WordNgrams::output() {

  FILE *fp = getOutFileName().length() > 0 ? fopen(getOutFileName().c_str(), "r") : stdin;

  int ngramN = this->getN();

  fprintf(fp, "BEGIN OUTPUT\n");
  fprintf(fp, "Total %d unique ngram in %d ngrams.\n", this->count(), this->total());
  fprintf(stderr, "Total %d unique ngram in %d ngrams.\n", this->count(), this->total());

  for (int i = 1; i <= ngramN; i++) {
    // Get sorted item list
    Vector<NgramToken *> ngramVector;
    this->getNgrams(ngramVector, i);

    fprintf(fp, "\n%d-GRAMS\n", i);
    fprintf(fp, "Total %d unique ngrams in %d %d-grams.\n", this->count(i), this->total(i), i);
    fprintf(stderr, "Total %d unique ngrams in %d %d-grams.\n", this->count(i), this->total(i), i);
    fprintf(fp, "------------------------\n");

    size_t count = ngramVector.count();
    ngramVector.sort(INgrams::compareFunction);

    for (unsigned j = 0; j < count; j++) {
      NgramToken *ngramToken = ngramVector[j];
      //outputWordNgram( ngramToken.ngram, ngramToken.value.frequency, i );
      fprintf(fp, "%s\t%d\n", ngramToken->ngram.c_str(), ngramToken->value.frequency);
      delete ngramToken;
    }
  }

}

void WordNgrams::getNgrams(vector<NgramToken *> &ngramVector, int n) {

  // Get sorted item list
  Vector<TstItem<NgramValue> *> &itemVector = this->getItems();
  size_t count = itemVector.count();
  string decodedKey;
  decodedKey.reserve(256);
  for (unsigned i = 0; i < count; i++) {
    TstItem<NgramValue> *item = itemVector[i];
    if (item->value.n == n) {
      // decode the key to readable string
      decodedKey.empty();
      this->decodeWordNgram(item->key, n, decodedKey);
      ngramVector.add(new NgramToken(decodedKey, item->value));
      //ngramVector.add( NgramToken( decodedKey, item->value ) );
    }
  }
}

void WordNgrams::encodeInteger(int num, int bas, char *buff) {
  unsigned short index = 0;
  int rem;

  while (num >= bas) {
    rem = num % bas;
    //rem = rem == 0 ? ENCODE_NULL : rem;
    if (!rem) {
      rem = ENCODE_NULL;
    }
    num /= bas;
    sprintf(buff + index++, "%c", rem);
  }

  if (!num) {
    num = ENCODE_NULL;
  }
  sprintf(buff + index, "%c%c", num, '\0');
}

int WordNgrams::decodeInteger(unsigned char *buffer, int bas) {
  int num = 0, accumulate = 1;
  unsigned short index = 0;
  unsigned char c;
  while ((c = buffer[index]) != 0) {
    //printf("%d.", c );
    if (index++ > 0) {
      accumulate *= bas;
    }

    if (c == WordNgrams::ENCODE_WORD_DELIMITER) {
      break;
    }

    num += (c == ENCODE_NULL ? 0 : c) * accumulate;
  }
  //printf("\n");
  return num;
}

void WordNgrams::decodeWordNgram(const string &ngram, int n, string &decodedNgram) {
  //printf("outputWordNgram %s.\n", ngram.c_str() );
  int index = 0;
  int loop = 0;
  const unsigned char *p = (unsigned char *) ngram.c_str();
  unsigned char buff[32];

  while (loop++ < n) {
    while (*p != WordNgrams::ENCODE_WORD_DELIMITER && *p != '\0') {
      buff[index++] = (unsigned char) *p++;
    }

    ++p;

    buff[index] = '\0';
    index = 0;
    //printf("ID -- %d.\n", decodeInteger( buff, ENCODE_BASE ) );

    decodedNgram += this->wordTable.getKey(decodeInteger(buff, ENCODE_BASE));
    if (loop < n) {
      decodedNgram.append('_');
    }
  }
}
