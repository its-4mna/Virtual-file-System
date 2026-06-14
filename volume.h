#ifndef VOLUME_H
#define VOLUME_H

#include "storageMedium.h"
#include "mountPoint.h"
#include "regFile.h"
#include <iostream>

class Volume
{
private:
    String name;
    storageMedium** media;
    int mediaCount;
    int mediaCapacity;
    User** users;
    int userCount;
    int userCapacity;
    Partition* primaryPartition;
    Partition** mountedPartitions;
    int mountedCount;
    int mountedCapacity;

    void expandMedia();
    void expandUsers();
    void expandMounted();

public:
    Volume(const String& n);
    ~Volume();

    storageMedium* addMedium(int totalCapMB,int type);

    User* registerUser(const String& uname,int uid,int role,const String& home,const String& group);
    User* findUser(const String& uname)const;
    User* findUserByID(int uid)const;

    void setPrimaryPartition(Partition* p);
    Partition* getPrimaryPartition()const;

    void registerMount(Partition* p);

    regularFile* createFile(directoy* dir,const String& fname,User* requester,const permissions& perm,const String& content,Partition* partition);
    directoy* createdirectoy(directoy* parent,const String& dname,User* requester,const permissions& perm);
    MountPoint* createMountPoint(directoy* parent,const String& mpname,User* requester,const permissions& perm);
    SymbolicLink* createSymLink(directoy* dir,const String& lname,User* requester,const permissions& perm,sysEntity* target,const String& targetPath);

    void readFile(regularFile* f,User* requester)const;
    bool modifyFile(regularFile* f,User* requester,const String& newContent,Partition* partition);
    bool moveEntity(directoy* srcDir,const String& entityName,directoy* dstDir,User* requester);
    bool copyEntity(const String& entityName,directoy* srcDir,directoy* dstDir,User* requester);

    void searchVolume(const String& pattern,User* requester)const;
    void detectDanglingLinks()const;

    void printUsers()const;
    void printStorageSummary()const;
    void printdirectoyTree()const;
    void printVolumeState()const;

    const String& getName()const;
};

#endif