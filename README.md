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
~~~
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
- `time_t now = time(NULL);``struct tm ts = *localtime(&now);` untuk mendapatkan timestamp waktu sekarang dengan library time.h
- Pertama program akan mengecek apakah argumen-argumen yang diberikan telah sesuai.
- `pid = fork();` merupakan perintah untuk membuat child process.
- Program selanjutnya membuat daemon dari child process yang telah dibuat sebelumnya.
- Dalam loop program daemon, program akan terus menerus mengambil timestamp terbaru.
- Variable secT, minT, hourT akan disamakan dengan timestamp sekarang apabila ketika menjalankan program argumen detik, menit atau jam adalah "*" (any value)
- Apabila bukan "*" (any value), variable tersebut akan terus berisi angka yang diinputkan melalui argumen.
- Apabila terjadi kecocokan seluruh secT, minT, hourT dengan timestamp sekarang, file bash script akan dijalankan dengan perintah execv melalui child process yang sebelumnya dibuat khusus untuk menjalankan execv tersebut.
- Di akhir loop, diberi sleep selama 1 detik agar menghindari kesalahan pengaturan penjalanan bash script.



### a. Tentukan wilayah bagian (region) mana yang memiliki keuntungan (profit) paling sedikit

#### Penyelesaian
Menggunakan awk. Berikut merupakan kode awk-nya.
~~~
awk 'BEGIN {FS="\t"; PROCINFO["sorted_in"]="@val_num_asc"}
FNR>1 {reg[$13]+=$21}
END {for(i in reg) print i, reg[i]}' Sample-Superstore.tsv |
awk '{print $1}' | head -1
~~~
Penjelasan:
- Dalam block **BEGIN** 
	- `FS=\"t"` artinya data yang diinput menggunakan "tab" sebagai pembatas kolom.
	- `PROCINFO["sorted_in"]="@val_num_asc"` merupakan fitur yang digunakan untuk meng-scan array menurut value-nya yang berupa numeric secara ascending (kecil ke besar)
- Dalam block **body**
	- `FNR>1` artinya mengabaikan row pertama (yang dalam data ini merupakan title atau atribut)
	- `{reg[$13]+=$21}` mengisi array reg dengan jumlah "profit" (kolom 21) di mana index array reg adalah data pada kolom 13
- Dalam block **END**
	- `{for(i in reg) print i, reg[i]}` iterasi array reg dengan i sebagai indexnya, lalu mencetak semua index dan isi dari array
- `Sample-Superstore.tsv` nama file yang menjadi input
- `awk '{print $1}'` mencetak kolom pertama dari hasil awk yang sebelumnya
- `head -1` membatasi hanya row pertama saja yang ditampilkan

### b. Tampilkan 2 negara bagian (state) yang memiliki keuntungan (profit) paling sedikit berdasarkan hasil poin a

#### Penyelesaian
Menggunakan awk, dan memanfaatkan hasil dari a. Berikut merupakan kode awk-nya.
~~~
awk -v min_region="$min_region" 'BEGIN {FS="\t"; PROCINFO["sorted_in"]="@val_num_asc"}
($13~min_region) {state[$11]+=$21}
END{for(i in state) print i, state[i]}' Sample-Superstore.tsv |
awk '{print $1}' | head -2
~~~
Penjelasan:
- `-v min_region="$min_region"` -v merupakan command untuk mendeklarasikan variable min_region di mana valuenya adalah hasil dari perintah sebelumnya
- Dalam block **BEGIN** sama seperti sebelumnya
- Dalam block **body**
	- `($13~min_region)` mencari pattern pada kolom 13 yang memiliki data sama dengan min_region 
	- `{state[$11]+=$21}` mengisi array state dengan jumlah "profit" (kolom 21) di mana index array state adalah data pada kolom 11
- Dalam block **END**
	- `for(i in state) print i, state[i]` iterasi array state dengan i sebagai indexnya, lalu mencetak semua index dan isi dari array
- `head -2` hanya menampilkan dua row teratas

### c. Tampilkan 10 produk (product name) yang memiliki keuntungan (profit) paling sedikit berdasarkan 2 negara bagian (state) hasil poin b

#### Penyelesaian
Menggunakan awk, dan memanfaatkan hasil dari b. Berikut merupakan kode awk-nya.
~~~
state1="Texas"
state2="Illinois"
awk -v state1="$state1" -v state2="$state2" 'BEGIN {FS="\t"; OFS="\t";
 PROCINFO["sorted_in"]="@val_num_asc"}
($11~state1 || $11~state2) {produk[$17]+=$21}
END{for(i in produk)print i,produk[i]}' Sample-Superstore.tsv |
awk 'BEGIN{FS="\t"}; {print $1}' | head -10
~~~
Penjelasan:
- `state1="Texas" state2="Illinois"` mendeklarasikan variable state1 dengan value "Texas" (didapat dari hasil b) dan state2 dengan value 
"Illinois" (yang juga didapat dari hasil b)
- Dalam block **BEGIN**
	- kurang lebih sama dengan yang sebelumnya, yang berbeda terdapat `OFS="\t"` yang berarti batas antar kolom pada output adalah tab
- Dalam block **body**
	- `($11~state1 || $11~state2)` mencari pattern pada kolom 11 yang memiliki data sama dengan state1 (Texas) atau state2 (Illinois)
	- `{produk[$17]+=$21}` mengisi array produk dengan jumlah "profit" (kolom 21) di mana index array produk adalah data pada kolom 17
- Dalam block **END**
	- `for(i in produk) print i, produk[i]` iterasi array produk dengan i sebagai indexnya, lalu mencetak semua index dan isi dari array
- `Sample-Superstore.tsv | awk 'BEGIN{FS="\t"}; {print $1}' | head -10`
	- `awk 'BEGIN{FS="\t"}` artinya data yang diinput (dalam hal ini merupakan output dari perintah sebelumnya) menggunakan "tab" sebagai pembatas kolom.
	- `head -10` menampilkan 10 row teratas

#### Hasil
![output_soal1](https://github.com/neutralix/SoalShiftSISOP20_modul1_F06/blob/master/Soal1/output_soal1.png)
