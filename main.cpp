/*
OBJECT ORIENTED PROGRAMMING
ASSIGNMENT 03: VFS
*/

#include <iostream>
using namespace std;

#include "volume.h"
#include "regFile.h"
#include "partition.h"

void printHeader(const char* title) 
{ 
    std::cout<<"\n--------------------------------------------\n  "<<title<<"\n--------------------------------------------\n"; 
}


int main()
{
    printHeader("Creating Volume");
    Volume volume(String("FAST-VFS"));
    std::cout<<"  Volume 'FAST-VFS' created.\n";

    printHeader("Registering Users");
    User* root=volume.registerUser(String("root"),0,User::root,String("/root"),String("root"));
    User* alice=volume.registerUser(String("alice"),1001,User::standard,String("/home/alice"),String("developers"));
    User* bob=volume.registerUser(String("bob"),1002,User::standard,String("/home/bob"),String("students"));
    User* charlie=volume.registerUser(String("charlie"),1003,User::standard,String("/home/charlie"),String("students"));
    std::cout<<"  Registered 4 users.\n"; volume.printUsers();

    printHeader("Creating Storage Media");
    storageMedium* ssd=volume.addMedium(512,storageMedium::TYPE_SSD);
    storageMedium* hdd=volume.addMedium(1024,storageMedium::TYPE_HDD);
    storageMedium* optical=volume.addMedium(700,storageMedium::TYPE_OPTICAL);
    std::cout<<"  SSD (512MB) added.\n  HDD (1024MB) added.\n  Optical Drive (700MB) added.\n";

    printHeader("Creating Partitions");
    Partition* ssd_boot=ssd->createPartition(String("ssd_boot"),100,String("ext4"),root);
    Partition* ssd_home=ssd->createPartition(String("ssd_home"),400,String("ext4"),root);
    Partition* hdd_data=hdd->createPartition(String("hdd_data"),600,String("NTFS"),root);
    Partition* hdd_backup=hdd->createPartition(String("hdd_backup"),400,String("FAT32"),root);
    Partition* opt_disc=optical->createPartition(String("opt_disc"),650,String("ISO9660"),root);
    std::cout<<"  SSD partitions: ssd_boot (100MB), ssd_home (400MB).\n  HDD partitions: hdd_data (600MB), hdd_backup (400MB).\n  Optical: opt_disc (650MB).\n";
    volume.setPrimaryPartition(ssd_boot);
    std::cout<<"  Primary partition set to 'ssd_boot'.\n";

    printHeader("Building File System Hierarchy");
    permissions def(true,true,true,true,false,false,true,false,false);
    permissions ro(true,false,false,true,false,false,true,false,false);
    permissions priv(true,true,true,true,true,false,false,false,false);
    permissions world(true,true,true,true,true,false,true,true,false);

    directoy* root_dir=ssd_boot->getRootDir();
    directoy* home=volume.createdirectoy(root_dir,String("home"),root,def);
    directoy* etc=volume.createdirectoy(root_dir,String("etc"),root,ro);
    directoy* var=volume.createdirectoy(root_dir,String("var"),root,def);
    directoy* mnt=volume.createdirectoy(root_dir,String("mnt"),root,def);
    directoy* alice_home=volume.createdirectoy(home,String("alice"),root,priv); if(alice_home)alice_home->setOwner(alice);
    directoy* bob_home=volume.createdirectoy(home,String("bob"),root,priv); if(bob_home)bob_home->setOwner(bob);
    directoy* charlie_home=volume.createdirectoy(home,String("charlie"),root,priv); if(charlie_home)charlie_home->setOwner(charlie);
    directoy* network=volume.createdirectoy(etc,String("network"),root,ro);
    directoy* nginx=volume.createdirectoy(etc,String("nginx"),root,ro);
    directoy* log=volume.createdirectoy(var,String("log"),root,def);
    directoy* tmp=volume.createdirectoy(var,String("tmp"),root,world);
    directoy* alice_proj=volume.createdirectoy(alice_home,String("projects"),alice,priv);
    directoy* alice_docs=volume.createdirectoy(alice_home,String("docs"),alice,priv);
    directoy* bob_assign=volume.createdirectoy(bob_home,String("assignments"),bob,priv);
    directoy* bob_notes=volume.createdirectoy(bob_home,String("notes"),bob,def);
    directoy* oop_dir=volume.createdirectoy(alice_proj,String("oop"),alice,priv);
    directoy* dsa_dir=volume.createdirectoy(alice_proj,String("dsa"),alice,priv);

    printHeader("Creating Files");
    regularFile* nginx_conf=volume.createFile(nginx,String("nginx.conf"),root,ro,String("worker_processes auto;\nevents { worker_connections 1024; }\nhttp { include /etc/nginx/sites-enabled/*; }\n"),ssd_boot);
    regularFile* net_iface=volume.createFile(network,String("interfaces"),root,ro,String("auto lo\niface lo inet loopback\nauto eth0\niface eth0 inet dhcp\n"),ssd_boot);
    regularFile* readme=volume.createFile(alice_docs,String("readme.txt"),alice,def,String("Welcome to Alice's docs folder.\nThis file contains project notes."),ssd_boot);
    regularFile* oop_file=volume.createFile(oop_dir,String("assignment3.cpp"),alice,priv,String("#include <iostream>\nint main() { return 0; }\n"),ssd_boot);
    regularFile* hw1=volume.createFile(bob_assign,String("hw1.txt"),bob,def,String("Homework 1: Solve the knapsack problem using dynamic programming."),ssd_boot);
    regularFile* lecture=volume.createFile(bob_notes,String("lecture_notes.txt"),bob,def,String("Lecture 1: Introduction to OOP.\nLecture 2: Classes and Objects.\nLecture 3: Inheritance and Polymorphism.\n"),ssd_boot);
    regularFile* syslog=volume.createFile(log,String("system.log"),root,ro,String("[INFO] System started.\n[INFO] All services running.\n"),ssd_boot);
    regularFile* charlie_note=volume.createFile(charlie_home,String("notes.txt"),charlie,def,String("Charlie's private notes. This file is important."),ssd_boot);

    printHeader("Adding Extended Attributes");
    if(readme){readme->addXattr(String("mime-type"),String("text/plain"));readme->addXattr(String("encoding"),String("UTF-8"));std::cout<<"  Added xattrs to readme.txt\n";}
    if(oop_file){oop_file->addXattr(String("mime-type"),String("text/x-c++"));oop_file->addXattr(String("compiler"),String("g++"));std::cout<<"  Added xattrs to assignment3.cpp\n";}

    printHeader("Creating Symbolic Links");
    SymbolicLink* link_oop=volume.createSymLink(alice_docs,String("oop_link"),alice,def,oop_dir,String("/home/alice/projects/oop"));
    SymbolicLink* link_readme=volume.createSymLink(bob_home,String("alice_readme_link"),bob,def,readme,String("/home/alice/docs/readme.txt"));
    regularFile* temp=volume.createFile(tmp,String("temp.txt"),alice,world,String("This is a temporary file that will be deleted."),ssd_boot);
    SymbolicLink* dangling=volume.createSymLink(var,String("dangling_link"),root,def,temp,String("/var/tmp/temp.txt"));
    std::cout<<"\n  Deleting temp.txt to create dangling link...\n";
    if(dangling)dangling->setTarget(0);
    if(tmp)tmp->deleteChild(String("temp.txt"),root);
    std::cout<<"  temp.txt deleted. dangling_link is now dangling.\n";

    printHeader("Mounting Second Partition");
    MountPoint* hdd_mp=volume.createMountPoint(mnt,String("hdd_data"),root,def);
    if(hdd_mp&&hdd_data){hdd_mp->mountPartition(hdd_data);volume.registerMount(hdd_data);
        directoy* hdd_root=hdd_data->getRootDir();
        directoy* shared=volume.createdirectoy(hdd_root,String("shared"),root,world);
        directoy* archive=volume.createdirectoy(hdd_root,String("archive"),root,def);
        volume.createFile(shared,String("shared_notes.txt"),root,world,String("This is shared storage on the HDD."),hdd_data);
        volume.createFile(archive,String("backup.tar"),root,ro,String("[binary archive data placeholder]"),hdd_data);
        std::cout<<"  Files added to mounted HDD partition.\n";
    }

    printHeader("Attempting Duplicate Mount (Should Fail)");
    MountPoint* mp2=volume.createMountPoint(mnt,String("hdd_data_2"),root,def);
    if(mp2&&hdd_data)mp2->mountPartition(hdd_data);

    printHeader("Recursive Search");
    volume.searchVolume(String("notes"),alice);
    volume.searchVolume(String("log"),root);
    volume.searchVolume(String("assign"),bob);

    printHeader("Subtree Size Calculation");
    std::cout<<"  Size of /home/alice: "<<alice_home->getSize()<<" bytes\n  Size of /home/bob: "<<bob_home->getSize()<<" bytes\n  Size of /etc: "<<etc->getSize()<<" bytes\n  Size of root partition: "<<ssd_boot->getRootDir()->getSize()<<" bytes\n";

    printHeader("Moving a directoy");
    std::cout<<"  Moving /home/bob/notes -> /home/alice/docs/notes\n";
    volume.moveEntity(bob_home,String("notes"),alice_docs,root);

    printHeader("Copying a File");
    std::cout<<"  Copying hw1.txt from bob to alice's docs\n";
    volume.copyEntity(String("hw1.txt"),bob_assign,alice_docs,alice);

    printHeader("File Version History");
    if(readme){std::cout<<"  Modifying readme.txt twice...\n";
        volume.modifyFile(readme,alice,String("Version 2: Added more project details and documentation."),ssd_boot);
        volume.modifyFile(readme,alice,String("Version 3: Final revision with full content."),ssd_boot);
        readme->printHistory();
        std::cout<<"  Restoring to version 0...\n";
        readme->restoreVersion(0,alice);
        std::cout<<"  Current content: \""<<readme->getContent()<<"\"\n";
    }

    printHeader("Shallow Listing");
    if(alice_home)alice_home->listShallow(alice);
    if(etc)etc->listShallow(root);
    if(bob_home)bob_home->listShallow(bob);

    printHeader("Deep Listing");
    std::cout<<"  Deep listing of /home/alice:\n";
    if(alice_home)alice_home->listDeep(alice,0);

    printHeader("Dangling Link Detection");
    volume.detectDanglingLinks();

    printHeader("permissions Violations");
    std::cout<<"  Bob tries to read alice's private file:\n";
    if(oop_file)volume.readFile(oop_file,bob);
    std::cout<<"\n  Charlie tries to modify nginx.conf:\n";
    if(nginx_conf)volume.modifyFile(nginx_conf,charlie,String("malicious"),ssd_boot);
    std::cout<<"\n  Bob tries to delete from /etc/nginx:\n";
    if(nginx)nginx->deleteChild(String("nginx.conf"),bob);
    std::cout<<"\n  Alice tries to delete /etc/network/interfaces:\n";
    if(network)network->deleteChild(String("interfaces"),alice);

    printHeader("Capacity Violation");
    std::cout<<"  opt_disc capacity: "<<opt_disc->getAllocatedCapacityMB()<<"MB\n  Consuming 649MB to nearly fill...\n";
    opt_disc->consumeSpace(649*1024*1024);
    std::cout<<"  Available: "<<opt_disc->getAvailableSpaceMB()<<"MB. Trying 2MB file...\n";
    bool capFail=opt_disc->consumeSpace(2*1024*1024);
    if(!capFail)std::cout<<"  [DEMO] Capacity enforcement works.\n";
    opt_disc->releaseSpace(649*1024*1024);

    printHeader("Symbolic Link Resolution");
    if(link_readme){std::cout<<"  Resolving alice_readme_link:\n";
        if(!link_readme->isDangling()){sysEntity* t=link_readme->getTarget();std::cout<<"  -> ["<<t->getType()<<"] "<<t->getName()<<"\n";}
        else std::cout<<"  -> Dangling!\n";
    }
    if(link_oop){std::cout<<"\n  Resolving oop_link:\n";
        if(!link_oop->isDangling()){sysEntity* t=link_oop->getTarget();std::cout<<"  -> ["<<t->getType()<<"] "<<t->getName()<<"\n";}
    }
    if(dangling)std::cout<<"\n  dangling_link is "<<(dangling->isDangling()?"DANGLING":"valid")<<"\n";

    printHeader("Mount Point Listing");
    if(hdd_mp){std::cout<<"  Contents of HDD mount point:\n";hdd_mp->listShallow(root);
        std::cout<<"\n  Deep listing of HDD root:\n";hdd_data->getRootDir()->listDeep(root,1);
    }

    volume.printVolumeState();
    std::cout<<"\n============================================\n  End of VFS Simulation.\n============================================\n";
    return 0;
}