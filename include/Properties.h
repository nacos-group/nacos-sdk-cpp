#ifndef __PROPERTIES_H_
#define __PROPERTIES_H_
#include <map>
#include "NacosString.h"

class Properties : public std::map<NacosString, NacosString>
{
};

#endif