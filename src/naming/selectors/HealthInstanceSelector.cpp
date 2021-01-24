#include <list>
#include "naming/selectors/HealthInstanceSelector.h"

namespace nacos { namespace naming { namespace selectors {

std::list<nacos::Instance> HealthInstanceSelector::select(const std::list<nacos::Instance> &instancesToSelect){
    std::list<nacos::Instance> result;
    for (std::list<nacos::Instance>::const_iterator it = instancesToSelect.begin();
         it != instancesToSelect.end(); it++) {
        if (it->healthy) {
            result.push_back(*it);
        }
    }

    return result;
}

} /*selectors*/ } /*naming*/ }/*nacos*/
