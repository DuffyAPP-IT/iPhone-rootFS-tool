#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <zconf.h>
#include "iLibX.h"
#include <unistd.h> 
#include <limits.h>
#include <dirent.h>
#include <errno.h>

#define RELEASE 2
#define DEBUG 1

#define ARRAY_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )

void clean_menu(int note, char *notedata) {
    system("clear");
    printf("==================\niPhone-rootFS-tool for macOS v%d\nDebug is %d\n==================\nLICENSE [DUFFYAPPIT[FREE]]\n==================\n", RELEASE, DEBUG);
    if (note !=0) {
        printf("%s\n==================\n", notedata);
    }
}

void dump_with_output_directory(int usecustdir, char *outputDirectory) {
    clean_menu(1, "Initialising");
    if(usecustdir==0){
            clean_menu(0, "");
            system("rm -rf SENSITIVE");
            if (mkdir("SENSITIVE", 0777) == -1) {
                clean_menu(0, "");
                printf("[-] Pre-Acquisition Check Failed -> 'SENSETIVE' Directory Already Exists And May Contain User Data.\nBackup & Remove The 'SENSETIVE' Directory Appropriately Before Executing...\n");
                exit(1);
            }
        
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("[+] Dumping rootfs in current working directory\n");
            } else {
                printf("[-] Error getting current working directory\n");
                exit(1);
           }
    }
        clean_menu(1, "Initialising");
        if(macos_run_ge("which iproxy")==0){
            printf("[+] Starting Device Proxy\n");
            if(macos_run_ge("iproxy 7788 44 &")==0){
                printf("[+] Started Device Proxy\n");
            }
        } else{
            printf("[-] iProxy Not Found...Install Via Brew & Ensure PATH Entry\n'brew install libimobiledevice'\n");
            exit(1);
        }


        clean_menu(1, "Initialising Connected Device...");
        if (ios_fetch_access("127.0.0.1", "7788") == 0) {
            if (DEBUG == 1) {
                printf("[+] Mounting iDevice Disk...\n");
            }
            ios_run_ge("mount -o rw,union,update /"); //need to test without remount
            printf("[+] Ready...\n");
            printf("[+] Dumping Connected Device In Current State.\n[+] This may take some time...\n");
            
            if(usecustdir==1){
                char commandBuffer[PATH_MAX];
                snprintf(commandBuffer, sizeof(commandBuffer), "resources/sshpass -p alpine ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@127.0.0.1 -p7788 'tar zcf - / 2>/dev/null' | resources/pv > %s/filesystem.tar", outputDirectory);

                system(commandBuffer);
                
                printf("[+] Complete! filesystem.tar now available in '%s'\n",outputDirectory);
            }
            else{
                system("resources/sshpass -p alpine ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@127.0.0.1 -p7788 'tar zcf - / 2>/dev/null' | resources/pv > SENSITIVE/filesystem.tar");
                printf("[+] Complete! filesystem.tar now available in 'SENSETIVE/'\n");
            }
        }
    }

void dump_in_output_directory(char *outputDirectory) {
    DIR* dir = opendir(outputDirectory);
    if (dir) {
        printf("[+] Dumping root file system at directory: %s\n", outputDirectory);
        dump_with_output_directory(1,outputDirectory);
    } else if (ENOENT == errno) {
        printf("[-] Directory provided is not valid");
        exit(1);
    }
    closedir(dir);
}
//
//void dump_with_current_directory() {
//    clean_menu(1, "Initialising0");
//    system("rm -rf SENSITIVE");
//    if (mkdir("SENSITIVE", 0777) == -1) {
//        clean_menu(0, "");
//        printf(" [-] Pre-Acquisition Check Failed -> The 'SENSETIVE' Directory Already Exists And May Contain User Data.\n[-] Backup & Remove The 'SENSETIVE' Directory Appropriately Before Executing...\n");
//        exit(1);
//    } else {
//        clean_menu(1, "Initialising1");
//        if (macos_run_ge("which iproxy")==0) {
//            printf("[+] Starting Device Proxy\n");
//            if(macos_run_ge("iproxy 7788 44 &")==0) {
//                printf("[+] Started Device Proxy\n");
//            }
//        } else {
//            printf("[-] iProxy Not Found...Install Via Brew & Ensure PATH Entry\n'brew install libimobiledevice'");
//            exit(1);
//        }
//
//        //SENSETIVE Directory Created
//        //Check If AL Is In FirstLaunch State...
//        clean_menu(1, "Initialising Connected Device...");
//        if (ios_fetch_access("127.0.0.1", "7788") == 0) {
//            //Tunnel To Device Is Open, Send Companion
//            if (DEBUG == 1) {
//                printf("[+] Mounting iDevice Disk...\n");
//            }
//
//            ios_run_ge("mount -o rw,union,update /");
//            printf("[+] Ready...\n");
//            printf("[+] Dumping Connected Device In Current State.\n[+] This may take some time...\n");
//            system("resources/sshpass -p alpine ssh -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@127.0.0.1 -p7788 'tar zcf - / 2>/dev/null' | resources/pv > SENSITIVE/filesystem.tar");
//
////            if(argv[2]!=NULL){
////                printf("[+] Complete! filesystem.tar now available in '%s'\n",argv[2]);
////            } else printf("[+] Complete! filesystem.tar now available in 'SENSETIVE/'\n");
//
//        } else {
//            printf("[-] Stable Connection To Device Could Not Be Established.\n");
//        }
//    }
//}

//void dump_in_current_directory() {
//    char cwd[PATH_MAX];
//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
//        printf("[+] Dumping rootfs in current working directory\n");
//        dump_with_current_directory();
//    } else {
//        printf("[-] Error getting current working directory\n");
//        exit(1);
//   }
//}

int main(int argc, char *argv[]) {
    int opt;
    if (optind < argc ) {
        if ((opt = getopt(argc, argv, "o:")) != -1) {
            switch(opt) {
            case 'o':
                dump_in_output_directory(optarg);
                break;
            default:
                exit(1);
            }
        }
    } else {
       dump_with_output_directory(0,"x");
    }

    return 0;
}
