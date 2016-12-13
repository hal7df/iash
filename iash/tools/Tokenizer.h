/*
 * Tokenizer.h
 *
 *  Created on: Nov 22, 2016
 *      Author: paul
 */

#ifndef IASH_TOOLS_TOKENIZER_H_
#define IASH_TOOLS_TOKENIZER_H_

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include "Token.h"

/**
 * Splits up a source string into Tokens based on the given delimiter(s). This
 * is used under the hood by iash to parse the user's command input into
 * UserCommands to be used by iash and Commands.
 *
 * @since 0.5
 */
class Tokenizer {
public:
	/**
	 * Creates a Tokenizer with no initial settings.
	 */
	Tokenizer();

	/**
	 * Creates a Tokenizer and tokenizes the given raw string over spaces.
	 * This does not tokenize over tabs, newlines, and carriage returns, use
	 * Tokenizer#Tokenizer(const std::string&, const std::string&) instead.
	 *
	 * @param raw	the string to tokenize
	 */
	Tokenizer(const std::string &raw);

	/**
	 * Creates a Tokenizer and tokenizes the given raw string over the given
	 * delimiter.
	 *
	 * @param raw	the string to tokenize
	 * @param delim	the delimiter to tokenize over
	 */
	Tokenizer(const std::string &raw, const char delim);

	/**
	 * Creates a Tokenizer and tokenizes the given raw string over the given
	 * delimiters.
	 *
	 * @param raw		the string to tokenize
	 * @param delims	the delimiters to tokenize over as a string. Each
	 * 					character in the string is interpreted as a delimiter
	 */
	Tokenizer(const std::string &raw, const std::string &delims);

	/**
	 * Creates a Tokenizer and tokenizes the contents of the given token over
	 * spaces. This does not tokenize over tabs, newlines, and carriage returns,
	 * use Tokenizer#Tokenizer(const Token&, const std::string&) instead.
	 *
	 * @param  raw	the Token to tokenize further
	 */
	Tokenizer(const Token &raw);

	/**
	 * Creates a Tokenizer and tokenizes the contents of the given token over
	 * the given delimiter.
	 *
	 * @param raw	the Token to tokenize further
	 * @param delim	the delimiter to tokenize over
	 */
	Tokenizer(const Token &raw, const char delim);

	/**
	 * Creates a Tokenizer and tokenizes the contents of the given token over
	 * the given delimiters.
	 *
	 * @param raw		the Token to tokenize further
	 * @param delims	the delimiters to tokenize over as a string. Each
	 * 					character in the string is interpreted as a delimiter
	 */
	Tokenizer(const Token &raw, const std::string &delims);

	/**
	 * Sets the source string to tokenize and tokenizes it.
	 *
	 * @param raw	the string to tokenize
	 */
	void setSource (const std::string &raw);

	/**
	 * Sets the delimiters to tokenize over. Does not retokenize the source.
	 */
	void setDelims (const std::string &delims);

	/**
	 * Gets a reference to a vector of all of the tokens that were found in
	 * the source string.
	 *
	 * @return	an immutable vector reference of all tokens that were found
	 */
	const std::vector<Token>& getTokens () const;

	/**
	 * Gets the Token at the given position in the token vector.
	 *
	 * @param pos	the position of the Token
	 * @return		a const reference to the Token
	 */
	const Token& getToken (const size_t pos) const;

	/**
	 * Finds the index of the given Token in the token vector.
	 *
	 * @param token	the Token to search for
	 * @param start	the position to start searching from
	 */
	size_t indexOf (const Token &token, const size_t start = 0) const;
private:
	/**
	 * Removes any delimiters from the delimiter list that are not present in
	 * the source string. Used to speed up tokenizing time.
	 *
	 * @param raw	the source string that is being tokenized
	 */
	void limitDelims (const std::string &raw);

	/**
	 * Tokenizes the given string over the configured delimiters.
	 *
	 * @param raw	the source string to tokenize
	 */
	void tokenize (std::string raw);

	std::vector<Token> m_tokens;
	std::string m_delims;
};

/**
 * Thrown when the tokenizer encounters trouble tokenizing a string. For
 * example, this could be thrown when the tokenizer encounters an unclosed
 * string literal.
 */
class TokenizeException : public std::runtime_error
{
public:
	/**
	 * Constructs a TokenizeException with the given reason.
	 *
	 * @param reason	an explanation why the Tokenizer failed.
	 */
	TokenizeException(const std::string &reason);
};

#endif /* IASH_TOOLS_TOKENIZER_H_ */
