#ifndef PARTITION_H
#define PARTITION_H

#include "Strings.h"
#include "user.h"
#include "directory.h"
#include "storageMedium.h"

class storageMedium;

class Partition
{
private:
    String label;
    int allocatedCapacityMB;
    int usedSpaceMB;
    String fsType;
    directoy* rootDir;
    storageMedium* parentMedium;
    bool mounted;

public:
    Partition(const Partition& other);
    Partition(const String& lbl, int capMB, const String& fs,storageMedium* medium, User* rootUser);
    Partition& operator=(const Partition& other);
    ~Partition();

    bool hasCapacity(int sizeBytes) const;
    bool consumeSpace(int sizeBytes);
    void releaseSpace(int sizeBytes);
    void recalcUsedSpace();

    const String& getLabel() const;
    int getAllocatedCapacityMB() const;
    int getUsedSpaceMB() const;
    int getAvailableSpaceMB() const;
    const String& getFsType() const;
    directoy* getRootDir() const;
    storageMedium* getParentMedium() const;
    bool isMounted() const;

    void setMounted(bool m);
    void print() const;
};

#endif