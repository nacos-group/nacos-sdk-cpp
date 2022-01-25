#!/bin/bash

g++ -I/usr/local/include/nacos/ IntegratingIntoYourProject.cpp -lnacos-cli -o IntegratingIntoYourProject.out
g++ -I/usr/local/include/nacos/ getAllInstances.cpp -lnacos-cli -o getAllInstances.out
g++ -I/usr/local/include/nacos/ getConfig.cpp -lnacos-cli -o getConfig.out
g++ -I/usr/local/include/nacos/ listenToKeys.cpp -lnacos-cli -o listenToKeys.out
g++ -I/usr/local/include/nacos/ registerInstances.cpp -lnacos-cli -o registerInstances.out
g++ -I/usr/local/include/nacos/ setConfig.cpp -lnacos-cli -o setConfig.out
g++ -I/usr/local/include/nacos/ subscribeServices.cpp -lnacos-cli -o subscribeServices.out
