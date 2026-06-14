#ifndef STORAGEMEDIUM_H
#define STORAGEMEDIUM_H

#include "user.h"
#include <iostream>

class Partition;

class storageMedium
{
public:
    static const int TYPE_HDD=0;
    static const int TYPE_SSD=1;
    static const int TYPE_OPTICAL=2;

private:
    int totalCapacityMB;
    int usedCapacityMB;
    bool mountedState;
    int mediumType;
    Partition** partitions;
    int partitionCount;
    int partitionCapacity;
    void expandPartitions();
    String typeToString()const;

public:
    storageMedium(int totalCapMB,int type);
    storageMedium(const storageMedium& other);
    storageMedium& operator=(const storageMedium& other);
    ~storageMedium();

    Partition* createPartition(const String& label,int capMB,
    const String& fsType,User* rootUser);

    int getTotalCapacityMB()const;
    int getUsedCapacityMB()const;
    int getAvailableMB()const;
    bool isMounted()const;
    int getMediumType()const;
    int getPartitionCount()const;
    Partition* getPartition(int i)const;

    void setMounted(bool m);
    void print()const;
};

#endif