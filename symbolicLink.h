#ifndef SYMBOLICLINK_H
#define SYMBOLICLINK_H

#include "sysEntity.h"

class SymbolicLink : public sysEntity
{
private:
    String targetPath;
    sysEntity* target;
    

public:
    SymbolicLink(const String& n, User* o, const permissions& p, sysEntity* tgt, const String& tgtPath);
    SymbolicLink(const SymbolicLink& other);
    SymbolicLink& operator=(const SymbolicLink& other);
    ~SymbolicLink();

    void setTarget(sysEntity* tgt);
    sysEntity* getTarget() const;     
    const String& getTargetPath() const;
    bool isDangling() const;

    int getSize() const override;
    String getType() const override;
    void print(int depth) const override;
    sysEntity* clone() const override;
};

#endif