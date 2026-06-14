#include "user.h"

User::User(const String& name, int id, int r,const String& home, const String& group)
{
    username = name;
    userID = id;
    role = r;
    homePath = home;
    groupName = group;
}

User::User(const User& other)
{
    username = other.username;
    userID = other.userID;
    role = other.role;
    homePath = other.homePath;
    groupName = other.groupName;
}

User& User::operator=(const User& other)
{
    if (this != &other)
    {
        username = other.username;
        userID = other.userID;
        role= other.role;
        homePath= other.homePath;
        groupName = other.groupName;
    }
    return *this;
}

User::~User()
{
}

bool User::isRoot() const
{
    return role == root;
}

bool User::isStandard() const
{
    return role == standard;
}


const String& User::getUsername() const
{
    return username;
}

int User::getUserID() const
{
    return userID;
}

int User::getRole() const
{
    return role;
}


const String& User::getGroupName() const
{
    return groupName;
}

const String& User::getHomePath() const
{
    return homePath;
}



void User::displayUserInfo() const
{
    cout << "\nUsername: " << username;
    cout << "\nUserID: " << userID;
    cout << "\nRole: ";
    if(isRoot())
        cout << "root";
    else
        cout << "standard";
    cout << "\nGroup: " << groupName;
    cout << "\nHomePath: " << homePath;
}