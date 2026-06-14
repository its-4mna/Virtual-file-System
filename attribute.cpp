#include "attribute.h"

Attribute::Attribute(const String& k,const String& v)
{
    key = k;
    value = v;
}


Attribute& Attribute::operator=(const Attribute& other)
{
    if(this!=&other)
    {
        key = other.key;
        value =other.value;
    }
    return *this;
}

Attribute::~Attribute()
{
    //what
}


void Attribute::setValue(const String& v)
{
    value=v;
}

const String& Attribute::getKey()const
{
    return key;
}
const String& Attribute::getValue()const
{
    return value;
}

void Attribute::displayAttributes()const
{
    std::cout<<"\nAttribute: "<<key<<" = "<<value<<"\n";
}


Attribute::Attribute(const Attribute& copy)
{
    key =copy.key;
    value = copy.value; 
}