#include "volume.h"
#include "storageMedium.h"
#include "permissions.h"
#include "versions.h"
#include "partition.h"
#include <iostream>

void Volume::expandMedia()
{
    int newCap=(mediaCapacity==0)?4:mediaCapacity*2;

    storageMedium** newArr=new storageMedium*[newCap];
    for(int i=0;i<mediaCount;i++) 
        newArr[i]=media[i];
    delete[] media;
    media=newArr;
    mediaCapacity=newCap;
}

void Volume::expandUsers()
{
    int newCap=(userCapacity==0)?4:userCapacity*2;
    User** newArr=new User*[newCap];
    for(int i=0;i<userCount;i++) 
        newArr[i]=users[i];
    delete[] users;
    users=newArr;
    userCapacity=newCap;
}

void Volume::expandMounted()
{
    int newCap=(mountedCapacity==0)?4:mountedCapacity*2;

    Partition** newArr=new Partition*[newCap];
    for(int i=0;i<mountedCount;i++) 
        newArr[i]=mountedPartitions[i];
    delete[] mountedPartitions;

    mountedPartitions=newArr;
    mountedCapacity=newCap;
}

Volume::Volume(const String& n)
{
    name=n;
    media=0;
    mediaCount=0;
    mediaCapacity=0;
    users=0;
    userCount=0;
    userCapacity=0;
    primaryPartition=0;
    mountedPartitions=0;
    mountedCount=0;
    mountedCapacity=0;
}

Volume::~Volume()
{
    for(int i=0;i<mediaCount;i++) 
        delete media[i];
    for(int i=0;i<userCount;i++) 
        delete users[i];
    delete[] media;
    delete[] users;
    delete[] mountedPartitions;
}

storageMedium* Volume::addMedium(int totalCapMB,int type)
{
    storageMedium* m=new storageMedium(totalCapMB,type);
    m->setMounted(true);
    if(mediaCount==mediaCapacity) 
        expandMedia();
    media[mediaCount++]=m;
    return m;
}

User* Volume::registerUser(const String& uname,int uid,int role,const String& home,const String& group)
{
    for(int i=0;i<userCount;i++) if(users[i]->getUserID()==uid) 
    { 
        std::cout<<"\nError: User ID "<<uid<<" already exists"; 
        return 0; 
    }
    User* u=new User(uname,uid,role,home,group);
    if(userCount==userCapacity)
        expandUsers();
    users[userCount++]=u;
    return u;
}

User* Volume::findUser(const String& uname)const
{
    for(int i=0;i<userCount;i++) 
        if(users[i]->getUsername()==uname) 
            return users[i];
    return 0;
}

User* Volume::findUserByID(int uid)const
{
    for(int i=0;i<userCount;i++) 
        if(users[i]->getUserID()==uid) 
            return users[i];
    return 0;
}

void Volume::setPrimaryPartition(Partition* p) 
{ 
    primaryPartition=p; 
}

Partition* Volume::getPrimaryPartition()const 
{ 
    return primaryPartition; }

void Volume::registerMount(Partition* p)
{
    for(int i=0;i<mountedCount;i++) 
        if(mountedPartitions[i]==p) 
            return;
    if(mountedCount==mountedCapacity) 
        expandMounted();

    mountedPartitions[mountedCount++]=p;
}

regularFile* Volume::createFile(directoy* dir,const String& fname,User* requester,const permissions& perm,const String& content,Partition* partition)
{
    if(!dir->checkPermission(requester,1)) 
    { 
        std::cout<<"\nPERMISSION DENIED: User "<<requester->getUsername()<<" cannot write to directory '"<<dir->getName()<<"'.\n"; 
        return 0; 
    }

    if(partition&&!partition->hasCapacity(content.getlen())) 
    { 
        std::cout<<"  [CAPACITY ERROR] Not enough space on partition '"<<partition->getLabel()<<"' to create file '"<<fname<<"'.\n    Available: "<<partition->getAvailableSpaceMB()<<"MB, Needed: ~"<<((content.getlen()+1023)/1024)<<"MB\n"; 
        return 0; 
    }
    regularFile* f=new regularFile(fname,requester,perm,content);
    if(!dir->addChild(f)) { delete f; return 0; }
    if(partition) partition->consumeSpace(content.getlen());
    std::cout<<"\nFile '"<<fname<<"' created by '"<<requester->getUsername()<<"'.\n";
    return f;
}

directoy* Volume::createdirectoy(directoy* parent,const String& dname,User* requester,const permissions& perm)
{
    if(!parent->checkPermission(requester,1)) { std::cout<<"  [PERMISSION DENIED] User '"<<requester->getUsername()<<"' cannot write to directory '"<<parent->getName()<<"'.\n"; return 0; }
    directoy* d=new directoy(dname,requester,perm);
    if(!parent->addChild(d)) { delete d; return 0; }
    std::cout<<"  [INFO] directoy '"<<dname<<"' created by '"<<requester->getUsername()<<"'.\n";
    return d;
}

MountPoint* Volume::createMountPoint(directoy* parent,const String& mpname,User* requester,const permissions& perm)
{
    if(!parent->checkPermission(requester,1)) { std::cout<<"  [PERMISSION DENIED] Cannot create mount point.\n"; return 0; }
    MountPoint* mp=new MountPoint(mpname,requester,perm);
    if(!parent->addChild(mp)) { delete mp; return 0; }
    std::cout<<"  [INFO] MountPoint '"<<mpname<<"' created.\n";
    return mp;
}

SymbolicLink* Volume::createSymLink(directoy* dir,const String& lname,User* requester,const permissions& perm,sysEntity* target,const String& targetPath)
{
    if(!dir->checkPermission(requester,1)) { std::cout<<"  [PERMISSION DENIED] User '"<<requester->getUsername()<<"' cannot create symlink in '"<<dir->getName()<<"'.\n"; return 0; }
    SymbolicLink* sl=new SymbolicLink(lname,requester,perm,target,targetPath);
    if(!dir->addChild(sl)) { delete sl; return 0; }
    std::cout<<"  [INFO] SymLink '"<<lname<<"' -> '"<<targetPath<<"' created.\n";
    return sl;
}

void Volume::readFile(regularFile* f,User* requester)const
{
    if(!f->checkPermission(requester,0)) { std::cout<<"  [PERMISSION DENIED] User '"<<requester->getUsername()<<"' cannot read file '"<<f->getName()<<"'.\n"; return; }
    std::cout<<"  [READ] File '"<<f->getName()<<"' content:\n  \""<<f->getContent()<<"\"\n";
}

bool Volume::modifyFile(regularFile* f,User* requester,const String& newContent,Partition* partition)
{
    if(!f->checkPermission(requester,1)) 
    { 
        std::cout<<"  [PERMISSION DENIED] User '"<<requester->getUsername()<<"' cannot write file '"<<f->getName()<<"'.\n"; 
        return false; 
    
    }

    int oldSize=f->getSize();
    int newSize=newContent.getlen();
    int delta=newSize-oldSize;

    if(partition&&delta>0&&!partition->hasCapacity(delta)) 
    { 
        std::cout<<"  [CAPACITY ERROR] Not enough space to modify '"<<f->getName()<<"'.\n"; 
        return false; 
    }

    f->setContent(newContent,requester);

    if(partition) 
    { 
        if(delta>0) 
            partition->consumeSpace(delta); 
        else 
            partition->releaseSpace(-delta); 
    }
    
    std::cout<<"  [INFO] File '"<<f->getName()<<"' modified by '"<<requester->getUsername()<<"'.\n";
    return true;
}

bool Volume::moveEntity(directoy* srcDir,const String& entityName,directoy* dstDir,User* requester)
{
    if(!srcDir->checkPermission(requester,1)) { std::cout<<"  [PERMISSION DENIED] Cannot remove from source.\n"; return false; }
    if(!dstDir->checkPermission(requester,1)) { std::cout<<"  [PERMISSION DENIED] Cannot write to destination.\n"; return false; }
    sysEntity* entity=srcDir->removeChild(entityName);
    if(entity==0) { std::cout<<"  [ERROR] '"<<entityName<<"' not found in '"<<srcDir->getName()<<"'.\n"; return false; }
    if(!dstDir->addChild(entity)) { srcDir->addChild(entity); return false; }
    std::cout<<"  [INFO] Moved '"<<entityName<<"' from '"<<srcDir->getName()<<"' to '"<<dstDir->getName()<<"'.\n";
    return true;
}

bool Volume::copyEntity(const String& entityName,directoy* srcDir,directoy* dstDir,User* requester)
{
    sysEntity* entity=srcDir->findChild(entityName);
    if(entity==0) 
    { 
        std::cout<<"  [ERROR] '"<<entityName<<"' not found in '"<<srcDir->getName()<<"'.\n"; 
        return false; 
    }

    if(!entity->checkPermission(requester,0)) 
    { 
        std::cout<<"  [PERMISSION DENIED] Cannot read source entity.\n"; 
        return false; 
    }

    if(!dstDir->checkPermission(requester,1)) 
    { 
        std::cout<<"  [PERMISSION DENIED] Cannot write to destination.\n"; 
        return false; 
    }

    sysEntity* copy=entity->clone();

    copy->setOwner(requester);
    copy->setName(entityName+String("_copy"));

    if(!dstDir->addChild(copy)) 
    { 
        delete copy; 
        return false; 
    }
    std::cout<<"  [INFO] Copied '"<<entityName<<"' as '"<<(entityName+String("_copy"))<<"' into '"<<dstDir->getName()<<"'.\n";
    return true;
}

void Volume::searchVolume(const String& pattern,User* requester)const
{
    if(primaryPartition==0) 
    { 
        std::cout<<"  [ERROR] No primary partition set.\n"; 
        return; 
    }

    std::cout<<"\n=== Recursive Search for \""<<pattern<<"\" across volume ===\n";

    sysEntity** results=0;
    int count=0,capacity=0;
    primaryPartition->getRootDir()->search(pattern,results,count,capacity);

    if(count==0) 
        std::cout<<"  (no matches found)\n";

    for(int i=0;i<count;i++) 
        std::cout<<"  Found: ["<<results[i]->getType()<<"] "<<results[i]->getName()<<" | Owner: "<<(results[i]->getOwner()?results[i]->getOwner()->getUsername():String("none"))<<"\n";
    delete[] results;
}

void Volume::detectDanglingLinks()const
{
    if(primaryPartition==0) 
        return;

    std::cout<<"\n=== Dangling Symbolic Link Detection ===\n";
    SymbolicLink** results=0;
    int count=0,capacity=0;
    primaryPartition->getRootDir()->findDanglingLinks(results,count,capacity);

    if(count==0) 
        std::cout<<"  (no dangling links found)\n";
    for(int i=0;i<count;i++) 
        std::cout<<"  DANGLING: [SymLink] '"<<results[i]->getName()<<"' -> target was: '"<<results[i]->getTargetPath()<<"'\n";
    delete[] results;
}

void Volume::printUsers()const
{
    std::cout<<"\n=== User Registry ===\n";
    for(int i=0;i<userCount;i++) 
        users[i]->displayUserInfo();
}

void Volume::printStorageSummary()const
{
    std::cout<<"\n=== Storage Summary ===\n";
    for(int i=0;i<mediaCount;i++) { std::cout<<"Medium "<<(i+1)<<": "; media[i]->print(); }
}
/*


*/
void Volume::printdirectoyTree() const
{
    std::cout << "\n=== File System Tree (Primary Partition) ===\n";
    if (primaryPartition == 0)
    {
        std::cout << "  (no primary partition)\n";
        return;
    }
    User* admin = 0;
    for (int i = 0; i < userCount; i++)
        if (users[i]->isRoot()) { admin = users[i]; break; }
    primaryPartition->getRootDir()->listDeep(admin, 0);
}
void Volume::printVolumeState()const
{
    std::cout<<"\n============================================\n";
    std::cout<<"  VOLUME STATE: "<<name<<"\n";
    std::cout<<"============================================\n";
    printUsers();
    printStorageSummary();
    printdirectoyTree();
}

const String& Volume::getName()const 
{
    return name; 
}