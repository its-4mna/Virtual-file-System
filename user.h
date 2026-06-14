#ifndef USER_H
#define USER_H

#include "Strings.h"
#include <iostream>

class User
{
public:
    static const int root = 0;
    static const int standard = 1;

private:
    String username;
    int userID;
    int role;
    String homePath;
    String groupName;

public:
    User(const String& name, int id, int r,const String& home, const String& group);
    User(const User& other);
    User& operator=(const User& other);
    ~User();    //do i even need this

    //getters
    int getUserID()const;
    int getRole() const;
    const String& getUsername()const;
    const String& getGroupName() const;
    const String& getHomePath() const;

    bool isRoot()const;
    bool isStandard() const;

    void displayUserInfo() const;
};

#endif