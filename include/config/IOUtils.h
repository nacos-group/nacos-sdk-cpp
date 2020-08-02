#ifndef __IO_UTILS_H_
#define __IO_UTILS_H_
#include <list>
#include "NacosString.h"
#include "NacosExceptions.h"

class IOUtils
{
private:
public:
	static size_t getFileSize(const NacosString &file);

	static NacosString readStringFromFile(const NacosString &file, const NacosString &encoding) throw (IOException);

	static void writeStringToFile(const NacosString &file, const NacosString &data, const NacosString &encoding) throw (IOException);

	//Returns true if:
	//a. the file doesn't exist
	//b. the file is not a regular file
	static bool checkNotExistOrNotFile(const NacosString &pathname);

	//Returns true if:
	//a. the file doesn't exist
	//b. the file is not a directory
	static bool checkNotExistOrNotDir(const NacosString &pathname);

	//TODO:To provide compability across different platforms
	static NacosString getParentFile(const NacosString &thefile);

	//Upon success, return true
	//Upon failure, return false
	static bool recursivelyRemove(const NacosString &file);

	static bool cleanDirectory(const NacosString &file);

	static void recursivelyCreate(const NacosString &file);
	
	static NacosString getCwd();

	static std::list<NacosString> listFiles(const NacosString &path);
};

#endif