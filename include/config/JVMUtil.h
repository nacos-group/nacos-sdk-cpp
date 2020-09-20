#ifndef __JVMUTIL_H_
#define __JVMUTIL_H_

class JVMUtil {
public:
    /**
     * whether is multi instance
     *
     * @return whether multi
     */
    static bool isMultiInstance() {
        return _isMultiInstance;
    };
private:
    static bool _isMultiInstance;

    /*static {
        NacosString multiDeploy = System.getProperty("isMultiInstance", "false");
        if (TRUE.equals(multiDeploy)) {
            isMultiInstance = true;
        }
        LOGGER.info("isMultiInstance:{}", isMultiInstance);
    }*/
};

#endif