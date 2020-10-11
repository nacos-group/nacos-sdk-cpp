#include <stdlib.h>
#include "Debug.h"

using namespace std;
using namespace nacos;

bool testDebug() {
    log_print(DEBUG, "print\n");
    log_debug("debug\n");
    log_info("info\n");
    log_warn("warn\n");
    log_error("error\n");
    return true;
}

bool testVaArgs() {
    //log_print(DEBUG, "print\n","print","print");
    log_debug("debug\n", "debug", "debug");
    log_info("info\n", "info", "info");
    log_warn("warn\n", "warn", "warn");
    log_error("error\n", "error", "error");
    return true;
}

bool testlogPrint() {
    log_print(DEBUG, "===>print %s %s %d\n", "print", "print", 9);
    log_print(DEBUG, "===>print %s %s %d\n", "print", "print", 9);
    log_print(DEBUG, "===>print %s %s %d\n", "print", "print", 9);
    return true;
}

bool testVaArgs2() {
    log_info("info %s %s\n", "info", "info");
    log_print(DEBUG, "print %s %s %d\n", "print", "print", 9);
    log_debug("debug %s %s\n", "debug", "debug");
    log_info("info %s %s\n", "info", "info");
    log_warn("warn %s %s\n", "warn", "warn");
    log_error("error %s %s %d %f\n", "error", "error", 999, 3.14);
    return true;
}