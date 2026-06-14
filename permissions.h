#ifndef PERMISSIONS_H
#define PERMISSIONS_H

class permissions
{
private:
    // Owner permissions
    bool ownerR, ownerW, ownerE;
    // Group permissions
    bool groupR, groupW, groupE;
    // Others permissions
    bool othersR, othersW, othersE;

public:
    permissions();
    permissions(bool o_r, bool ow, bool ox, bool gr, bool gw, bool gx,bool otr, bool otw, bool otx);
    void displayPerms() const;
    //setters
    void setOwnerRead(bool v);
    void setOwnerWrite(bool v);
    void setOwnerExecute(bool v);
    void setGroupRead(bool v);
    void setGroupWrite(bool v);
    void setGroupExecute(bool v);
    void setOthersRead(bool v);
    void setOthersWrite(bool v);
    void setOthersExecute(bool v);
    //getters
    bool canOwnerRead() const;
    bool canOwnerWrite() const;
    bool canOwnerExecute() const;
    bool canGroupRead() const;
    bool canGroupWrite() const;
    bool canGroupExecute() const;
    bool canOthersRead() const;
    bool canOthersWrite() const;
    bool canOthersExecute() const;
    


};

#endif