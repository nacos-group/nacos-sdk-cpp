#include "naming/cache/ChangeAdvice.h"

using namespace std;

ChangeAdvice::ChangeAdvice()
{
    added = false;
    modified = false;
    removed = false;
    //addedInstances.clear();
    //removedInstances.clear();
    //modifiedInstances.clear();
}

ChangeAdvice::~ChangeAdvice()
{
}

void ChangeAdvice::compareChange
(
        ServiceInfo &oldInfo,
        ServiceInfo &newInfo,
        ChangeAdvice &changeAdvice
)
{
    map<NacosString, Instance> oldInstanceList;
    map<NacosString, Instance> newInstanceList;
    for (list<Instance>::iterator it = oldInfo.getHostsNocopy()->begin();
        it != oldInfo.getHostsNocopy()->end(); it++)
    {
        oldInstanceList[it->toInetAddr()] = *it;
    }

    for (list<Instance>::iterator it = newInfo.getHostsNocopy()->begin();
         it != newInfo.getHostsNocopy()->end(); it++)
    {
        newInstanceList[it->toInetAddr()] = *it;
    }

    //find removed instances
    for (map<NacosString, Instance>::iterator it = oldInstanceList.begin();
        it != oldInstanceList.end(); it++)
    {
        if (newInstanceList.count(it->first) == 0)
        {
            changeAdvice.removed = true;
            //changeAdvice.removedInstances.push_back(it->second);
        }
        else//find modified instances
        {
            //the item exists in both Lists, compare the content between these 2
            if (it->second != newInstanceList[it->first])
            {
                changeAdvice.modified = true;
                //changeAdvice.modifiedInstances.push_back(newInstanceList[it->first]);
            }

        }
    }

    //find added instances
    for (map<NacosString, Instance>::iterator it = newInstanceList.begin();
         it != newInstanceList.end(); it++)
    {
        if (oldInstanceList.count(it->first) == 0)
        {
            changeAdvice.added = true;
            //changeAdvice.addedInstances.push_back(it->second);
        }
    }
}

NacosString ChangeAdvice::toString()
{
    return "Unimplemented";//trivial function
}