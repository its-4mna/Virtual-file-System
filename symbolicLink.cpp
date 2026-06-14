#include "symbolicLink.h"
#include <iostream>

SymbolicLink::SymbolicLink(const String& n, User* o, const permissions& p, sysEntity* tgt, const String& tgtPath): sysEntity(n, o, p)
{
    target = tgt;
    targetPath = tgtPath;
}

SymbolicLink::SymbolicLink(const SymbolicLink& other): sysEntity(other)
{
    target = other.target;
    targetPath = other.targetPath;

}


sysEntity* SymbolicLink::clone() const
{
    return new SymbolicLink(*this);
}
SymbolicLink& SymbolicLink::operator=(const SymbolicLink& other)
{
    if (this != &other)
    {
        sysEntity::operator=(other);
        target = other.target;
        targetPath = other.targetPath;
    }
    return *this;
}

SymbolicLink::~SymbolicLink() {}

void SymbolicLink::setTarget(sysEntity* tgt)
{
    target = tgt;
}
bool SymbolicLink::isDangling() const
{
    return target == 0;
}

int SymbolicLink::getSize() const
{
    return targetPath.getlen();  
}

sysEntity* SymbolicLink::getTarget() const
{
    return target;
}

const String& SymbolicLink::getTargetPath() const
{
    return targetPath;
}

String SymbolicLink::getType() const
{
    return String("SymLink");
}

void SymbolicLink::print(int depth) const
{
    printHeader(depth);
    for (int i = 0; i < depth; i++) std::cout << "  ";   
    if (isDangling())
        std::cout << "\nDangling target was: " << targetPath;
    else
        std::cout << "\n" << targetPath << ": "<< target->getType();
}
