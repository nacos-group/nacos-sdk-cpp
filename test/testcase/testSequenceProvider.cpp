#include <iostream>
#include "src/thread/Thread.h"
#include "src/utils/SequenceProvider.h"

using namespace std;
using namespace nacos;

SequenceProvider<uint64_t> *sequenceProvider;

void *SeqThreadFunc(void *param) {
    Thread *thisThread = *((Thread **) param);
    for (int i = 0; i < 10; i++) {
        log_debug("Thread %s SEQ: %u\n", thisThread->getThreadName().c_str(), sequenceProvider->next());
        cout << "Thread " << thisThread->getThreadName().c_str() << " SEQ:" << ":" << sequenceProvider->next() << endl;
    }

    return NULL;
}

bool testSequenceProvider() {
    cout << "in function testSequenceProvider" << endl;

    cout << "Generating SEQ..." << endl;

    sequenceProvider = new SequenceProvider<uint64_t> ("/var/tmp/defaultSeq.dat", 20000, 100);

    Thread *threads[10] = {NULL};
    for (int i = 0; i < 10; i++) {
        NacosString threadName = "SEQThread-" + NacosStringOps::valueOf(i);
        threads[i] = new Thread(threadName, SeqThreadFunc, (void *) &threads[i]);
        threads[i]->start();
    }

    for (int i = 0; i < 10; i++) {
        threads[i]->join();
        delete threads[i];
    }

    cout << "test end..." << endl;

    return true;
}
