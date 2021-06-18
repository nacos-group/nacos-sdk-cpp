#ifndef __ATOMIC_INT_H_
#define __ATOMIC_INT_H_

namespace nacos{
template<typename T>
class AtomicInt {
private:
    volatile T _curval;
public:
    AtomicInt(T curval = 0) : _curval(curval) {};

    void set(T val) { _curval = val; };

    T inc(T incval = 1) {
        T oldValue = getAndInc(incval);
        return incval + oldValue;
    };

    T getAndInc(T incval = 1) {
        T oldValue = __sync_fetch_and_add(&_curval, incval);
        return oldValue;
    }

    T dec(int decval = 1) {
        return inc(-decval);
    };

    T get() const { return _curval; };
};
}//namespace nacos

#endif
