/*
 * Token.h
 *
 *  Created on: Nov 22, 2016
 *      Author: paul
 */

#ifndef IASH_TOOLS_TOKEN_H_
#define IASH_TOOLS_TOKEN_H_

#include <string>

class Token {
public:
	Token (const std::string& rawToken, const char precedingDelim,
			const char followingDelim);

	const std::string& getToken () const;
	const std::string& finalize ();
	const char getPrecedingDelimiter () const;
	const char getFollowingDelimiter () const;

	bool operator== (const Token& other) const;
private:
	static bool isWhitespace (const char c);

	std::string m_token;
	char m_precedingDelim;
	char m_followingDelim;
};

#endif /* IASH_TOOLS_TOKEN_H_ */
