#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "sysEntity.h"
#include "symbolicLink.h"

class directoy : public sysEntity
{
protected:
    int childCapacity;
    sysEntity** children;
    int childCount;
    void expandChildren();

public:
    directoy(const String& n, User* o, const permissions& p);
    directoy& operator=(const directoy& other);
    directoy(const directoy& other);
    virtual ~directoy();

    bool addChild(sysEntity* entity);
    sysEntity* removeChild(const String& childName);
    bool deleteChild(const String& childName, User* requestingUser);
    sysEntity* findChild(const String& childName) const;

    int getSize() const override;
    String getType() const override;
    int getChildCount() const;
    sysEntity* getChild(int i) const;
    void listShallow(User* requestingUser) const;
    void listDeep(User* requestingUser, int depth = 0) const;
    void print(int depth) const override;
    sysEntity* clone() const override;
    void search(const String& pattern, sysEntity**& results, int& count, int& capacity) const;
    void findDanglingLinks(SymbolicLink**& results, int& count, int& capacity) const;
};

#endif