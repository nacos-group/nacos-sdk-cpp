#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "config/ConcurrentDiskUtil.h"
#include "config/IOUtils.h"

/**
 * get file content
 *
 * @param file        file
 * @param charsetName charsetName
 * @return content
 * @throws IOException IOException
 */
NacosString
ConcurrentDiskUtil::getFileContent(const NacosString &file, const NacosString &charsetName) throw(IOException) {
    if (IOUtils::checkNotExistOrNotFile(file)) {
        //TODO:add errorcode
        throw IOException(99999, "checkNotExistOrNotFile failed, unable to access the file, maybe it doesn't exist.");
    }
    size_t toRead = IOUtils::getFileSize(file);
    FILE *fp = fopen(file.c_str(), "rb");
    if (fp == NULL) {
        char errbuf[100];
        sprintf(errbuf, "Failed to open file for read, errno: %d", errno);
        //TODO:add errorcode
        throw IOException(99999, errbuf);
    }
    flock(fileno(fp), LOCK_SH);
    char buf[toRead + 1];
    fread(buf, toRead, 1, fp);
    buf[toRead] = '\0';
    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return NacosString(buf);
}

/**
 * write file content
 *
 * @param file        file
 * @param content     content
 * @param charsetName charsetName
 * @return whether write ok
 * @throws IOException IOException
 */
bool ConcurrentDiskUtil::writeFileContent
        (
                const NacosString &path,
                const NacosString &content,
                const NacosString &charsetName
        ) throw(IOException) {
    FILE *fp = fopen(path.c_str(), "wb");
    if (fp == NULL) {
        char errbuf[100];
        sprintf(errbuf, "Failed to open file for write, errno: %d", errno);
        //TODO:add errorcode
        throw IOException(99999, errbuf);
    }
    flock(fileno(fp), LOCK_SH);
    fwrite(content.c_str(), content.size(), 1, fp);
    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return true;
}