#include "sysEntity.h"

int sysEntity::totalEntitiesCreated = 0;

void sysEntity::expandXattrs()
{
    int newCap = xattrCapacity * 2;
    if (xattrCapacity == 0) newCap = 4;
    Attribute** newArr = new Attribute*[newCap];
    for (int i = 0; i < xattrCount; i++)
        newArr[i] = xattrs[i];
    delete[] xattrs;
    xattrs = newArr;
    xattrCapacity = newCap;
}

sysEntity::sysEntity(const String& n, User* o, const permissions& p)
{
    name = n;
    owner = o;
    perms = p;
    parent = 0;
    xattrs = 0;
    xattrCount = 0;
    xattrCapacity = 0;

    creationTime = time(0);
    modifiedTime = creationTime;
    totalEntitiesCreated++;
}

sysEntity::sysEntity(const sysEntity& other)
{
    name = other.name;
    creationTime = other.creationTime;
    modifiedTime = other.modifiedTime;
    owner = other.owner;
    perms = other.perms;
    parent =0;
    xattrs = 0;
    xattrCount = 0;
    xattrCapacity = 0;

    for (int i = 0; i < other.xattrCount; i++)
        addXattr(other.xattrs[i]->getKey(), other.xattrs[i]->getValue());
    totalEntitiesCreated++;
}

sysEntity& sysEntity::operator=(const sysEntity& other)
{
    if (this != &other)
    {
        for (int i = 0; i < xattrCount; i++)
            delete xattrs[i];
        delete[] xattrs;

        name = other.name;
        creationTime = other.creationTime;
        modifiedTime = other.modifiedTime;
        owner = other.owner;
        perms = other.perms;
        parent = other.parent;  
        xattrCount = 0;
        xattrCapacity = 0;
        xattrs = 0;

        for (int i = 0; i < other.xattrCount; i++)
            addXattr(other.xattrs[i]->getKey(), other.xattrs[i]->getValue());
    }
    return *this;
}

sysEntity::~sysEntity()
{
    for (int i = 0; i < xattrCount; i++)
        delete xattrs[i];
    delete[] xattrs;
    totalEntitiesCreated--;
}

const String& sysEntity::getName() const 
{ 
    return name; 
}

time_t sysEntity::getCreationTime() const 
{ 
    return creationTime; 
}

time_t sysEntity::getModifiedTime() const 
{ 
    return modifiedTime; 
}

User* sysEntity::getOwner() const 
{ 
    return owner; 
}

const permissions& sysEntity::getPermissions() const 
{ 
    return perms; 
}

permissions& sysEntity::getPermissions() 
{ 
    return perms; 
}

sysEntity* sysEntity::getParent() const 
{ 
    return parent; 
}

void sysEntity::setName(const String& n) 
{ 
    name = n; 
}

void sysEntity::setOwner(User* u) 
{ 
    owner = u; 
}

void sysEntity::setParent(sysEntity* p) 
{ 
    parent = p; 
}

void sysEntity::touchModified() 
{ 
    modifiedTime = time(0); 
}

void sysEntity::addXattr(const String& key, const String& value)
{
    for (int i = 0; i < xattrCount; i++)
    {
        if (xattrs[i]->getKey() == key)
        {
            xattrs[i]->setValue(value);
            return;
        }
    }
    if (xattrCount == xattrCapacity) 
        expandXattrs();
    xattrs[xattrCount] = new Attribute(key, value);  
    xattrCount++;  
}

String sysEntity::getXattr(const String& key) const
{
    for (int i = 0; i < xattrCount; i++)
        if (xattrs[i]->getKey() == key)
            return xattrs[i]->getValue();
    return String("");  
}

void sysEntity::printXattrs() const
{
    for (int i = 0; i < xattrCount; i++)
        xattrs[i]->displayAttributes();
}

bool sysEntity::checkPermission(const User* user, int operation) const
{
    if (user == 0) 
        return false;
    if (user->isRoot()) 
        return true;

    bool isOwner = (owner->getUserID() == user->getUserID());  
    bool sameGroup = (user->getGroupName() == owner->getGroupName()); 

    if (isOwner)
    {
        if (operation == 0) 
            return perms.canOwnerRead();
        if (operation == 1) 
            return perms.canOwnerWrite();
        if (operation == 2) 
            return perms.canOwnerExecute();
    }
    else if (sameGroup)
    {
        if (operation == 0) 
            return perms.canGroupRead();
        if (operation == 1) 
            return perms.canGroupWrite();
        if (operation == 2) 
            return perms.canGroupExecute();
    }
    else
    {
        if (operation == 0) 
            return perms.canOthersRead();
        if (operation == 1) 
            return perms.canOthersWrite();
        if (operation == 2) 
            return perms.canOthersExecute();
    }
    return false;
}

void sysEntity::printHeader(int depth) const
{
    for (int i = 0; i < depth; i++) 
        std::cout << "  ";
    std::cout << "\nType: " << getType() << name;
    std::cout << "\nOwner: " << owner->getUsername(); 
    std::cout << "\nPerms: ";
    perms.displayPerms();
    std::cout << "\nSize: " << getSize();
}

int sysEntity::getTotalEntitiesCreated()
{
    return totalEntitiesCreated;
}