/*
 * Tokenizer.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: paul
 */

#include "Tokenizer.h"
#include <iostream>
using namespace std;

Tokenizer::Tokenizer() : m_delims(1, ' ') {}

Tokenizer::Tokenizer(const string &raw) : m_delims(1, ' ')
{
	tokenize(raw);
}

Tokenizer::Tokenizer(const string &raw, const char delim) : m_delims(1, delim)
{
	tokenize(raw);
}

Tokenizer::Tokenizer(const string &raw, const string& delims)
	: m_delims(delims)
{
	limitDelims(raw);
	tokenize(raw);
}

Tokenizer::Tokenizer(const Token &raw) : m_delims(1, ' ')
{
	tokenize(raw.getToken());
}

Tokenizer::Tokenizer(const Token &raw, const char delim) : m_delims(1, delim)
{
	tokenize(raw.getToken());
}

Tokenizer::Tokenizer(const Token &raw, const string &delims) : m_delims(delims)
{
	tokenize(raw.getToken());
}

void Tokenizer::setSource (const string &raw)
{
	tokenize(raw);
}

void Tokenizer::setDelims (const string &delims)
{
	m_delims = delims;
}

const std::vector<Token>& Tokenizer::getTokens () const
{
	return m_tokens;
}

const Token& Tokenizer::getToken (const size_t pos) const
{
	return m_tokens.at(pos);
}

size_t Tokenizer::indexOf (const Token &token, const size_t start) const
{
	for (size_t i = start; i < m_tokens.size(); ++i)
	{
		if (m_tokens.at(i) == token) return i;
	}

	return -1;
}

void Tokenizer::limitDelims (const string &raw)
{
	for (size_t i = 0; i < m_delims.length(); ++i)
	{
		if (raw.find(m_delims[i]) == string::npos) m_delims.erase(i);
	}
}

void Tokenizer::tokenize(string raw)
{
	size_t curDelim, lastDelim;
	bool foundDelim;
	string tmpToken;
	char last, cur;

	lastDelim = 0;
	limitDelims(raw);

	do
	{
		foundDelim = false;

		//Find the next delimiter
		for (curDelim = (lastDelim + 1); curDelim < raw.length(); ++curDelim)
		{
			if (raw[curDelim] == '\"')
			{
				curDelim = (raw.find('\"', curDelim + 1) + 1);

				if (curDelim == string::npos)
					throw TokenizeException("Unclosed string literal");
			}

			for (char c : m_delims)
			{
				if (raw[curDelim] == c && raw[curDelim - 1] != '\\')
					foundDelim = true;
			}

			if (foundDelim) break;
		}

		//Get the token
		if (lastDelim > 0)
			tmpToken = raw.substr(lastDelim + 1, (curDelim - lastDelim));
		else tmpToken = raw.substr(0, (curDelim - lastDelim));

		//Remove any escapes for delimiters in this token
		for (size_t i = 0; i < (tmpToken.length() - 1) && (tmpToken.length() > 0); ++i)
		{
			if (tmpToken[i] == '\\')
			{
				for (char c : m_delims)
				{
					if (tmpToken[i + 1] == c)
					{
						tmpToken.erase(i,1);
						--i;
						break;
					}
				}
			}
		}

		//Get the delimiters on either side of the token
		last = lastDelim == 0 ? '\0' : raw[lastDelim];
		cur = curDelim == raw.length() ? '\0' : raw[curDelim];

		m_tokens.push_back(Token(tmpToken, last, cur));

		lastDelim = curDelim;
	} while (curDelim < raw.length());
}

TokenizeException::TokenizeException(const string &reason)
	: runtime_error("tokenize error: " + reason) {}
