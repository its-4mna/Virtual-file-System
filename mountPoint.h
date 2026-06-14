#ifndef MOUNTPOINT_H
#define MOUNTPOINT_H

#include "directory.h"
#include "permissions.h"
#include <iostream>

class Partition;

class MountPoint : public directoy
{
private:
    Partition* mountedPartition;

public:
    MountPoint(const String& n, User* o, const permissions& p);
    MountPoint(const MountPoint& other);
    MountPoint& operator=(const MountPoint& other);
    ~MountPoint();
    String getType() const override;
    void print(int depth) const override;
    sysEntity* clone() const override;
    bool mountPartition(Partition* p);
    void unmountPartition();
    Partition* getMountedPartition() const;  
    bool isMounted() const;

};

#endif