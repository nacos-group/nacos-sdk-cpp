#ifndef __RND_SELECTOR_H_
#define __RND_SELECTOR_H_

#include <list>
#include "naming/selectors/Selector.h"
#include "Debug.h"
#include "utils/ParamUtils.h"

namespace nacos { namespace naming { namespace selectors {
class RandomSelector : public Selector<Instance>{
private:
public:
    std::list<Instance> select(const std::list<Instance> &instancesToSelect){
        size_t maxSvrSlot = instancesToSelect.size();
        log_debug("RandomSelector::select:nr_servers%d\n", maxSvrSlot);
        srand(time(NULL));//TODO:optimize random generator
        size_t selectedServer = rand() % maxSvrSlot;
        log_debug("RandomSelector::select:%d\n", selectedServer);

        std::list<Instance> result;

        result.push_back(ParamUtils::getNthElem(instancesToSelect, selectedServer));

        return result;
    }
};
} /*selectors*/ } /*naming*/ }/*nacos*/

#endif