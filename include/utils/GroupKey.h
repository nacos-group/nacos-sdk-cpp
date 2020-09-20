#ifndef __GROUP_KEY_H_
#define __GROUP_KEY_H_

#include "NacosString.h"
#include "utils/url.h"

/*
 * Copyright 1999-2018 Alibaba Group Holding Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Synthesize the form of dataId+groupId. Escapes reserved characters in dataId and groupId.
 *
 * @author Nacos
 */
class GroupKey {
public:
    static NacosString getKey(const NacosString &dataId, const NacosString &group) {
        //DataID+GroupId
        return urlencode(dataId) + "+" + urlencode(group);
    }

    static NacosString getKeyTenant(const NacosString &dataId, const NacosString &group, const NacosString &tenant) {
        //DataID+GroupId
        NacosString key = getKey(dataId, group);

        if (!isNull(tenant)) {
            key = key + "+" + urlencode(tenant);
        }
        return key;
    }

    /*static NacosString getKey(const NacosString &dataId, const NacosString &group, const NacosString &datumStr)
    {
        //DataID+GroupId+datumStr
        return urlencode(dataId) + "+" + urlencode(group) + "+" + urlencode(datumStr);
    }*/

    /*static public NacosString[] parseKey(NacosString groupKey) {
        StringBuilder sb = new StringBuilder();
        NacosString dataId = null;
        NacosString group = null;
        NacosString tenant = null;

        for (int i = 0; i < groupKey.length(); ++i) {
            char c = groupKey.charAt(i);
            if ('+' == c) {
                if (null == dataId) {
                    dataId = sb.toString();
                    sb.setLength(0);
                } else if (null == group) {
                    group = sb.toString();
                    sb.setLength(0);
                } else {
                    throw new IllegalArgumentException("invalid groupkey:" + groupKey);
                }
            } else if ('%' == c) {
                char next = groupKey.charAt(++i);
                char nextnext = groupKey.charAt(++i);
                if ('2' == next && 'B' == nextnext) {
                    sb.append('+');
                } else if ('2' == next && '5' == nextnext) {
                    sb.append('%');
                } else {
                    throw new IllegalArgumentException("invalid groupkey:" + groupKey);
                }
            } else {
                sb.append(c);
            }
        }

        if (StringUtils.isBlank(group)) {
            group = sb.toString();
            if (group.length() == 0) {
                throw new IllegalArgumentException("invalid groupkey:" + groupKey);
            }
        } else {
            tenant = sb.toString();
            if (group.length() == 0) {
                throw new IllegalArgumentException("invalid groupkey:" + groupKey);
            }
        }

        return new NacosString[] {dataId, group, tenant};
    }*/

    /**
     * + -> %2B % -> %25
     */
    /*static void urlEncode(NacosString str, StringBuilder sb) {
        for (int idx = 0; idx < str.length(); ++idx) {
            char c = str.charAt(idx);
            if ('+' == c) {
                sb.append("%2B");
            } else if ('%' == c) {
                sb.append("%25");
            } else {
                sb.append(c);
            }
        }
    }*/

};

#endif