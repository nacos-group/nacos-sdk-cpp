#ifndef __SELECTOR_H_
#define __SELECTOR_H_

#include <list>
#include "naming/Instance.h"

//Selector interface definition
//All Selectors shall be THREAD-SAFE!
template <typename T>
class Selector{
private:
public:
    virtual std::list<T> select(const std::list<T> &itemsToSelect) = 0;
    virtual ~Selector() {};
};

#endif