#include "storageMedium.h"
#include "partition.h"

storageMedium::~storageMedium()
{
    for(int i=0;i<partitionCount;i++)
        delete partitions[i];
    delete[] partitions;
}

void storageMedium::print()const
{
    std::cout<<"\nstorageMedium "<<typeToString();
    std::cout <<"\nTotal: "<<totalCapacityMB<<"MB";
    std::cout <<"\nAllocated: "<<usedCapacityMB<<"MB";
    std::cout <<"\nFree: "<<getAvailableMB()<<"MB";
    std::cout<<"\nMounted: "<<(mountedState?"yes":"no");
    
    for(int i=0;i<partitionCount;i++)
        partitions[i]->print();
}

String storageMedium::typeToString()const
{
    if(mediumType==TYPE_HDD)
        return String("HDD");
    if(mediumType==TYPE_SSD)
        return String("SSD");
    if(mediumType==TYPE_OPTICAL)
        return String("Optical");

    return String("Unknown");
}


void storageMedium::expandPartitions()
{
    int newCap=(partitionCapacity==0)?4:partitionCapacity*2;

    Partition** newArr=new Partition*[newCap];

    for(int i=0;i<partitionCount;i++)
        newArr[i]=partitions[i];
    delete[] partitions;

    partitions=newArr;
    partitionCapacity=newCap;
}


storageMedium::storageMedium(const storageMedium& other)
{
    totalCapacityMB=other.totalCapacityMB;
    usedCapacityMB=other.usedCapacityMB;
    mountedState=other.mountedState;
    mediumType=other.mediumType;
    partitions=0;
    partitionCount=0;
    partitionCapacity=0;
}
storageMedium::storageMedium(int totalCapMB,int type)
{
    totalCapacityMB=totalCapMB;
    usedCapacityMB=0;
    mountedState=false;
    mediumType=type;
    partitions=0;
    partitionCount=0;
    partitionCapacity=0;
}
storageMedium& storageMedium::operator=(const storageMedium& other)
{
    if(this!=&other)
    {
        totalCapacityMB=other.totalCapacityMB;
        usedCapacityMB=other.usedCapacityMB;
        mountedState=other.mountedState;
        mediumType=other.mediumType;
    }
    return *this;
}

Partition* storageMedium::createPartition(const String& label,int capMB, const String& fsType,User* rootUser)
{
    int remainingMB=totalCapacityMB-usedCapacityMB;
    if(capMB>remainingMB)
    {
        std::cout<<"\nError: Cannot create partition "<<label <<" only "<<remainingMB<<"MB available on this medium";
        return 0;
    }
    Partition* p=new Partition(label,capMB,fsType,this,rootUser);

    if(partitionCount==partitionCapacity)
        expandPartitions();

    partitions[partitionCount++]=p;
    usedCapacityMB+=capMB;
    return p;
}

int storageMedium::getAvailableMB()const
{
    return totalCapacityMB-usedCapacityMB;
}

int storageMedium::getUsedCapacityMB()const
{
    return usedCapacityMB;
}

bool storageMedium::isMounted()const
{
    return mountedState;
}
int storageMedium::getPartitionCount()const
{
    return partitionCount;
}
int storageMedium::getTotalCapacityMB()const
{
    return totalCapacityMB;
}

int storageMedium::getMediumType()const
{
    return mediumType;
}

Partition* storageMedium::getPartition(int i)const
{
    if(i<0||i>=partitionCount)return 0;
    return partitions[i];
}

void storageMedium::setMounted(bool m)
{
    mountedState=m;
}
