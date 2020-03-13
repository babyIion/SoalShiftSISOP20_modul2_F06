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
#include <wait.h>

int main(int argc, char *argv[]) {
  pid_t pid, sid;   // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  pid_t thispid = getpid();
  int killpid = (int)thispid;

  if (argv[1][1] == 'a') {
  	FILE *file;
	file = fopen("killerA.sh", "w");
	fprintf(file, "killall ./soal2\n");
	fprintf(file, "rm $0\n");
	fclose(file);
	if (fork() == 0) {
		char *argmA[] = {"chmod", "+x", "killerA.sh", NULL};
		execv("/bin/chmod", argmA);
	}
  }
  else if (argv[1][1] == 'b') {
  	FILE *file;
	file = fopen("killerB.sh", "w");
	fprintf(file, "kill -9 %d\n", killpid);
	fprintf(file, "rm $0\n");
	fclose(file);
	if (fork() == 0) {
		char *argmB[] = {"chmod", "+x", "killerB.sh", NULL};
		execv("/bin/chmod", argmB);
	}
  }

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
	time_t now = time(NULL);
	struct tm ts = *localtime(&now);
	char folder[200];
	strftime(folder, 200, "%Y-%m-%d_%H:%M:%S", &ts);

	pid_t child1 = fork();
	if (child1 == 0) {
		int status2;
		pid_t child2 = fork();

		if (child2 == 0) {
			char *argm[] = {"mkdir", folder, NULL};
			execv("/bin/mkdir", argm);
		}
		else {
			while ((wait(&status2)) > 0);
			int i;
			char photo[200];
			strcpy(photo, "https://picsum.photos/");
			for (i=0; i<20; i++) {
				time_t now = time(NULL);
				struct tm ts = *localtime(&now);
				char timenow[200];
				strftime(timenow, 200, "%Y-%m-%d_%H:%M:%S", &ts);
				char photoname[200];
				snprintf(photoname, 200, "%s/%s", folder, timenow);
				char photolink[200];
				snprintf(photolink, 200, "%s%ld", photo, (time(NULL)%1000 + 100));

				pid_t childtmp = fork();
				if (childtmp == 0) {
					char *argm3[] = {"wget", photolink, "-O", photoname, "-q", "-o", "/dev/null", NULL};
					execv("/usr/bin/wget", argm3);
				}
				sleep(5);
			}

			int status3;
			pid_t child3 = fork();

			if (child3 == 0) {
				char zipname[200];
				snprintf(zipname, 200, "%s.zip", folder);
				char *argm4[] = {"zip", "-r", zipname, folder, NULL};
				execv("/usr/bin/zip", argm4);
			}
			else {
				while ((wait(&status3)) > 0);
				char *argm5[] = {"rm", "-r", folder, NULL};
				execv("/bin/rm", argm5);
			}
		}
	}
	sleep(30);
  }
}
