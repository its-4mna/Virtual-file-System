#include "mountPoint.h"
#include "partition.h"

MountPoint::MountPoint(const String& n, User* o, const permissions& p)
    : directoy(n, o, p), mountedPartition(0)
{
}

MountPoint::MountPoint(const MountPoint& other)
    : directoy(other), mountedPartition(other.mountedPartition)
{
}

MountPoint& MountPoint::operator=(const MountPoint& other)
{
    if (this != &other)
    {
        directoy::operator=(other);
        mountedPartition = other.mountedPartition;
    }
    return *this;
}

MountPoint::~MountPoint()
{
    //does not delete the partition so idk 
}

bool MountPoint::mountPartition(Partition* p)
{
    if (p == 0)
    {
        std::cout << "\nERROR: Cannot mount null partition";
        return false;
    }
    if (mountedPartition != 0)
    {
        std::cout << "\nerror: Mount point '" << name << " already has a partition mounted";
        return false;
    }
    if (p->isMounted())
    {
        std::cout << "\nERROR: Partition '" << p->getLabel() << "' is already mounted elsewhere";
        return false;
    }
    mountedPartition = p;
    p->setMounted(true);
    std::cout << "\nPartition " << p->getLabel() << " mounted at '" << name;
    return true;
}

void MountPoint::unmountPartition()
{
    if (mountedPartition == 0)
    {
        std::cout << "\nNo partition mounted at " << name;
        return;
    }
    std::cout << "\n Unmounted partition from " << name;
    mountedPartition = 0;
}

Partition* MountPoint::getMountedPartition() const
{
    return mountedPartition;
}

bool MountPoint::isMounted() const
{
    return mountedPartition != 0;
}

String MountPoint::getType() const
{
    return String("MountPoint");
}

void MountPoint::print(int depth) const
{
    printHeader(depth);
    for (int i = 0; i < depth; i++) 
        std::cout << "  ";  
    if (isMounted())
        std::cout << "\nMounted partition attached";
    else
        std::cout << "\nNo partition mounted";
}

sysEntity* MountPoint::clone() const
{
    return new MountPoint(*this);
}