#ifndef text2wfreq_h
#define text2wfreq_h

#include "WordNgrams.h"
#include "CharNgrams.h"
#include "ByteNgrams.h"

/**
* This class utilizes WordNgrams and CharNgrams classes to generate and output ngrams.
*/

class Text2wfreq
{
public:

	/**
	* Constructor
	*/

	Text2wfreq()
	{
		ngramN = Config::DEFAULT_NGRAM_N;
		ngramType = Config::DEFAULT_NGRAM_TYPE;
		inFileName = "";
		outFileName = "";
	}

	~Text2wfreq()
	{
	}


	/**
	* get options
	*/
	bool getOptions( int argc, char * argv[] );

	/**
	* display help information 
	*/
	void showHelp();

	void printOptions ()
	{
		printf ( "ngramN %d, ngramType %s, inFileName %s, outFileName %s.\n", ngramN, ngramType == Config::CHAR_NGRAM?"character":"word", inFileName.c_str(), outFileName.c_str() );
	}


	int getNgramN() 
	{
		return ngramN;
	}

	int getNgramType()
	{
		return ngramType;
	}

	string getInFileName()
	{
		return inFileName;
	}

	string getOutFileName()
	{
		return outFileName;
	}

private:
	int ngramN;	// default number of ngrams
	int ngramType;	// default type
	string inFileName;	// input text file name
	string outFileName;	// output text file name

};

#endif
