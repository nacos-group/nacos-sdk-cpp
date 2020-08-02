#include <iostream>
#include <stdlib.h>
#include <unistd.h>
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

bool testNamingProxySmokeTest()
{
	cout << "in function testNamingProxySmokeTest" << endl;
	NacosString servers = "127.0.0.1:8848";
	HTTPCli *httpcli = new HTTPCli();
	NamingProxy *namingProxy = new NamingProxy(httpcli, UtilAndComs::DEFAULT_NAMESPACE_ID, NULLSTR/*endpoint*/, servers);
	Instance theinstance;
	theinstance.instanceId = "TestInstance";
	theinstance.ip = "127.0.0.1";
	theinstance.port = 3333;
	theinstance.clusterName = "TestCluster";

	//Create a clean environment
    try
	{
	    for (int i = 0; i < 10; i++)
        {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->deregisterService(serviceName, theinstance);
        }
	}
	catch(NacosException e)
	{
	    cout << "Exception caught during deregistering service, raison:" << e.what() << endl;

        ReleaseResource(namingProxy);
        ReleaseResource(httpcli);
        return false;
	}

	//Register 100 services
	try
	{
	    for (int i = 0; i < 10; i++)
        {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->registerService(serviceName, Constants::DEFAULT_GROUP, theinstance);
            sleep(1);
        }
	}
	catch(NacosException e)
	{
	    cout << "Exception caught during registering service, raison:" << e.what() << endl;

        ReleaseResource(namingProxy);
        ReleaseResource(httpcli);
        return false;
	}

	//check whether the data are correct
	for (int i = 0; i < 10; i++)
    {
        NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
        NacosString serverlist = namingProxy->queryList(serviceName, "TestCluster", 0, false);

        if (serverlist.find("\"serviceName\":\"" + serviceName + "\"") == string::npos)
        {
            cout << "Failed to get data for:" << serviceName << endl;
            ReleaseResource(namingProxy);
            ReleaseResource(httpcli);
            return false;
        }
        cout << "Servers from nacos:" + serverlist << endl;
    }

    //Clear-ups
    try
	{
	    for (int i = 0; i < 10; i++)
        {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->deregisterService(serviceName, theinstance);
        }
	}
	catch(NacosException e)
	{
	    cout << "Exception caught during cleaning the test environment, raison:" << e.what() << endl;

        ReleaseResource(namingProxy);
        ReleaseResource(httpcli);
        return false;
	}

	ReleaseResource(namingProxy);
	ReleaseResource(httpcli);
	return true;
}

bool testNamingProxyFailOver()
{
	cout << "in function testNamingProxyFailOver" << endl;
	return true;
}

bool testNamingServiceRegister()
{
	cout << "in function testNamingServiceRegister" << endl;
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
		for (int i = 0; i < 400; i++)
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
	sleep(30);
	try
	{
		for (int i = 20; i < 40; i++)
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
	sleep(30);

	ReleaseResource(namingSvc);
	return true;
}
