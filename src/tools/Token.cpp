/*
 * Token.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: paul
 */

#include "Token.h"
using namespace std;

Token::Token(const string& rawToken, const char precedingDelim,
				const char followingDelim)
	: m_token(rawToken), m_precedingDelim(precedingDelim),
	  m_followingDelim(followingDelim)
{
	if (!m_token.empty() && m_token[0] == '\"' && *(m_token.end() - 1) == '\"')
	{
		m_token.erase(m_token.begin());
		m_token.erase(m_token.end() - 1);
	}
}

const string& Token::getToken () const
{
	return m_token;
}

const string& Token::finalize ()
{
	//Remove whitespace at front of token
	while (!m_token.empty() && isWhitespace(m_token[0]))
		m_token.erase(m_token.begin());
	while (!m_token.empty() && isWhitespace(*(m_token.end() - 1)))
		m_token.erase(m_token.end() - 1);

	//Remove any remaining backslashes
	for (size_t i = 0; i < (m_token.length() - 1); ++i)
	{
		if (m_token[i] == '\\')
		{
			switch (m_token[i + 1])
			{
			case 't':
				m_token.replace(i, 2, "\t");
				break;
			case 'n':
				m_token.replace(i, 2, "\n");
				break;
			case '\\':
				m_token.erase(i, 1);
				break;
			default:
				m_token.erase(i, 1);
				--i;
				break;
			}
		}
	}

	return m_token;
}

const char Token::getPrecedingDelimiter () const
{
	return m_precedingDelim;
}

const char Token::getFollowingDelimiter () const
{
	return m_followingDelim;
}

bool Token::operator== (const Token& rhs) const
{
	return m_token == rhs.m_token;
}

bool Token::isToken (const char delim) const
{
	return isToken(m_token, delim);
}

bool Token::isToken (const string &candidate, const char delim)
{
	size_t pos;
	size_t begin = 0;
	size_t end = candidate.length() - 1;

	while (isWhitespace(candidate[begin])) ++begin;
	while (isWhitespace(candidate[end])) --end;

	pos = begin;

	while ((pos = candidate.find(delim, pos + 1)) < end)
	{
		if (candidate[pos - 1] != '\\') return false;
	}

	return true;
}

bool Token::isWhitespace(const char c)
{
	return (c == ' ' || c == '\t' || c= '\n');
}
