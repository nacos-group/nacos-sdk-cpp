#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Debug.h"
#include "DebugAssertion.h"
#include "listen/Listener.h"
#include "http/ServerHttpAgent.h"
#include "config/NacosConfigService.h"
#include "PropertyKeyConst.h"

using namespace std;

class KeyChangeListener : public Listener
{
private:
	NacosString key;
public:
	void setKey(const NacosString &_key) {key = _key;};
	NacosString getKey() const {return key;};
	void receiveConfigInfo(const NacosString &configInfo)
	{
		cout << "in receiveConfigInfo: " << key << " changed to " << configInfo << endl;
	}
};

bool testAddListener()
{
	cout << "in function testAddListener" << endl;
	Properties props;
	props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
	NacosConfigService *n = NULL;
	KeyChangeListener *thelistener = new KeyChangeListener();
	thelistener->setKey("k");
	bool bSucc;
	try
	{
		n = new NacosConfigService(props);
		n->addListener("k4", NULLSTR, thelistener);
		n->addListener("k2", NULLSTR, thelistener);
		n->addListener("k", NULLSTR, thelistener);
		bSucc = n->publishConfig("k", NULLSTR, "hahaha");
	}
	catch (NacosException e)
	{
		cout <<
		"Failed to add listener" << endl <<
		"Reason:" << e.what() << endl;
		ReleaseResource(n);
		return false;
	}

	getchar();
	ReleaseResource(n);
	SHOULD_BE_TRUE(bSucc, "Publish should succeed");
	return true;
}