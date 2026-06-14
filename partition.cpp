#include "partition.h"
#include "mountPoint.h"
#include "permissions.h"   // for Permission
#include <iostream>

Partition::Partition(const String& lbl, int capMB, const String& fs, storageMedium* medium, User* rootUser)
{
    label = lbl;
    allocatedCapacityMB = capMB;
    fsType = fs;
    parentMedium = medium;
    mounted = false;
    usedSpaceMB = 0;

    permissions rootPerm(true, true, true, true, false, true, true, false, true);
    rootDir = new directoy(String("/"), rootUser, rootPerm);
}

Partition::Partition(const Partition& other)
{
    rootDir = new directoy(*other.rootDir);
    label = other.label;
    fsType = other.fsType;
    allocatedCapacityMB = other.allocatedCapacityMB;
    parentMedium = other.parentMedium;
    usedSpaceMB = other.usedSpaceMB;
    mounted = other.mounted;
}

Partition& Partition::operator=(const Partition& other)
{
    if (this != &other)
    {
        label = other.label;
        allocatedCapacityMB = other.allocatedCapacityMB;
        usedSpaceMB = other.usedSpaceMB;
        fsType = other.fsType;
        parentMedium = other.parentMedium;
        mounted = other.mounted;

        delete rootDir;

        rootDir = new directoy(*other.rootDir);  
    }
    return *this;
}


bool Partition::hasCapacity(int sizeBytes) const
{
    int sizeMB = (sizeBytes + 1023) / 1024;  
    return (usedSpaceMB + sizeMB) <= allocatedCapacityMB;
}

bool Partition::consumeSpace(int sizeBytes)
{
    int sizeMB = (sizeBytes + 1023) / 1024;
    if (usedSpaceMB + sizeMB > allocatedCapacityMB)
    {
        std::cout << "\nerror: Partition " << label<< " is full" ;
        std::cout << "\nAvailable: " << (allocatedCapacityMB - usedSpaceMB);
        return false;
    }
    usedSpaceMB += sizeMB;
    return true;
}

void Partition::releaseSpace(int sizeBytes)
{
    int sizeMB = (sizeBytes + 1023) / 1024;
    usedSpaceMB -= sizeMB;
    if (usedSpaceMB < 0) usedSpaceMB = 0;
}

void Partition::recalcUsedSpace()
{
    int totalBytes = rootDir->getSize();
    usedSpaceMB = (totalBytes + 1023) / 1024; 
}

const String& Partition::getLabel() const 
{ 
    return label; 
}

int Partition::getAllocatedCapacityMB() const 
{ 
    return allocatedCapacityMB; }

int Partition::getUsedSpaceMB() const 
{   
    return usedSpaceMB; }

int Partition::getAvailableSpaceMB() const 
{   
    return allocatedCapacityMB - usedSpaceMB; 
}

const String& Partition::getFsType() const 
{   
    return fsType; 
}

directoy* Partition::getRootDir() const 
{   
    return rootDir; } 

storageMedium* Partition::getParentMedium() const 
{   
    return parentMedium; }

bool Partition::isMounted() const { return mounted; }

void Partition::setMounted(bool m) { mounted = m; }

void Partition::print() const
{
    std::cout << "\nPartition: " << label;
    std::cout << "\nFS: " << fsType;
    std:: cout<< "\nCapacity: " << allocatedCapacityMB << "MB";
    std::cout<< "\nUsed: " << usedSpaceMB << "MB";
    std::cout << "\nAvailable: " << getAvailableSpaceMB() << "MB";
    std::cout << "\nMounted: " << (mounted ? "yes" : "no");
}

Partition::~Partition()
{
    delete rootDir;
}
/*
bool MountPoint::mountPartition(Partition* p)
{
    if (p == 0)
    {
        std::cout << "\nerror: Cannot mount null partition";
        return false;
    }
    if (mountedPartition != 0)
    {
        std::cout << "\nerror: Mount point " << name << " already has a partition mounted";
        return false;
    }
    if (p->isMounted())
    {
        std::cout << "\nerror: Partition " << p->getLabel() << " is already mounted elsewhere";
        return false;
    }
    mountedPartition = p;
    p->setMounted(true);
    std::cout << "\nPartition " << p->getLabel() << " mounted at " << name;
    return true;
}
    */
