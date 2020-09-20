#include "config/IOUtils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <list>
#include <unistd.h>
#include "Debug.h"

size_t IOUtils::getFileSize(const NacosString &file) {
    struct stat statbuf;

    if (stat(file.c_str(), &statbuf) == -1) {
        return 0;
    }

    return statbuf.st_size;
}

NacosString IOUtils::readStringFromFile(const NacosString &file, const NacosString &encoding) throw(IOException) {
    size_t toRead = getFileSize(file);
    FILE *fp = fopen(file.c_str(), "rb");
    if (fp == NULL) {
        throw IOException(NacosException::FILE_NOT_FOUND, "File not found:" + file);
    }
    char buf[toRead + 1];
    fread(buf, toRead, 1, fp);
    buf[toRead] = '\0';
    fclose(fp);
    return NacosString(buf);
}

void IOUtils::writeStringToFile(const NacosString &file, const NacosString &data,
                                const NacosString &encoding) throw(IOException) {
    FILE *fp = fopen(file.c_str(), "wb");
    fwrite(data.c_str(), data.size(), 1, fp);
    fclose(fp);
}

//Returns true if:
//a. the file doesn't exist
//b. the file is not a regular file
bool IOUtils::checkNotExistOrNotFile(const NacosString &pathname) {
    struct stat thestat = {0};
    int res = stat(pathname.c_str(), &thestat);

    if (res != 0) {
        if (errno == ENOENT) {
            //a. the file doesn't exist
            return true;
        } else {
            //Maybe something's wrong with the permission
            //Anyway, we have no access to this file
            return true;
        }
    }

    if (!S_ISREG(thestat.st_mode)) {
        //b. the file is not a regular file
        return true;
    } else {

        //This IS a regular file
        return false;
    }
}

//Returns true if:
//a. the file doesn't exist
//b. the file is not a directory
bool IOUtils::checkNotExistOrNotDir(const NacosString &pathname) {
    struct stat thestat = {0};
    int res = stat(pathname.c_str(), &thestat);

    if (res != 0) {
        if (errno == ENOENT) {
            //a. the file doesn't exist
            return true;
        } else {
            //Maybe something's wrong with the permission
            //Anyway, we have no access to this file
            return true;
        }
    }

    if (!S_ISDIR(thestat.st_mode)) {
        //b. the file is not a directory
        return true;
    } else {
        //This IS a directory
        return false;
    }
}

//TODO:To provide compability across different platforms
NacosString IOUtils::getParentFile(const NacosString &thefile) {
    size_t parentFilePos = thefile.rfind('/');
    //Invalid Directory/Filename, returning empty
    if (parentFilePos == std::string::npos || parentFilePos == 0) {
        return NULLSTR;
    }
    NacosString parentFile = thefile.substr(0, parentFilePos);
    return parentFile;
}

//Upon success, return true
//Upon failure, return false
bool IOUtils::recursivelyRemove(const NacosString &file) {
    struct stat thestat;

    if (stat(file.c_str(), &thestat) == -1 && errno != ENOENT) {
        //Something's wrong, and it's not "FileNotExist", we should record this and exit
        log_error("Failed to stat() file, errno: %d\n", errno);
        return false;
    }

    if (S_ISDIR(thestat.st_mode)) {
        DIR *curdir = opendir(file.c_str());
        struct dirent *direntp = readdir(curdir);
        while (direntp != NULL) {
            if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) {
                //skip this dir and parent
                direntp = readdir(curdir);
                continue;
            }
            struct stat subfilestat;
            NacosString subfilepath = file + "/" + direntp->d_name;

            if (stat(subfilepath.c_str(), &subfilestat) == -1 && errno != ENOENT) {
                log_error("Failed to stat() file, errno: %d\n", errno);
                closedir(curdir);
                return false;
            }
            if (S_ISREG(subfilestat.st_mode)) {
                remove(subfilepath.c_str());
            } else if (S_ISDIR(subfilestat.st_mode)) {
                recursivelyRemove(subfilepath);
            }
            //get to the next entry
            direntp = readdir(curdir);
        }
        closedir(curdir);
        remove(file.c_str());
    } else if (S_ISREG(thestat.st_mode)) {
        remove(file.c_str());
    }

    return true;
}

bool IOUtils::cleanDirectory(const NacosString &file) {
    struct stat thestat;

    if (stat(file.c_str(), &thestat) == -1 && errno != ENOENT) {
        //Something's wrong, and it's not "FileNotExist", we should record this and exit
        log_error("Failed to stat() file, errno: %d\n", errno);
        return false;
    }

    if (!S_ISDIR(thestat.st_mode)) {
        log_error("Call cleanDirectory() on non-directory entity: %s\n", file.c_str());
        return false;
    }

    DIR *curdir = opendir(file.c_str());
    struct dirent *direntp = readdir(curdir);
    while (direntp != NULL) {
        if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) {
            //skip this dir and parent
            direntp = readdir(curdir);
            continue;
        }
        NacosString subfilepath = file + "/" + direntp->d_name;

        recursivelyRemove(subfilepath);
        //get to the next entry
        direntp = readdir(curdir);
    }
    closedir(curdir);

    return true;
}

void IOUtils::recursivelyCreate(const NacosString &file) {
    NacosString parentFile = getParentFile(file);
    if (!isNull(parentFile)) {
        recursivelyCreate(parentFile);
    }

    if (checkNotExistOrNotDir(file)) {
        mkdir(file.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
}

std::list <NacosString> IOUtils::listFiles(const NacosString &path) {
    struct stat thestat;
    std::list <NacosString> filelist;
    if (stat(path.c_str(), &thestat) == -1 && errno != ENOENT) {
        //Something's wrong, and it's not "FileNotExist", we should record this and exit
        log_error("Failed to stat() file, errno: %d\n", errno);
        return filelist;
    }

    if (!S_ISDIR(thestat.st_mode)) {
        log_error("Call listFiles() on non-directory entity: %s\n", path.c_str());
        return filelist;
    }

    DIR *curdir = opendir(path.c_str());
    struct dirent *direntp = readdir(curdir);
    while (direntp != NULL) {
        if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) {
            //skip this dir and parent
            direntp = readdir(curdir);
            continue;
        }
        NacosString curitem = direntp->d_name;
        filelist.push_back(curitem);
        //get to the next entry
        direntp = readdir(curdir);
    }
    closedir(curdir);

    return filelist;
}