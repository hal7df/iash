/*
 * IashEnvironment.h
 *
 *  Created on: Oct 10, 2016
 *      Author: paul
 */

#ifndef IASH_IASHENVIRONMENT_H_
#define IASH_IASHENVIRONMENT_H_

#include <map>

class IashEnvironment {
public:
	IashEnvironment();
	IashEnvironment (std::string filename);
	virtual ~IashEnvironment();

	void load (std::string filename);
	void save (std::string filename);

	void set (std::string key, std::string value);
	void set (std::string key, bool value);
	void set (std::string key, int value);
	void set (std::string key, double value);

	std::string getString (std::string key);
	bool getBool (std::string key);
	int getInt (std::string key);
	double getDouble (std::string key);

	void rm (std::string key);
private:
	std::map<std::string,std::string> m_envMap;
};

#endif /* IASH_IASHENVIRONMENT_H_ */
