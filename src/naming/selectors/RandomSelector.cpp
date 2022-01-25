#include "naming/selectors/RandomSelector.h"
#include "src/log/Logger.h"
#include "src/utils/RandomUtils.h"
#include "src/utils/ParamUtils.h"

namespace nacos { namespace naming { namespace selectors {

std::list<Instance> RandomSelector::select(const std::list<Instance> &instancesToSelect){
    size_t maxSvrSlot = instancesToSelect.size();
    log_debug("RandomSelector::select:nr_servers%d\n", maxSvrSlot);
    size_t selectedServer;
    if (maxSvrSlot == 1) {
        selectedServer = 0;
    } else {
        selectedServer = RandomUtils::random(0, maxSvrSlot - 1);
    }
    log_debug("RandomSelector::select:%d\n", selectedServer);

    std::list<Instance> result;

    result.push_back(ParamUtils::getNthElem(instancesToSelect, selectedServer));

    return result;
}

} /*selectors*/ } /*naming*/ }/*nacos*/