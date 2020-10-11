#ifndef __URL_H_
#define __URL_H_

#include "NacosString.h"

namespace nacos{
NacosString urlencode(const NacosString &content);

NacosString urldecode(const NacosString &content);
}//namespace nacos

#endif