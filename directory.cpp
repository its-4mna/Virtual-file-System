#include "directory.h"
#include <iostream>

directoy::directoy(const String& n, User* o, const permissions& p): sysEntity(n, o, p)
{
    children = 0;
    childCapacity = 0;
    childCount = 0;
}

directoy::directoy(const directoy& other): sysEntity(other)
{
    children = 0;
    childCapacity = 0;
    childCount = 0;
    for (int i = 0; i < other.childCount; i++)
    {
        sysEntity* copy = other.children[i]->clone();
        copy->setParent(this);
        if (childCount == childCapacity) expandChildren();
        children[childCount++] = copy;
    }
}


sysEntity* directoy::findChild(const String& childName) const
{
    for (int i = 0; i < childCount; i++)
        if (children[i]->getName() == childName)
            return children[i];
    return 0;
}

void directoy::expandChildren()    //if full
{
    int cap;
    if(childCapacity == 0)
        cap = 8;
    else
        cap = childCapacity*2;

    sysEntity** newArr = new sysEntity*[cap];

    for (int i = 0; i < childCount; i++)
        newArr[i] = children[i];

    delete[] children;
    children = newArr;
    childCapacity = cap;
}

directoy& directoy::operator=(const directoy& other)
{
    if (this != &other)
    {
        sysEntity::operator=(other);

        for (int i = 0; i < childCount; i++)
            delete children[i];
        delete[] children;

        children = 0; 
        childCount = 0; 
        childCapacity = 0;

        for (int i = 0; i < other.childCount; i++)
        {
            sysEntity* copy = other.children[i]->clone();
            copy->setParent(this);

            if (childCount == childCapacity) 
                expandChildren();

            children[childCount++] = copy;
        }
    }
    return *this;
}

directoy::~directoy()
{
    for (int i = 0; i < childCount; i++)
        delete children[i];
    delete[] children;
}
int directoy::getChildCount() const 
{ 
    return childCount; 
}

sysEntity* directoy::getChild(int i) const
{
    if (i < 0 || i >= childCount) 
        return 0;
        
    return children[i];
}

bool directoy::addChild(sysEntity* entity)
{
    if (entity == 0) 
        return false;
    for (int i = 0; i < childCount; i++)
    {
        if (children[i]->getName() == entity->getName())
        {
            std::cout << "\nError: Entity " << entity->getName() << " already exists in directory " << name;
            return false;
        }
    }
    entity->setParent(this);
    if (childCount == childCapacity) 
        expandChildren();

    children[childCount++] = entity;
    touchModified();

    return true;
}


sysEntity* directoy::removeChild(const String& childName)
{
    for (int i = 0; i < childCount; i++)
    {
        if (children[i]->getName() == childName)
        {
            sysEntity* removed = children[i];
            for (int j = i; j < childCount - 1; j++)
                children[j] = children[j + 1];

            childCount--;
            removed->setParent(0);
            touchModified();
            return removed;
        }
    }
    return 0;
}
int directoy::getSize() const
{
    int total = 0;
    for (int i = 0; i < childCount; i++)
        total += children[i]->getSize();
    return total;
}

bool directoy::deleteChild(const String& childName, User* requestingUser)
{
    for (int i = 0; i < childCount; i++)
    {
        if (children[i]->getName() == childName)
        {
            if (!checkPermission(requestingUser, 1))
            {
                //if user doesnt even exist OR if they dont have permisison dono k liye deny
                std::cout << "\nPERMISSION DENIED for user "<< (requestingUser ? requestingUser->getUsername() : String("?")) << " cannot delete from directoy " << name;
                return false;
            }
            delete children[i];
            for (int j = i; j < childCount - 1; j++)
                children[j] = children[j + 1];

            childCount--;
            touchModified();
            return true;
        }
    }
    //incase not found
    std::cout << "\nError: " << childName << " not found in " << name;
    return false;
}



String directoy::getType() const { return String("directoy"); }

void directoy::listShallow(User* requestingUser) const
{
    if (!checkPermission(requestingUser, 0))
    {
        std::cout << "\nPERMISSION DENIED: Cannot read directoy "<< name;
        return;
    }
    std::cout << "=== Shallow listing of " << name <<" ("<< childCount << " items) ===";
    for (int i = 0; i < childCount; i++)
    {
        sysEntity* child = children[i];
        std::cout << "\nType: " << child->getType(); 
        std::cout << "\nName: " << child->getName();
        std::cout << "\nSize: " << child->getSize();
        std::cout  << "\nOwner: " << (child->getOwner()? child->getOwner()->getUsername(): String("none"));
        std::cout << "\nPerms: ";
        child->getPermissions().displayPerms();
    }
}

void directoy::listDeep(User* requestingUser, int depth) const
{
    if (!checkPermission(requestingUser, 0))
    {
        for (int i = 0; i < depth; i++) 
            std::cout << "  ";
        std::cout << "\nPERMISSION DENIED: Cannot read " << name;
        return;
    }
    print(depth);
    for (int i = 0; i < childCount; i++)
    {
        directoy* subDir = dynamic_cast<directoy*>(children[i]);
        if (subDir)
            subDir->listDeep(requestingUser, depth + 1);
        else
            children[i]->print(depth + 1);
    }
}

void directoy::print(int depth) const
{
    printHeader(depth);
    if (xattrCount > 0) 
        printXattrs();
}

sysEntity* directoy::clone() const
{
    return new directoy(*this);
}

void directoy::search(const String& pattern, sysEntity**& results, int& count, int& capacity) const
{
    for (int i = 0; i < childCount; i++)
    {
        if (children[i]->getName().contains(pattern))
        {
            if (count == capacity)
            {
                int newCap = (capacity == 0) ? 8 : capacity * 2;    //increasing cap
                sysEntity** newArr = new sysEntity*[newCap];

                for (int j = 0; j < count; j++) 
                    newArr[j] = results[j];

                delete[] results;
                results = newArr;
                capacity = newCap;
            }
            results[count++] = children[i];
        }
        directoy* subDir = dynamic_cast<directoy*>(children[i]);
        if (subDir) subDir->search(pattern, results, count, capacity);
    }
}

void directoy::findDanglingLinks(SymbolicLink**& results, int& count, int& capacity) const
{
    for (int i = 0; i < childCount; i++)
    {
        SymbolicLink* sl = dynamic_cast<SymbolicLink*>(children[i]);
        if (sl && sl->isDangling())
        {
            if (count == capacity)
            {
                int newCap = (capacity == 0) ? 4 : capacity * 2;
                SymbolicLink** newArr = new SymbolicLink*[newCap];

                for (int j = 0; j < count; j++) 
                    newArr[j] = results[j];
                delete[] results;

                results = newArr;
                capacity = newCap;
            }
            results[count++] = sl;
        }
        directoy* subDir = dynamic_cast<directoy*>(children[i]);

        if (subDir) 
            subDir->findDanglingLinks(results, count, capacity);
    }
}