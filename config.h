
/**
* define constants that used by the package
*/

#ifndef NGRAM_CONFIG_H
#define NGRAM_CONFIG_H

#include "mystring.h"

class Config
{
public:
	enum 
	{ 
		// word ngram
		WORD_NGRAM,
		// char ngram
		CHAR_NGRAM,
		// byte ngram
		BYTE_NGRAM
	};

	enum 
	{
		DEFAULT_NGRAM_TYPE = WORD_NGRAM		// default ngram type
	};

	enum
	{
		DEFAULT_NGRAM_N = 3
	};


	/**
	* get the default delimiters
	*/

	static const char * getDefaultDelimiters()
	{
		return " \t,.?;<>'\"`~!+-*/@#$%^&(){}[]|=\\:";
	}

	/**
	* get the default delimiters
	*/

	static const char * getDefaultStopChars()
	{
		return " \n\r";
	}


	/**
	* get the value of an argument in a command line
	*
	* @param	option - name of the argument
	* @param	argc - total number of argument
	* @param	argv - argument list
	* @return	value of the option. If not found, return ""
	*/

	static string getOptionValue ( string option, int argc, char * argv[] )
	{
		string value = "";
		string commandLine = "";
		for ( int i = 0; i < argc; i++ )
		{
			commandLine += " ";
			commandLine += string( argv[i] );
		}
		int start = commandLine.toLower().indexOf ( option.toLower() );
		if ( start != -1 )
		{
			value = commandLine.substring( start + option.length() );
			int end = value.indexOf( " -" );
			if ( end != -1 )
			{
				value = value.substring( 0, end + 1 );

			}
		}
		//printf("option string %s.\n", value.c_str() );
		return value.trim().trimStart( "=" ).trim();
	}

	/**
	* check whether command line contain an option
	*
	* @param	option - name of the argument
	* @param	argc - total number of argument
	* @param	argv - argument list
	* @return	true if contain the option, otherwise false
	*/

	static bool hasOption ( string option, int argc, char * argv[] )
	{
		string s;
		for ( int i = 0; i < argc; i++ )
		{
			s.append( argv[ i ] );
		}
		return s.toLower().indexOf( option.toLower() ) != -1;
	}


};

#endif
