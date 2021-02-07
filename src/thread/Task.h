#ifndef __TASK_H_
#define __TASK_H_

#include "NacosString.h"

namespace nacos{
class Task {
private:
    NacosString _taskName;
public:
    virtual void run() = 0;

    virtual ~Task() {};

    void setTaskName(const NacosString &taskName) { _taskName = taskName; };

    const NacosString &getTaskName() const { return _taskName; };
};

}//namespace nacos

#endif
