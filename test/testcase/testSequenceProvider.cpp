#include <iostream>
#include "src/thread/Thread.h"
#include "src/utils/SequenceProvider.h"
#include "src/utils/DirUtils.h"

using namespace std;
using namespace nacos;

#define NR_THREADS 200
#define GENERATION_PER_THREAD 1000

int64_t sequences[GENERATION_PER_THREAD * NR_THREADS];
int tid[NR_THREADS];

SequenceProvider<uint64_t> *sequenceProvider;

void *SeqThreadFunc(void *param) {
    int *thread_no = (int*)param;
    for (int i = 0; i < GENERATION_PER_THREAD; i++) {
        int64_t res = sequenceProvider->next();
        sequences[(*thread_no) * GENERATION_PER_THREAD + i] = res;
    }

    return NULL;
}

bool testSequenceProvider() {
    cout << "in function testSequenceProvider" << endl;

    cout << "Generating SEQ..." << endl;

    sequenceProvider = new SequenceProvider<uint64_t> (DirUtils::getCwd() + "/test_seq.dat", 20000, 100);

    Thread *threads[NR_THREADS] = {NULL};
    for (int i = 0; i < NR_THREADS; i++) {
        NacosString threadName = "SEQThread-" + NacosStringOps::valueOf(i);
        tid[i] = i;
        threads[i] = new Thread(threadName, SeqThreadFunc, (void *) &tid[i]);
        threads[i]->start();
    }

    for (int i = 0; i < NR_THREADS; i++) {
        threads[i]->join();
        delete threads[i];
    }

    cout << "Generated." << endl;

    for (int i = 0; i < NR_THREADS; i++) {
        for (int j = 0; j < GENERATION_PER_THREAD; j++) {
            cout << "Thread " << i << ": sequence =\t" << sequences[i * GENERATION_PER_THREAD + j] << endl;
        }
    }

    cout << "test end..." << endl;

    return true;
}
