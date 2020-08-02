#ifndef __URL_H_
#define __URL_H_
#include "NacosString.h"

NacosString urlencode(const NacosString & content);
NacosString urldecode(const NacosString & content);

#endif