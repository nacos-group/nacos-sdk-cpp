#ifndef __RND_SELECTOR_H_
#define __RND_SELECTOR_H_

#include <list>
#include "naming/selectors/Selector.h"

namespace nacos { namespace naming { namespace selectors {
class RandomSelector : public Selector<Instance>{
private:
public:
    std::list<Instance> select(const std::list<Instance> &instancesToSelect);
};
} /*selectors*/ } /*naming*/ }/*nacos*/

#endif