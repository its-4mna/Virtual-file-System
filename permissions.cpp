#include "permissions.h"

permissions::permissions()
{
    ownerR = true;
    ownerW = true;
    ownerE = true;

    groupE = false;
    groupR = true;
    groupW = false;

    othersE = false;
    othersW = false;
    othersR = true;
}

permissions::permissions(bool o_r, bool ow, bool ox, bool gr, bool gw, bool gx,bool otr, bool otw, bool otx)
{
    ownerE= ox;
    ownerR = o_r;
    ownerW = ow;

    othersR = otr;
    othersE = otx;
    othersW = otw;

    groupW = gw;
    groupR = gr;
    groupE = gx;
}

bool permissions::canOwnerRead()const 
{ 
    return ownerR; 
}
bool permissions::canOwnerWrite()const 
{  
    return ownerW; 
}
bool permissions::canOwnerExecute()const
{   
    return ownerE; 
}
bool permissions::canGroupRead()const 
{ 
    return groupR; 
}
bool permissions::canGroupWrite()const 
{ 
    return groupW; 
}
bool permissions::canGroupExecute()const 
{ 
    return groupE; 
}
bool permissions::canOthersRead()const 
{ 
    return othersR; 
}
bool permissions::canOthersWrite()const 
{ 
    return othersW; 
}
bool permissions::canOthersExecute()const 
{ 
    return othersE; 
}


void permissions::displayPerms() const
{

}

void permissions::setOwnerRead(bool v)    
{ 
    ownerR = v; 
}
void permissions::setOwnerWrite(bool v)   
{ 
    ownerW = v; 
}
void permissions::setOwnerExecute(bool v) 
{ 
    ownerE = v; 
}
void permissions::setGroupRead(bool v)    
{ 
    groupR = v; 
}
void permissions::setGroupWrite(bool v)   
{ 
    groupW = v; 
}
void permissions::setGroupExecute(bool v) 
{ 
    groupE = v; 
}
void permissions::setOthersRead(bool v)   
{ 
    othersR = v; 
}
void permissions::setOthersWrite(bool v)  
{ 
    othersW = v; 
}
void permissions::setOthersExecute(bool v)
{ 
    othersE = v; 
}
