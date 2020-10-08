#ifndef __THREAD_LOCAL_H_
#define __THREAD_LOCAL_H_

#include <pthread.h>
#include "Debug.h"

template <typename T>
class ThreadLocal;
template <typename T>
struct ObjectWrapper {
    T wrappedObject;
    ThreadLocal<T> *threadLocalObj;
};

template <typename T>
class ThreadLocal{
private:
    pthread_key_t pthreadKey;
    T _defaultValue;
    static void destroyer(void *param) {
        ObjectWrapper<T> *wrapper = reinterpret_cast< ObjectWrapper<T> *>(param);
        log_debug("Calling destroyer...\n");
        wrapper->threadLocalObj->onDestroy(&wrapper->wrappedObject);
        delete wrapper;
    }

    ObjectWrapper<T> *getWrapper() const {
        ObjectWrapper<T> *wrapper = reinterpret_cast< ObjectWrapper<T> *>(pthread_getspecific(pthreadKey));
        return wrapper;
    }

    ObjectWrapper<T> *createWrapper() {
        ObjectWrapper<T> *wrapper = new ObjectWrapper<T>;
        wrapper->threadLocalObj = this;
        onCreate(&wrapper->wrappedObject);
        pthread_setspecific(pthreadKey, reinterpret_cast<void *>(wrapper));

        return wrapper;
    }
public:
    ThreadLocal() {
        log_debug("ThreadLocal::ThreadLocal() is called\n");
        /* init the curl session */
        pthread_key_create(&pthreadKey, destroyer);
    }

    ThreadLocal(T defaultValue) {
        log_debug("ThreadLocal::ThreadLocal(defaultValue) is called\n");
        _defaultValue = defaultValue;
        /* init the curl session */
        pthread_key_create(&pthreadKey, destroyer);
    }

    void set(T value) {
        ObjectWrapper<T> *wrapper = getWrapper();
        if (wrapper == NULL) {
            wrapper = createWrapper();
        }
        wrapper->wrappedObject = value;
    }

    T get() {
        ObjectWrapper<T> *wrapper = getWrapper();
        if (wrapper == NULL) {
            wrapper = createWrapper();
        }
        return wrapper->wrappedObject;
    }

    virtual ~ThreadLocal() {
        log_debug("ThreadLocal::~ThreadLocal is called\n");
        pthread_key_delete(pthreadKey);
    }

    virtual void onCreate(T *value) {
        //do nothing by default;
        //!!!!!shall NOT access anything other than VALUE!!!!!
        *value = _defaultValue;
        log_debug("ThreadLocal::onCreate is called\n");
    }

    virtual void onDestroy(T *value) {
        //do nothing by default;
        //!!!!!shall NOT access anything other than VALUE!!!!!
    }
};

#endif
