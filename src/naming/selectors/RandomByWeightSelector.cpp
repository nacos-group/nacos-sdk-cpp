#include <list>
#include <vector>
#include "naming/selectors/RandomByWeightSelector.h"
#include "src/log/Logger.h"
#include "src/utils/ParamUtils.h"
#include "src/utils/RandomUtils.h"

#define BASIC_WEIGHT 65536

namespace nacos { namespace naming { namespace selectors {

std::list<Instance> RandomByWeightSelector::select(const std::list<Instance> &instancesToSelect){
    std::vector<std::pair<int, std::list<Instance>::const_iterator > > weightedList;
    std::list<Instance> result;

    int total_weight = 0;
    for (std::list<Instance>::const_iterator it = instancesToSelect.begin();
         it != instancesToSelect.end(); it++) {
        if (it->weight < 1e-10) {
            //we consider a very small weight as 0
            continue;
        }
        total_weight += it->weight * BASIC_WEIGHT;
        log_debug("RandomByWeightSelector::select:weight for current instance:%f\n", it->weight);
        weightedList.push_back(std::make_pair(it->weight * BASIC_WEIGHT, it));
    }
    if (total_weight == 0) {
        //no server instance is chosen
        return result;
    }
    log_debug("RandomByWeightSelector::select:total_weight:%d\n", total_weight);
    size_t selectedWeight = RandomUtils::random(0, total_weight - 1);
    log_debug("RandomByWeightSelector::select selected weight:%d\n", selectedWeight);

    std::vector<std::pair<int, std::list<Instance>::const_iterator> >::const_iterator it = weightedList.begin();
    while (selectedWeight > it->second->weight * BASIC_WEIGHT) {
        selectedWeight -= it->second->weight * BASIC_WEIGHT;
        it++;
    }

    result.push_back(*it->second);
    return result;
}

} /*selectors*/ } /*naming*/ }/*nacos*/