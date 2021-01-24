#ifndef __HEALTH_INST_SELECTOR_H_
#define __HEALTH_INST_SELECTOR_H_

#include <list>
#include "naming/selectors/Selector.h"

namespace nacos { namespace naming { namespace selectors {

class HealthInstanceSelector : public Selector<Instance>{
private:
public:
    std::list<Instance> select(const std::list<Instance> &instancesToSelect);
};
} /*selectors*/ } /*naming*/ }/*nacos*/

#endif