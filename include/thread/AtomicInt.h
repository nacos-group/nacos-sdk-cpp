#ifndef __ATOMIC_INT_H_
#define __ATOMIC_INT_H_

namespace nacos{
class AtomicInt {
private:
    volatile int _intval;
public:
    AtomicInt(int initval = 0) : _intval(initval) {};

    int inc(int incval = 1) {
        int oldValue = __sync_fetch_and_add(&_intval, incval);
        return incval + oldValue;
    };

    int dec(int decval = 1) {
        return inc(-decval);
    };

    int get() const { return _intval; };
};
}//namespace nacos

#endif
