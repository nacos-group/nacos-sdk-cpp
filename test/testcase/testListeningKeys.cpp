#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "config/NacosConfigService.h"
#include "listen/Listener.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

class MyListener : public Listener
{
private:
    int num;
public:
    MyListener(int num)
    {
        this->num = num;
    }
    void receiveConfigInfo(const NacosString &configInfo)
    {
        cout << "===================================" << endl;
        cout << "Watcher" << num << endl;
        cout << "Watched Key UPDATED:" << configInfo << endl;
        cout << "===================================" << endl;
    }
};

bool testListeningKeys()
{
	cout << "in function testPublishConfig" << endl;
	Properties props;
	props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
	NacosConfigService *n = new NacosConfigService(props);
	bool bSucc;
	char cc;

	MyListener *theListener = new MyListener(1);
	MyListener *theListener2 = new MyListener(2);
	MyListener *theListener3 = new MyListener(3);
    n->addListener("dqid", NULLSTR, theListener);
    n->addListener("dqid", NULLSTR, theListener2);
    n->addListener("dqid", NULLSTR, theListener3);

    cin >> cc;
    cout << "remove listener" << endl;
    n->removeListener(theListener);

    cin >> bSucc;
    n->removeListener(theListener2);
    n->removeListener(theListener3);
    cin >> bSucc;

	ReleaseResource(n);
	return true;
}