/*
 * Directory.h
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#ifndef IASH_DIRECTORY_H_
#define IASH_DIRECTORY_H_

#include <string>

class Directory {
public:
	Directory();
	Directory(const std::string &dir);
	virtual ~Directory();

	bool isValid () const;
	const std::string& getAbsPath () const;
	bool changeDirRel (const std::string &relPath);
	bool changeDirAbs (const std::string &absPath);
	const char* getPathToFileInDirectory (const std::string &filename) const;
	int mkSubdir (const std::string &dirName) const;

	static int mkdir (const char *pathname);
	static bool isDir (const char *pathname);
	static bool isFile (const char *pathname);
	static Directory getHomeDir ();
	static Directory getConfigDir ();
	static Directory getWorkingDir ();
private:
	std::string computeRelative (const std::string &relPath) const;
	void ensureTrailingSlash ();

	std::string m_dirpath;
	bool m_valid;
};

#endif /* IASH_DIRECTORY_H_ */
