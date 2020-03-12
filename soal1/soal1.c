#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  pid_t pid, sid;   // Variabel untuk menyimpan PID
 
  time_t now = time(NULL);
  struct tm ts = *localtime(&now);

  int secT, minT, hourT;
  char path[200];

  if (argc < 5 || argc > 5) {
    printf("Jumlah argumen lebih atau kurang dari 4.\n");
    return 0;
  }

  if (strcmp(argv[1], "*") == 0)
    secT = ts.tm_sec;
  else {
    secT = atoi(argv[1]);
    if (secT < 0 || secT > 59) {
      printf("Format detik salah\n");
      return 0;
    }
  }

  if (strcmp(argv[2], "*") == 0)
    minT = ts.tm_min;
  else {
    minT = atoi(argv[2]);
    if (minT < 0 || minT > 59) {
      printf("Format menit salah.\n");
      return 0;
    }
  }

  if (strcmp(argv[3], "*") == 0)
    hourT = ts.tm_hour;
  else {
    hourT = atoi(argv[3]);
    if (hourT < 0 || hourT > 23) {
      printf("Format jam salah.\n");
      return 0;
    }
  }

  strcpy(path, argv[4]);
  int pathLen = strlen(path);
  if (path[pathLen - 3] != '.' && path[pathLen - 2] != 's' && path[pathLen - 1] != 'h') {
    printf("Bukan file bash script.\n");
    return 0;
  }

  printf("%d %d %d --- %d %d %d", secT, minT, hourT, ts.tm_sec, ts.tm_min, ts.tm_hour);

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) {

    now = time(NULL);
    ts = *localtime(&now);

    if (strcmp(argv[1], "*") == 0)
      secT = ts.tm_sec;
    if (strcmp(argv[2], "*") == 0)
      minT = ts.tm_min;
    if (strcmp(argv[3], "*") == 0)
      hourT = ts.tm_hour;

    if (secT == ts.tm_sec && minT == ts.tm_min && hourT == ts.tm_hour) {
      pid_t child = fork();
      if (child == 0) {
        char *argm[] = {"bash", path, NULL};
        execv("/bin/bash", argm);
      }
    }
    
    sleep(1);
  }
}
