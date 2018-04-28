/*
 * Token.h
 *
 *  Created on: Nov 22, 2016
 *      Author: paul
 */

#ifndef SRC_TOOLS_TOKEN_H_
#define SRC_TOOLS_TOKEN_H_

#include <string>

/**
 * Represents a single unit of a command language. Stores information
 * about the delimiters on either side of the Token from its source in case
 * the delimiters provide contextual information about the token.
 * <p>
 * There is usually no need to construct a Token directly, use a Tokenizer to
 * get Tokens from an input string.
 *
 * @since 0.5
 */
class Token {
public:
	/**
	 * Creates a Token with the given content and surrounding delimiters. If a
	 * Token does not have a delimiter on a side (possibly due to it being at
	 * the beginning or end of input), the null character (`'\0'`) should be
	 * passed in as appropriate.
	 *
	 * @param rawToken			the content of the Token
	 * @param precedingDelim	the delimiter preceding the token, or `'\0'` if
	 * 							not applicable
	 * @param followingDelim	the delimiter following the token, or `'\0'` if
	 * 							not applicable
	 */
	Token (const std::string& rawToken, const char precedingDelim,
			const char followingDelim);

	/**
	 * Gets the content of the Token.
	 *
	 * @return	the content of the Token
	 */
	const std::string& getToken () const;

	/**
	 * Removes any Tokenizer-specific formatting. Specifically, this removes
	 * backslashes and leading/trailing whitespace.
	 *
	 * @return	the finalized Token
	 */
	const std::string& finalize ();

	/**
	 * Gets the delimiter that came before this Token in the source string.
	 *
	 * @return	the preceding delimiter
	 */
	const char getPrecedingDelimiter () const;

	/**
	 * Gets the delimiter that came after this Token in the source string.
	 *
	 * @return	the following delimiter
	 */
	const char getFollowingDelimiter () const;

	/**
	 * Checks if this Token instance is also a Token for the given delimiter.
	 *
	 * @param delim	the delimiter to check for
	 * @return		true if no instances of the delimiter were found (excluding
	 * 				leading/trailing delimiters), false otherwise
	 */
	bool isToken (const char delim) const;

	/**
	 * Checks if the given candidate string is a Token for the given delimiter.
	 *
	 * @param candidate	the candidate string to search through
	 * @param delim		the delimiter to check for
	 * @return			true if no instances of the delimiter were found
	 *					(excluding leading/trailing delimiters), false otherwise
	 */
	static bool isToken (const std::string &candidate, const char delim);

	/**
	 * Checks for equality between tokens. This does <i>not</i> account for
	 * the delimiters on either side of the token.
	 *
	 * @param other	the other token to check for equality
	 * @return		true if the contents of the Tokens are equal, false
	 *				otherwise.
	 */
	bool operator== (const Token& other) const;
private:
	/**
	 * Helper function to check if a char is a whitespace character.
	 *
	 * @param c	the character to check
	 * @return	if the character is a whitespace character or not
	 */
	static bool isWhitespace (const char c);

	std::string m_token;
	char m_precedingDelim;
	char m_followingDelim;
};

#endif /* SRC_TOOLS_TOKEN_H_ */
