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

class Tokenizer {
public:
	Tokenizer();
	Tokenizer(const std::string &raw);
	Tokenizer(const std::string &raw, const char delim);
	Tokenizer(const std::string &raw, const std::string &delims);
	Tokenizer(const Token &raw);
	Tokenizer(const Token &raw, const char delim);
	Tokenizer(const Token &raw, const std::string &delims);

	void setSource (const std::string &raw);
	void setDelims (const std::string &delims);

	const std::vector<Token>& getTokens () const;
	const Token& getToken (const size_t pos) const;

	size_t indexOf (const Token &token, const size_t start = 0) const;
private:
	void limitDelims (const std::string &raw);
	void tokenize (std::string raw);

	std::vector<Token> m_tokens;
	std::string m_delims;
};

class TokenizeException : public std::runtime_error
{
public:
	TokenizeException(const std::string &reason);
};

#endif /* IASH_TOOLS_TOKENIZER_H_ */
