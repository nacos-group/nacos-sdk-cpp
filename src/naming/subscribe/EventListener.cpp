#include "naming/subscribe/EventListener.h"
#include "src/debug/DebugAssertion.h"

namespace nacos {

EventListener::~EventListener() {
    NACOS_ASSERT(refCnt() == 0)
}

}//namespace nacos
