# SoalShiftSISOP20_modul2_F06
## Contents

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:
a. Program menerima 4 argumen berupa:
i. Detik: 0-59 atau * (any value)
ii. Menit: 0-59 atau * (any value)
iii. Jam: 0-23 atau * (any value)
iv. Path file .sh
b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
sesuai
c. Program hanya menerima 1 config cron
d. Program berjalan di background (daemon)
e. Tidak boleh menggunakan fungsi system()
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

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
