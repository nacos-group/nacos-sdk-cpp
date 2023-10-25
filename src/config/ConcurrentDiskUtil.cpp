#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "ConcurrentDiskUtil.h"
#include "IOUtils.h"

/**
 * get file content
 *
 * @param file        file
 * @param charsetName charsetName
 * @return content
 * @throws IOException IOException
 */

namespace nacos{
NacosString
ConcurrentDiskUtil::getFileContent(const NacosString &file, const NacosString &charsetName) NACOS_THROW(IOException) {
    if (IOUtils::checkNotExistOrNotFile(file)) {
        //TODO:add errorcode
        throw IOException(NacosException::FILE_NOT_FOUND,
                          "checkNotExistOrNotFile failed, unable to access the file, maybe it doesn't exist.");
    }
    size_t toRead = IOUtils::getFileSize(file);
    FILE *fp = fopen(file.c_str(), "rb");
    if (fp == NULL) {
        char errbuf[100];
        snprintf(errbuf, sizeof(errbuf), "Failed to open file for read, errno: %d", errno);
        //TODO:add errorcode
        throw IOException(NacosException::UNABLE_TO_OPEN_FILE, errbuf);
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
        ) NACOS_THROW(IOException) {
    FILE *fp = fopen(path.c_str(), "wb");
    if (fp == NULL) {
        char errbuf[100];
        snprintf(errbuf, sizeof(errbuf), "Failed to open file for write, errno: %d", errno);
        //TODO:add errorcode
        throw IOException(NacosException::UNABLE_TO_OPEN_FILE, errbuf);
    }
    flock(fileno(fp), LOCK_SH);
    fwrite(content.c_str(), content.size(), 1, fp);
    flock(fileno(fp), LOCK_UN);
    fclose(fp);
    return true;
}
}//namespace nacos