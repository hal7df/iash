/*
 * Directory.h
 *
 *  Created on: Oct 30, 2016
 *      Author: paul
 */

#ifndef IASH_DIRECTORY_H_
#define IASH_DIRECTORY_H_

#include <string>

/**
 * Represents a directory on the filesystem. Intended to help with managing
 * filesystem resources in a cross-platform manner.
 *
 * @since 0.5
 */
class Directory {
public:
	/**
	 * Creates a Directory instance pointed at the application's current working
	 * directory. Note that this is NOT the same as the iash current working
	 * directory; this is actually held as a Directory instance internal to
	 * iash.
	 */
	Directory();

	/**
	 * Creates a Directory instance pointed at the directory `dir`.
	 *
	 * @param dir	the directory path to create this Directory at
	 */
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
	static std::string handleSeparators (const std::string &path);
	static const char* convToWindows (const std::string &path);
	std::string computeRelative (const std::string &relPath) const;
	void ensureTrailingSlash ();

	std::string m_dirpath;
	bool m_valid;
};

#endif /* IASH_DIRECTORY_H_ */
