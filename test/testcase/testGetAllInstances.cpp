#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "naming/NamingProxy.h"
#include "naming/NacosNamingService.h"
#include "naming/Instance.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"

using namespace std;

bool testGetAllInstances()
{
	cout << "in function testGetAllInstances" << endl;
	Properties configProps;
	configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
	NacosNamingService *namingSvc = new NacosNamingService(configProps);
	Instance instance;
	instance.clusterName = "DefaultCluster";
	instance.ip = "127.0.0.1";
	instance.port = 2333;
	instance.instanceId = "1";
	instance.ephemeral = true;

	try
	{
		for (int i = 0; i < 10; i++)
		{
			NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
			instance.port = 2000 + i;
			namingSvc->registerInstance(serviceName, instance);
		}
	}
	catch (NacosException e)
	{
		cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
		ReleaseResource(namingSvc);
		return false;
	}
	sleep(20);
	list<Instance> instances = namingSvc->getAllInstances("TestNamingService1");
	cout << "getAllInstances from server:" << endl;
	for (list<Instance>::iterator it = instances.begin();
		it != instances.end(); it++)
	{
		cout << "Instance:" << it->toString() << endl;
	}

	if (instances.size() != 1)
	{
		cout << "There should be only 1 instance for TestNamingService1" << endl;
		ReleaseResource(namingSvc);
		return false;
	}

	if (instances.front().port != 2001)
	{
		cout << "TestNamingService1's port should be 2001" << endl;
		ReleaseResource(namingSvc);
		return false;
	}

	if (instances.front().ip != "127.0.0.1")
	{
		cout << "TestNamingService1's ip should be 127.0.0.1" << endl;
		ReleaseResource(namingSvc);
		return false;
	}

	sleep(1);
	try
	{
		for (int i = 0; i < 3; i++)
		{
			NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
			
			namingSvc->deregisterInstance(serviceName, "127.0.0.1", 2000 + i);
			sleep(1);
		}
	}
	catch (NacosException e)
	{
		cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
		ReleaseResource(namingSvc);
		return false;
	}

	ReleaseResource(namingSvc);
	return true;
}