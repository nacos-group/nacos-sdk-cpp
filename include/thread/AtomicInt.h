#ifndef __ATOMIC_INT_H_
#define __ATOMIC_INT_H_

#define __atomic_fool_gcc(x) (*(volatile struct { int a[100]; } *)x)
#define LOCK_PREFIX "lock "

class AtomicInt {
private:
    volatile int _intval;
public:
    AtomicInt(int initval = 0) : _intval(initval) {};

    int inc(int incval = 1) {
        int __i = incval;
        asm volatile(LOCK_PREFIX "xaddl %0, %1"
        : "+r" (incval), "+m" (_intval)
        : : "memory");
        return incval + __i;
    };

    int dec(int decval = 1) {
        return inc(-decval);
    };

    int get() const { return _intval; };
};

#endif
