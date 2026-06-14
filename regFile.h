#ifndef REGULARFILE_H
#define REGULARFILE_H

#include "String.h"
#include "sysEntity.h"
#include "versions.h"

class regularFile : public sysEntity
{
private:
    String content;
    versions** history;
    int historyCount;
    int historyCapacity;
    void expandHistory();

public:
    regularFile(const String& n, User* o, const permissions& p,const String& initialContent = String(""));
    regularFile(const regularFile& other);
    regularFile& operator=(const regularFile& other);
    ~regularFile();

    void print(int depth) const override;
    sysEntity* clone() const override;
    void appendContent(const String& extra, User* modifier);
    void setContent(const String& newContent, User* modifier);
    const String& getContent() const;
    bool restoreVersion(int versionIndex, User* modifier);
    void printHistory() const;
    int getSize() const override;
    int getHistoryCount() const;
    String getType() const override;
    
};

#endif