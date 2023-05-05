```
cd nacos-sdk-cpp
cmake .
make
```

```
NacosServiceFactory.h
Mutex.h
Tid.h
TimeUtils.h
以上4个头文件需要从src源码中拷贝到`usr/local/include/nacos/`目录，
拷贝位置要和src保持一致，
同时修改以上头文件的的`include`使用相对路径
```

```
g++ -I/usr/local/include/nacos/ IntegratingIntoYourProject.cpp -lnacos-cli -o integrated.out
./integrated.out
```