#include "regFile.h"
#include <iostream>

void regularFile::expandHistory()   //for when we run out of space
{
    int cap;
    if(historyCapacity == 0)
        cap = 4;
    else
        cap = historyCapacity*2;

    versions** newArr = new versions*[cap];

    for (int i = 0; i < historyCount; i++)
        newArr[i] = history[i];

    delete[] history;
    history = newArr;
    historyCapacity = cap;
}

regularFile::regularFile(const String& n, User* o, const permissions& p, const String& initialContent): sysEntity(n, o, p)
{
    content = initialContent;
    history = 0;
    historyCapacity = 0;
    historyCount = 0;
}

regularFile::regularFile(const regularFile& other): sysEntity(other)
{
    content = other.content;
    historyCount = 0;
    historyCapacity = 0;
    history = 0;

    for (int i = 0; i < other.historyCount; i++)
    {
        if (historyCount == historyCapacity) 
            expandHistory();    

        history[historyCount++] = new versions(*other.history[i]);
    }
}

regularFile& regularFile::operator=(const regularFile& other)
{
    if (this != &other)
    {
        sysEntity::operator=(other);
        content = other.content;

        for (int i = 0; i < historyCount; i++)
            delete history[i];
        delete[] history;

        history = 0; historyCount = 0; historyCapacity = 0;

        for (int i = 0; i < other.historyCount; i++)
        {
            if (historyCount == historyCapacity) expandHistory();
            history[historyCount++] = new versions(*other.history[i]);
        }
    }
    return *this;
}

regularFile::~regularFile()
{
    for (int i = 0; i < historyCount; i++)
        delete history[i];
    delete[] history;
}


void regularFile::setContent(const String& newContent, User* modifier)
{
    if (historyCount == historyCapacity) expandHistory();
    history[historyCount++] = new versions(
        content, time(0),
        modifier ? modifier->getUsername() : String("unknown")
    );
    content = newContent;
    touchModified();
}
void regularFile::print(int depth) const
{
    printHeader(depth);
    
    if (xattrCount > 0) 
        printXattrs();
}

sysEntity* regularFile::clone() const
{
    return new regularFile(*this);
}

void regularFile::appendContent(const String& extra, User* modifier)
{
    setContent(content + extra, modifier);
}

bool regularFile::restoreVersion(int versionIndex, User* modifier)
{
    if (versionIndex < 0 || versionIndex >= historyCount)
    {
        std::cout << "\nError: Version " << versionIndex << " does not exist for file '" << name;
        return false;
    }
    setContent(history[versionIndex]->getOldContent(), modifier);
    std::cout << "\nFile '" << name << "' restored to version " << versionIndex;
    return true;
}
const String& regularFile::getContent() const 
{ 
    return content; 
}

void regularFile::printHistory() const
{
    std::cout << "\nVersion history for '" << name;
    if (historyCount == 0)
    {
        std::cout << "\n(no versions recorded)";
        return;
    }
    for (int i = 0; i < historyCount; i++)
        history[i]->print(i);
}
int regularFile::getSize() const 
{ 
    return content.getlen(); 
}

int regularFile::getHistoryCount() const 
{ 
    return historyCount; 
}

String regularFile::getType() const 
{ 
    return String("File"); 
}

