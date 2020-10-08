#ifndef __NACOS_LIST_VUE_H_
#define __NACOS_LIST_VUE_H_

#include <list>
#include "NacosString.h"

template <typename T>
class ListView{
private:
    int count;
    std::list<T> data;
public:
    int getCount() const { return count; };
    std::list<T> getData() const { return data; };
    void setCount(int _count) { count = _count; };
    void setData(const std::list<T> &_data) { data = _data; };
};

#endif
