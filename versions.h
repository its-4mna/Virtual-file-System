#ifndef VERSIONRECORD_H
#define VERSIONRECORD_H

#include "Strings.h"
#include <ctime>
#include <iostream>

class versions
{
private:
    String oldContent;
    time_t modifiedTime;
    String modifyingUser;
    char timeString[100];
    int hour;
    int minute;
    int second;

public:
    versions(const String& content,time_t t,const String& user);
    versions(const versions& other);
    versions& operator=(const versions& other);
    ~versions();
    const String& getOldContent() const;
    time_t getModifiedTime() const;
    const String& getModifyingUser() const;
    void print(int index) const;
};

#endif