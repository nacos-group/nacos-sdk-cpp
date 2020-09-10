#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils/UuidUtils.h"

NacosString UuidUtils::generateUuid()
{
	int fd=0;
	char buf[16];

	fd=open("/dev/urandom",O_RDONLY);

	int k = 0;
	while(k < 16){
		read(fd, buf+k, 1);
		if(std::isalpha(*(buf+k)) || std::isalnum(*(buf+k))){
			k++;
		}
	}

	close(fd);
	return std::string(buf);
}
