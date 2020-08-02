#include "naming/NacosNamingService.h"
#include "naming/beat/BeatReactor.h"
#include "utils/NamingUtils.h"
#include "utils/UtilAndComs.h"
#include "utils/ParamUtils.h"
#include "PropertyKeyConst.h"
#include "json/JSON.h"

using namespace std;

void NacosNamingService::initNamespace(Properties &props)
{
	if (props.count(PropertyKeyConst::NAMESPACE) == 0)//TODO:implement initNamespace
	{
		namesp = UtilAndComs::DEFAULT_NAMESPACE_ID;
	}
	else
	{
		namesp = props[PropertyKeyConst::NAMESPACE];
	}
}

void NacosNamingService::initEndpoint(Properties &properties)
{
    //TODO:implement initEndpoint

}

NacosNamingService::NacosNamingService(Properties &props)
{
	serverList = props[PropertyKeyConst::SERVER_ADDR];

	initNamespace(props);
	initEndpoint(props);
	httpCli = new HTTPCli();
	serverProxy = new NamingProxy(httpCli, namesp, endpoint, serverList);
	beatReactor = new BeatReactor(serverProxy);
	beatReactor->start();
}

NacosNamingService::~NacosNamingService()
{
	if (beatReactor != NULL)
	{
		delete beatReactor;
	}
	beatReactor = NULL;


	if (serverProxy != NULL)
	{
		delete serverProxy;
	}
	serverProxy = NULL;


	if (httpCli != NULL)
	{
		delete httpCli;
	}
	httpCli = NULL;
}

void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	const NacosString &ip,
	int port
) throw (NacosException)
{
	registerInstance(serviceName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	const NacosString &ip,
	int port
) throw (NacosException)
{
	registerInstance(serviceName, groupName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	const NacosString &ip,
	int port,
	const NacosString &clusterName
) throw (NacosException)
{
	registerInstance(serviceName, Constants::DEFAULT_GROUP, ip, port, clusterName);
}

void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	const NacosString &ip,
	int port,
	const NacosString &clusterName
) throw (NacosException)
{
	Instance instance;
	instance.ip = ip;
	instance.port = port;
	instance.weight = 1.0D;
	instance.clusterName = clusterName;
	
	registerInstance(serviceName, groupName, instance);
}
void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	Instance &instance
) throw (NacosException)
{
	registerInstance(serviceName, Constants::DEFAULT_GROUP, instance);
}

void NacosNamingService::registerInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	Instance &instance
) throw (NacosException)
{

	if (instance.ephemeral)
	{
		BeatInfo beatInfo;
		beatInfo.serviceName = NamingUtils::getGroupedName(serviceName, groupName);
		beatInfo.ip = instance.ip;
		beatInfo.port =instance.port;
		beatInfo.cluster = instance.clusterName;
		beatInfo.weight = instance.weight;
		beatInfo.metadata = instance.metadata;
		beatInfo.scheduled = false;

		beatReactor->addBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), beatInfo);
	}

	serverProxy->registerService(NamingUtils::getGroupedName(serviceName, groupName), groupName, instance);
}

void NacosNamingService::deregisterInstance
(
	const NacosString &serviceName,
	const NacosString &ip,
	int port
) throw (NacosException)
{
	deregisterInstance(serviceName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::deregisterInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	const NacosString &ip,
	int port
) throw (NacosException)
{
	deregisterInstance(serviceName, groupName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::deregisterInstance
(
	const NacosString &serviceName,
	const NacosString &ip,
	int port,
	const NacosString &clusterName
) throw (NacosException)
{
	deregisterInstance(serviceName, Constants::DEFAULT_GROUP, ip, port, clusterName);
}

void NacosNamingService::deregisterInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	const NacosString &ip,
	int port,
	const NacosString &clusterName
) throw (NacosException)
{
	Instance instance;
	instance.ip = ip;
	instance.port = port;
	instance.clusterName = clusterName;

	deregisterInstance(serviceName, groupName, instance);
}

void NacosNamingService::deregisterInstance
(
	const NacosString &serviceName,
	const NacosString &groupName,
	Instance &instance
) throw (NacosException)
{
	beatReactor->removeBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), instance.ip, instance.port);
	serverProxy->deregisterService(NamingUtils::getGroupedName(serviceName, groupName), instance);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName
) throw (NacosException)
{
	list<NacosString> clusters;
	return getAllInstances(serviceName, clusters);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	const NacosString &groupName
) throw (NacosException)
{
	list<NacosString> clusters;
	return getAllInstances(serviceName, groupName, clusters);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	bool subscribe
) throw (NacosException)
{
	list<NacosString> clusters;
	return getAllInstances(serviceName, clusters, subscribe);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	const NacosString &groupName,
	bool subscribe
) throw (NacosException)
{
	list<NacosString> clusters;
	return getAllInstances(serviceName, groupName, clusters, subscribe);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	list<NacosString> clusters
) throw (NacosException)
{
	return getAllInstances(serviceName, clusters, true);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	const NacosString &groupName,
	list<NacosString> clusters
) throw (NacosException)
{
	return getAllInstances(serviceName, groupName, clusters, true);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	list<NacosString> clusters,
	bool subscribe
) throw (NacosException)
{
	return getAllInstances(serviceName, Constants::DEFAULT_GROUP, clusters, subscribe);
}

list<Instance> NacosNamingService::getAllInstances
(
	const NacosString &serviceName,
	const NacosString &groupName,
	list<NacosString> clusters,
	bool subscribe
) throw (NacosException)
{
	ServiceInfo serviceInfo;
	//TODO:
	/*if (subscribe) {
		serviceInfo = hostReactor.getServiceInfo(NamingUtils::getGroupedName(serviceName, groupName), ParamUtils::Implode(clusters));
	} else {
		serviceInfo = hostReactor.getServiceInfoDirectlyFromServer(NamingUtils::getGroupedName(serviceName, groupName), ParamUtils::Implode(clusters));
	}*/
	NacosString clusterString = ParamUtils::Implode(clusters);
	NacosString result = serverProxy->queryList(serviceName, clusterString, 0/*What should be filled in UDPPort??*/, false);
	serviceInfo = JSON::JsonStr2ServiceInfo(result);
	list<Instance> hostlist = serviceInfo.getHosts();
	return hostlist;
}

