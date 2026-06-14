#include "versions.h"

versions::versions(const String& content,time_t t,const String& user)
{
    oldContent=content;
    modifiedTime=t;
    modifyingUser=user;
    struct tm* timeinfo=localtime(&modifiedTime);
    if(timeinfo)
    {
        hour=timeinfo->tm_hour;
        minute=timeinfo->tm_min;
        second=timeinfo->tm_sec;
        char buffer[100];
        const char* months[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        int pos=0;
        int day=timeinfo->tm_mday;
        int mon=timeinfo->tm_mon;
        int year=timeinfo->tm_year+1900;
        if(day<10) buffer[pos++]='0';
        for(int i=0;i<100;i++) timeString[i]=0;
    }
    else
    {
        hour=0;minute=0;second=0;
        for(int i=0;i<100;i++) timeString[i]=0;
    }
}

versions::versions(const versions& other)
{
    oldContent=other.oldContent;
    modifiedTime=other.modifiedTime;
    modifyingUser=other.modifyingUser;
    hour=other.hour;
    minute=other.minute;
    second=other.second;
    for(int i=0;i<100;i++) timeString[i]=other.timeString[i];
}

versions& versions::operator=(const versions& other)
{
    if(this!=&other)
    {
        oldContent=other.oldContent;
        modifiedTime=other.modifiedTime;
        modifyingUser=other.modifyingUser;
        hour=other.hour;
        minute=other.minute;
        second=other.second;
        for(int i=0;i<100;i++) timeString[i]=other.timeString[i];
    }
    return *this;
}

versions::~versions(){}

const String& versions::getOldContent()const{return oldContent;}
time_t versions::getModifiedTime()const{return modifiedTime;}
const String& versions::getModifyingUser()const{return modifyingUser;}

void versions::print(int index)const
{
    std::cout<<"    Version "<<index<<" | By: "<<modifyingUser;
    std::cout<<" | Time: "<<hour<<":"<<minute<<":"<<second;
    std::cout<<" | Content: \""<<oldContent<<"\"\n";
}