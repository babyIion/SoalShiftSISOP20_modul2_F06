#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>

int main(){
    pid_t child_id, child_id1, child_id2, child_id3;
    int status;
	int i;

    child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }
    if (child_id == 0) {
        // this is child
        char *argv[] = {"mkdir", "-p", "/home/tari/modul2/indomie", NULL};
        execv("/bin/mkdir", argv);
    }
    else {
        // this is parent
        while ((wait(&status)) > 0);
        sleep(5);
        child_id1 = fork();
        if (child_id1 < 0){
            exit(EXIT_FAILURE);
        }
        if (child_id1 == 0){
            char *argv[] = {"mkdir", "-p", "/home/tari/modul2/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
        else{
            while ((wait(&status)) > 0);
            child_id2 = fork();
            if (child_id2 < 0){
                exit(EXIT_FAILURE);
            }
            if (child_id2 == 0){
                char *argv[] = {"unzip", "/home/tari/jpg.zip", "-d", "/home/tari/modul2/", NULL};
                execv("/usr/bin/unzip", argv);
            }
            else{
                while ((wait(&status)) > 0);
                child_id3 = fork();
                if (child_id3 < 0) {
                    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
                }
                if (child_id3 == 0) {
                    // this is child
                    DIR *d;
                    struct dirent *dir;
                    d = opendir("/home/tari/modul2/jpg/");
                    if(d){
                        char namafile[300];
                        while ((dir = readdir(d)) != NULL)
                        {
                            if(dir->d_type == DT_REG){
                                pid_t child_id4;
                                child_id4 = fork();

                                if (child_id4 < 0){
                                    exit(EXIT_FAILURE);
                                }
                                if (child_id4 == 0){
                                    snprintf(namafile, 300, "/home/tari/modul2/jpg/%s", dir->d_name);
                                    char *argv[] = {"mv", namafile, "/home/tari/modul2/sedaap", NULL};
                                    execv("/bin/mv", argv);
                                }
                                else {
                                    while ((wait(&status)) > 0);			
                                }
                            }
                            else{
                                if( !(strcmp(dir->d_name, ".")) || !(strcmp(dir->d_name, "..")) );
                                else{
                                    child_id = fork();

                                    if (child_id < 0){
                                        exit(EXIT_FAILURE);
                                    }
                                    if (child_id == 0){
                                        snprintf(namafile, 300, "/home/tari/modul2/jpg/%s", dir->d_name);
                                        char *argv[] = {"mv", namafile, "/home/tari/modul2/indomie", NULL};
                                        execv("/bin/mv", argv);
                                    }
                                    else {
                                        while ((wait(&status)) > 0);
                                        char folder[350];
                                        for (i = 1; i < 3; i++)
                                        {
                                            snprintf(folder, 350, "/home/tari/modul2/indomie/%s/coba%d.txt", dir->d_name, i);
                                            FILE *fp;
                                            fp = fopen(folder, "w");
                                            fclose(fp);
                                            sleep(3);
                                        }
                                    }
                                }
                            }   
                        }
                    }
                }
            }
        }
    }
}