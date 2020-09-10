#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils/UuidUtils.h"

NacosString UuidUtils::generateUuid()
{
	int fd=0;
	char buf;

	fd=open("/dev/urandom",O_RDONLY);

	int k = 0;
	std::string uuid(16, ' ');
	while(k < 16){
		read(fd, &buf, 1);
		if(std::isalpha(buf) || std::isalnum(buf)){
			uuid[k] = buf;
			k++;
		}
	}

	close(fd);
	return uuid;
}
