#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Strings.h"
#include <iostream>

class Attribute
{
private:
    String key;
    String value;
public:
    Attribute(const String& k,const String& v);
    Attribute(const Attribute& copy);
    Attribute& operator=(const Attribute& copy);
    ~Attribute();
    const String& getKey()const;
    const String& getValue()const;
    void setValue(const String& v);
    void displayAttributes()const;
};

#endif