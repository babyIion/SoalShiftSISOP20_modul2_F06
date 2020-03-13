# SoalShiftSISOP20_modul2_F06

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:

a. Program menerima 4 argumen berupa: Detik: 0-59 atau * (any value), Menit: 0-59 atau * (any value), Jam: 0-23 atau * (any value), Path file .sh

b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
sesuai

c. Programhanya menerima 1 config cron

d. Program berjalan di background (daemon)

e. Tidak boleh menggunakan fungsi system()

Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

#### Penyelesaian
~~~c
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

//  printf("%d %d %d --- %d %d %d", secT, minT, hourT, ts.tm_sec, ts.tm_min, ts.tm_hour);

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
~~~
Penjelasan:
- `time_t now = time(NULL); struct tm ts = *localtime(&now);` untuk mendapatkan timestamp waktu sekarang dengan library time.h
- Pertama program akan mengecek apakah argumen-argumen yang diberikan telah sesuai.
- `pid = fork();` merupakan perintah untuk membuat child process.
- Program selanjutnya membuat daemon dari child process yang telah dibuat sebelumnya.
- Dalam loop program daemon, program akan terus menerus mengambil timestamp terbaru.
- Variable secT, minT, hourT akan disamakan dengan timestamp sekarang apabila ketika menjalankan program argumen detik, menit atau jam adalah "*" (any value)
- Apabila bukan "*" (any value), variable tersebut akan terus berisi angka yang diinputkan melalui argumen.
- Apabila terjadi kecocokan seluruh secT, minT, hourT dengan timestamp sekarang, file bash script akan dijalankan dengan perintah execv melalui child process yang sebelumnya dibuat khusus untuk menjalankan execv tersebut.
- Di akhir loop, diberi sleep selama 1 detik agar menghindari kesalahan pengaturan penjalanan bash script.

## Soal 2
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.

#### Penyelesaian
Loop Daemon
~~~c
...
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
~~~
a. Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
sebuah program C yang per 30 detik membuat sebuah folder dengan nama
timestamp [YYYY-mm-dd_HH:ii:ss].

Penjelasan:
- Pada loop daemon, pertama kali program akan mendapatkan timestamp terbaru lalu melakukan `fork()` untuk membuat child process
- Pada child process yang pertama, akan dibuat child process lagi unutuk melakukan pembuatan folder dengan nama sesuai yang ditentukan. (format : %Y-%m-%d_%H:%M:%S)
- Pembuatan folder menggunakan perintah execv yang akan menjalankan mkdir
- Loop awal akan di `sleep(30);` agar process terulang setiap 30 detik

b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah
detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYYmm-
dd_HH:ii:ss].

Penjelasan:
- Parent dari process pembuatan folder diatas akan menunggu process pembuatan folder selesai dengan `while ((wait(&status2)) > 0);`
- Kemudian akan menyiapkan link untuk mendownload gambar `strcpy(photo, "https://picsum.photos/");` . Dimasukkan ke variable char photo.
- Akan dijalankan loop sebanyak 20x untuk mendownload gambar.
- Untuk mendapatkan detik Epoch Unix dapat menggunakan command `time(NULL)`. Detik ini akan digunakan untuk menentukan ukuran gambar yang didownload `snprintf(photolink, 200, "%s%ld", photo, (time(NULL)%1000 + 100));`
- Setelah itu akan dibuat child yang khusus untuk mendownload gambar dengan perintah wget
- `char *argm3[] = {"wget", photolink, "-O", photoname, "-q", "-o", "/dev/null", NULL};` wget menggunakan opsi -O untuk mengatur filename gambar yang didownload dan `-q -o /dev/null` agar wget tidak menghasilkan output(misal : logfile).
- Setiap loop, program akan di sleep selama 5 detik.

c. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
folder akan di delete(sehingga hanya menyisakan .zip).

Penjelasan :
- Setelah loop selesai menjalankan pendownloadan gambar, akan dibuat child process lagi untuk melakukan process zip folder.
- `char *argm4[] = {"zip", "-r", zipname, folder, NULL};` zip menggunakan opsi -r untuk menandakan akan men-zip sebuah folder dengan nama output zipname(berisi nama folder yang sudah berisi gambar + .zip) dan file yang akan di zip adalah folder(berisi nama folder yang sudah berisi gambar);
- Parent akan menunggu child process selesai dengan `while ((wait(&status3)) > 0);` lalu setelah itu menghapus folder yang sebelumnya dilakukan zip
- `char *argm5[] = {"rm", "-r", folder, NULL};` rm menggunakan opsi -r yang menandakan akan me-remove sebuah folder.
- Percabangan process berhenti di sini, program akan kembali ke cabang paling awal di mana akan berlanjut ke `sleep(30);`

Membuat Program Killer
~~~c
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
...
~~~

d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut mengenerate
sebuah program "killer" yang siap di run(executable) untuk
menterminasi semua operasi program tersebut. Setelah di run, program yang
menterminasi ini lalu akan mendelete dirinya sendiri.

Penjelasan :
- File killer dapat dibuat menggunakan perintah pembuatan file dalam C.
- Akan dibuat pointer to FILE yang akan digunakan untuk membuat file bash script.
- Bash script tersebut diisi dengan perintah `killall ./soal2` atau `kill -9 (pid)` tergantung dari mode yang dijalankan (-a atau -b)
- Program akan menghapus dirinya sendiri menggunakan `rm $0`
- Akan dibuat child process yang khusus untuk mengubah permission file yang baru kita buat dengan perintah chmod
- `char *argmB[] = {"chmod", "+x", "killer[A/B].sh", NULL};` chmod menggunakan opsi +x yang menandakan file kita akan ditambahkan akses executable sehingga dalam dijalankan.

e. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen
-b. Ketika dijalankan dalam MODE_A, program utama akan langsung
menghentikan semua operasinya ketika program killer dijalankan. Untuk
MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
folder terisi gambar, terzip lalu di delete).

Penjelasan :
- `int main(int argc, char *argv[])` menggunakan argumen pada int main sehingga dapat menerima opsi ketika program dijalankan apakah -a atau -b
- Program akan membuat killerprogram yang sesuai dengan mode dijalankannya.
- killerA akan melakukan killall sehingga segala process berhenti saat itu juga, sedangkan killerB akan melakukan kill process parent yang merupakan loop utama daemon yang melakukan spawning child process terus menerus. Child process yang sedang berjalan akan terus berjalan hingga selesai pada mode b.

## Soal 3
### a

Program buatan jaya harus bisa membuat dua direktori di
“/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.

Penyelesaian:
~~~c
pid_t child_id, child_id1, child_id2, child_id3;
int status;

child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }
    if (child_id == 0) {
        // this is child
        char *argv[] = {"mkdir", "-p", "/home/tari/modul2/indomie", NULL};
        execv("/bin/mkdir", argv);
    }
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
...
~~~

Penjelasan:
~~~c
pid_t child_id, child_id1, child_id2, child_id3;
int status;
~~~
Mendeklarasi variabel yang akan dipakai.
~~~c
child_id = fork();
~~~
fork() untuk membuat child process.
~~~c
if (child_id == 0) {
      // this is child
      char *argv[] = {"mkdir", "-p", "/home/tari/modul2/indomie", NULL};
      execv("/bin/mkdir", argv);
}else {
      // this is parent
      while ((wait(&status)) > 0);
      sleep(5);
~~~
Arti dari `child_id == 0` merupakan child process, di mana dilakukan proses membuat folder indomie. Pembuatan folder menggunakan perintah execv yang akan menjalankan mkdir. Dalam block else, `while ((wait(&status)) > 0);` untuk menunggu child process selesai, `sleep(5)` artinya menunggu selama 5 detik untuk melakukan proses selanjutnya.
~~~c
child_id1 = fork();
        if (child_id1 < 0){
            exit(EXIT_FAILURE);
        }
        if (child_id1 == 0){
            char *argv[] = {"mkdir", "-p", "/home/tari/modul2/sedaap", NULL};
            execv("/bin/mkdir", argv);
        }
~~~
Melakukan fork() lagi untuk membuat child process yang mengeksekusi hal yang sama dengan yang di atas, hanya saja "indomie" diganti dengan "sedaap".

### b
Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.

### Penyelesaian
~~~c
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
~~~
dalam else, melakukan fork() lagi untuk melakukan unzip. Unzip menggunakan perintah execv yang akan menjalankan unzip dari `"/home/tari/jpg.zip"`, `-d` adalah opsi untuk meletakkan hasil unzip di `"/home/tari/jpg.zip"`.

### c
Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.

### Penyelesaian
~~~c
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
~~~
Penjelasan:
~~~c
DIR *d;
struct dirent *dir;
d = opendir("/home/tari/modul2/jpg/");
~~~
Mendeklarasikan sebuah direktori d yang membuka direktori `/home/tari/modul2/jpg/`.
~~~c
char namafile[300];
while ((dir = readdir(d)) != NULL)
~~~
Mendeklarasikan string namafile dengan size 300, lalu membuat loop unutk membaca isi direktori.
~~~c
if(dir->d_type == DT_REG)
~~~
kondisi ketika tipe dari isi direktori merupakan regular file.
~~~c
if (child_id4 == 0){
    snprintf(namafile, 300, "/home/tari/modul2/jpg/%s", dir->d_name);
    char *argv[] = {"mv", namafile, "/home/tari/modul2/sedaap", NULL};
    execv("/bin/mv", argv);
}
~~~
melakukan fork(), dalam block child process melakukan `snprintf(namafile, 300, "/home/tari/modul2/jpg/%s", dir->d_name)` untuk menginisialisasi string namafile dengan `"/home/tari/modul2/jpg/%s", dir->d_name"`. Pemindahan file-file dilakukan dengan perintah execv yang akan menjalankan mv.
~~~c
else{
	if( !(strcmp(dir->d_name, ".")) || !(strcmp(dir->d_name, "..")) );
...
	if (child_id == 0){
		snprintf(namafile, 300, "/home/tari/modul2/jpg/%s", dir->d_name);
		char *argv[] = {"mv", namafile, "/home/tari/modul2/indomie", NULL};
		execv("/bin/mv", argv);
	}
~~~
ketika tipe dari isi direktori bukan regular file (dalam hal ini berarti sebuah direktori / folder), `if( !(strcmp(dir->d_name, ".")) || !(strcmp(dir->d_name, "..")) )` adalah untuk mengabaikan direktori . (direktori sendiri) dan .. (parent direktori). Setelah itu membuat fork() lagi kemudian dalam child process memindah semua direktori ke dalam direktori indomie.

### d
Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).

### Penyelesaian
~~~c
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
~~~
mendeklarasikan string folder dengan size 350, setelah itu melakukan looping dari 1 sampai 2. Dalam loop membuat file coba1.txt yang setelah 3 detik kemudian membuat file coba2.txt.
