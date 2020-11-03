#ifndef __HEALTH_INST_SELECTOR_H_
#define __HEALTH_INST_SELECTOR_H_

#include <list>
#include "naming/selectors/Selector.h"
#include "Debug.h"

namespace nacos { namespace naming { namespace selectors {
class HealthInstanceSelector : public Selector<Instance>{
private:
public:
    std::list<Instance> select(const std::list<Instance> &instancesToSelect){
        std::list<Instance> result;
        for (std::list<Instance>::const_iterator it = instancesToSelect.begin();
            it != instancesToSelect.end(); it++) {
            if (it->healthy) {
                result.push_back(*it);
            }
        }

        return result;
    }
};
} /*selectors*/ } /*naming*/ }/*nacos*/

#endif