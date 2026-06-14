#ifndef FILESYSTEMENTITY_H
#define FILESYSTEMENTITY_H

#include "String.h"
#include "user.h"
#include "permissions.h"
#include "attribute.h"
#include <ctime>
#include <iostream>

class sysEntity
{
protected:
    String name;
    time_t creationTime;
    time_t modifiedTime;
    int xattrCount;
    int xattrCapacity;
    static int totalEntitiesCreated;
    void expandXattrs();
    User* owner;
    permissions perms;
    sysEntity* parent;
    Attribute** xattrs;

public:
    sysEntity(const String& n, User* o, const permissions& p);
    sysEntity(const sysEntity& other);
    sysEntity& operator=(const sysEntity& other);
    virtual ~sysEntity();

    void setName(const String& n);
    void setOwner(User* u);
    void setParent(sysEntity* p);
    void touchModified();

    const String& getName()const;
    time_t getCreationTime()const;
    time_t getModifiedTime()const;
    User* getOwner()const;
    const permissions& getPermissions() const;
    permissions& getPermissions();
    sysEntity* getParent() const;
    static int getTotalEntitiesCreated();

    void addXattr(const String& key, const String& value);
    String getXattr(const String& key) const;
    void printXattrs() const;

    virtual int getSize() const = 0;
    virtual sysEntity* clone() const = 0;
    virtual void print(int depth) const = 0;
    virtual String getType()  const = 0;

    void printHeader(int depth) const;
    bool checkPermission(const User* user, int operation) const;

};

#endif