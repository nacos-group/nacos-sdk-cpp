#!/bin/bash

if [ ! -f ../libnacos-cli.so ]; then
    echo "Please create the libnacos-cli.so by running make on the project's root directory first.";
    exit;
fi

cp ../libnacos-cli.so .

g++ IntegratingIntoYourProject.cpp -L. -lnacos-cli -I../include -o IntegratingIntoYourProject.out
g++ getAllInstances.cpp -L. -lnacos-cli -I../include -o getAllInstances.out
g++ getConfig.cpp -L. -lnacos-cli -I../include -o getConfig.out
g++ listenToKeys.cpp -L. -lnacos-cli -I../include -o listenToKeys.out
g++ registerInstances.cpp -L. -lnacos-cli -I../include -o registerInstances.out
g++ setConfig.cpp -L. -lnacos-cli -I../include -o setConfig.out
g++ subscribeServices.cpp -L. -lnacos-cli -I../include -o subscribeServices.out
