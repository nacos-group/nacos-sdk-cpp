#ifndef __LISTENINGDATA_H_
#define __LISTENINGDATA_H_
#include <map>
#include "NacosString.h"
#include "listen/Listener.h"
#include "md5/md5.h"

class ListeningData
{
private:
	NacosString tenant;
	NacosString dataId;
	NacosString group;
	NacosString dataMD5;
	std::map<Listener*, char> listenerList;
public:
    NacosString getDataId() const
    {
        return dataId;
    }
    NacosString getTenant() const
    {
        return tenant;
    }
    NacosString getGroup() const
    {
        return group;
    }
    NacosString getMD5() const
    {
        return dataMD5;
    }
    void setMD5(const NacosString &md5)
    {
        dataMD5 = md5;
    }
    /**
    * Adds a listener to this object being listened for further notification
    * @param listener the listener to be added
    * @return true if added successfully
    *         false if the listener is already in the list
    */
    bool addListener(Listener *listener)
    {
        if (listenerList.find(listener) != listenerList.end())
        {
            return false;
        }

        listenerList[listener] = 1;
        return true;
    }

    /**
    * Removes a listener from this object being listened
    * @param listener the listener to be removed
    * @return true if removed successfully
    *         false if the listener does not exist in the list
    */
    bool removeListener(Listener *listener)
    {
        if (listenerList.find(listener) == listenerList.end())
        {
            return false;
        }

        listenerList.erase(listener);
        return true;
    }

    bool isEmpty() const
    {
        return listenerList.size() == 0;
    }

    ListeningData(const NacosString &tenant, const NacosString &dataId, const NacosString &group, const NacosString &cfgcontent)
    {
        setContent(tenant, dataId, group, cfgcontent);
    }

    void setContent(const NacosString &tenant, const NacosString &dataId, const NacosString &group, const NacosString &cfgcontent)
    {
        this->tenant = tenant;
        this->dataId = dataId;
        this->group = group;
        if (!isNull(cfgcontent))
        {
            MD5 md5;//TODO:optimize this to a static object to avoid repeatedly ctor&dtor of object
            md5.update(cfgcontent);
            this->dataMD5 = md5.toString();
        }
        else
        {
            this->dataMD5 = "";
        }
    }
	bool operator == (const ListeningData &rhs) const
	{
		return tenant == rhs.tenant && dataId == rhs.dataId && group == rhs.group;
	};

    const std::map<Listener*, char> *getListenerList() const
    {
        return &this->listenerList;
    }
};

#endif