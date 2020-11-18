//
// Created by 010010 on 20/07/2020.
//

#include "iLibX.h"

#define DEBUG 0

int ios_send_f(char *filetosend, char *remotedir){
    FILE *fileout;
    if ((fileout = fopen(filetosend,"r"))!=NULL) {
        fclose(fileout);
        char commout[800];
        sprintf(commout, "sshpass -p alpine scp -P 2222 -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" %s root@127.0.0.1:%s >/dev/null 2>/dev/null \; echo $?", filetosend, remotedir);
        char *com = commout;
        char out[2048];
        FILE *shell = popen(com, "r");
        fgets(out, sizeof(out), shell);
        pclose(shell);
        if (atoi(out)==0) {
            return 0;
        } else{
            return 1;
        }
    }
}

char *macos_run_comm(char *command){
    char com2[1024];
    strcpy(com2, command);
    char commout[1024];
    sprintf(commout, "%s", com2);
    char *com = commout;
    char out[4096];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return out;
}

int ios_fetch_access(char *ipin,char *port){
    char *ping = "nc -z";
    char *silence = ">/dev/null 2>/dev/null";
    char command[800];
    sprintf(command,"%s %s %s %s",ping,ipin,port,silence);
    FILE * shell;
    shell = popen(command, "r");
    int r = pclose(shell);
    if (WEXITSTATUS(r)==0) {
        return 0;
    } else {
        return 1;
    }
}

int ios_run_ge(char *command){
    char *com1 = "sshpass -p";
    char *com2 = "ssh  -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@127.0.0.1 -p 2222";
    char *silence = "2>/dev/null";
    char commout[2400];
    sprintf(commout, "%s alpine %s %s %s", com1, com2, command, silence);
    if (DEBUG == 1) {
        printf("[+] EXECUTING -> %s",commout);
    }
    char *com = commout;
    char out[4048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return atoi(out);
}

int ios_rec_f(char *remotefiledir, char *localfname){
    char commout[800];
    sprintf(commout, "sshpass -p alpine scp -r -P 2222 -o \"UserKnownHostsFile=/dev/null\" -o \"StrictHostKeyChecking=no\" root@127.0.0.1:%s %s >/dev/null 2>/dev/null \; echo $?", remotefiledir, localfname);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    if (atoi(out)==0){
        return 0;
    } else{
        return 1;
    }
}

void execSQL(char *fname, char *sql){
    char execSQL[2000];
    sprintf(execSQL,"sqlite3 -line %s \"%s\" | awk 'NF' | cut -f2- -d'=' | cut -f2- -d' '", fname, sql);
    system(execSQL);
}

int macos_run_ge(char *command){
    char *com2 = ">/dev/null 2>/dev/null;echo $?";
    char com1[2400];
    strcpy(com1, command);
    char commout[2400];
    sprintf(commout, "%s %s", com1, com2);
    char *com = commout;
    char out[2048];
    FILE *shell = popen(com, "r");
    fgets(out, sizeof(out), shell);
    pclose(shell);
    return atoi(out);
}