#ifndef __WEIGHTED_RND_SELECTOR_H_
#define __WEIGHTED_RND_SELECTOR_H_

#include <list>
#include "naming/selectors/Selector.h"

#define BASIC_WEIGHT 65536

namespace nacos { namespace naming { namespace selectors {
class RandomByWeightSelector : public Selector<Instance>{
private:
public:
    std::list<Instance> select(const std::list<Instance> &instancesToSelect);
};
} /*selectors*/ } /*naming*/ }/*nacos*/

#endif