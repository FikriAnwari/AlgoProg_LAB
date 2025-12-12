#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

struct Items{
    int no;
    char nama[50];
    char jenis[30];
    char kategori[30];
    char sub_kategori[30];
    char metode_saji[30];
    int harga;
    int harga_total;
    int stok;
    char kode[10];
    int porsi;
};


void baca_per_baris_items(struct Items *item, FILE *file){

    char buffer[99999];

    fgets(buffer, 99999, file);

    sscanf(buffer, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d\t%d\t%[^\t]",
        &item->no, item->nama, item->jenis, item->kategori, item->metode_saji, item->sub_kategori, &item->harga, &item->stok, item->kode
    );
}

void skip_baris(FILE *file){
    char buffer[9999];
    fgets(buffer, 9999, file);
}

int jumlah_baris(FILE *file){
    char buffer[9999];
    int counter = 0;
    while(fgets(buffer, 9999, file)){
        counter++;
    }
    fseek(file, 0, SEEK_SET);
    return counter;
}

void isi_items(struct Items *items, FILE *file, int jumlah_baris){
    skip_baris(file);
    
    for(int i = 0; i < jumlah_baris; i++){
        baca_per_baris_items(&items[i], file);
    }
}

void cetak_garis(char *text, int panjang){
    for(int i = 0; i < panjang; i++){
        printf("%s", text);
    }
}

void cetak_items(struct Items items[], int jumlah, char *label){
    //mencari Nama terpanjang
    int terpanjang = 0;
    for(int i = 0; i < jumlah; i++){
        if(strlen(items[i].nama) > terpanjang){
            terpanjang = strlen(items[i].nama);
        } 
    }

    //cuma buat fleksibilitas lebar
    int space_between = 3, correction = 2;
    int no = 1, harga = 5;
    terpanjang += 0;

    printf("+");
    cetak_garis("=", no + space_between);
    cetak_garis("=", terpanjang + space_between);
    cetak_garis("=", harga + space_between);
    cetak_garis("=", correction);
    printf("+\n");  

    printf("|%-*s|\n", 
            no+terpanjang+harga+(space_between*3)+2,label
        );
    
    printf("+");
    cetak_garis("=", no + space_between);
    cetak_garis("=", terpanjang + space_between);
    cetak_garis("=", harga + space_between);
    cetak_garis("=", correction);
    printf("+\n");  

    printf("|%-*s|%-*s|%-*s|\n", 
            no+space_between,"No", 
            terpanjang+space_between, "Menu", 
            harga+space_between, "Harga"
        );
    
    printf("+");
    cetak_garis("=", no + space_between);
    cetak_garis("=", terpanjang + space_between);
    cetak_garis("=", harga + space_between);
    cetak_garis("=", correction);
    printf("+\n");

    //cetak itemsnya
    for(int i = 0; i < jumlah; i++){
        if(strcmp(label, items[i].jenis) == 0){
            printf("|%-*d|%-*s|%-*d|\n", 
            no+space_between,items[i].no, 
            terpanjang+space_between, items[i].nama, 
            harga+space_between,items[i].harga); 
        }
        
    }

    printf("+");
    cetak_garis("=", no + space_between);
    cetak_garis("=", terpanjang + space_between);
    cetak_garis("=", harga + space_between);
    cetak_garis("=", correction);
    printf("+\n");
}

//nanti dulu udah 1 jam gak kelar2
void format_rupiah(char teks[], int angka){

    char buffer[9999];
    sprintf(buffer, "%d", angka);
    int panjang = strlen(buffer);

    //membalik angkanya biar enak
    char temp[9999];
    for(int i = panjang - 1; i >= 0; i--){
        if((i-1) % 3 == 0){//000,00051
            // printf(",");
            printf("%c", buffer[i]);
        }else{
            printf("%c", buffer[i]);
        }
        // temp[i] = buffer[panjang - 1 - i];
    }

    // printf("%s", temp);

}

int cek_alfabet_dan_numeric(char teks[], int panjang){

    int counter = 0;
    for(int i =0; i < panjang; i++){
        if((teks[i] >= 'A' && teks[i] <= 'Z') || (teks[i] >= 'a' && teks[i] <= 'z') || teks[i] == ' ' ||isdigit(teks[i]) != 0){
            counter++;
        }else{
            return 0;
        }
    }

    return 1;
}

int cek_numeric(char teks[], int panjang){

    int counter = 0;
    for(int i =0; i < panjang; i++){
        if(isdigit(teks[i]) != 0){
            counter++;
        }else{
            return 0;
        }
    }

    return 1;
}

void teks_casting(char *teks, int panjang, char mode){
    if(mode == 'u'){
        for(int i =0; i < panjang; i++){
            teks[i] = toupper(teks[i]);
        }
    }else if(mode == 'l'){
        for(int i =0; i < panjang; i++){
            teks[i] = tolower(teks[i]);
        }
    }else{
        printf("\nERROR undifined mode = %c in teks_casting functions please select between(l or u)\n", mode);
    }
}

int cek_ada_dimenu_nama(struct Items items[], char teks[], int panjang){
    char dicari[9999];
    strcpy(dicari, teks);
    teks_casting(dicari, strlen(dicari), 'l');

    int counter = 0;
    for(int i = 0; i < panjang; i++){
        char items_nama[9999];
        strcpy(items_nama, items[i].nama);
        teks_casting(items_nama, strlen(items_nama),'l');

        if(strcmp(items_nama, dicari) == 0){
            counter++;
        }
    }

    return counter;
}

void input_menu(struct Items items[], struct Items pesanan[], int jumlah_baris_file, int *loop){
    char menu[9999];
    char porsi[9999];


    for(int i = 0; i < *loop; i++){

        while(1){

            printf("Menu(ketik Q untuk selesai) untuk i ke-%d:", i);
            fgets(menu, sizeof(menu), stdin);
            menu[strcspn(menu, "\n")] = '\0';

            if(menu[0] == 'q' || menu[0] == 'Q'){
                strcpy(menu, "");
                strcpy(porsi, "0");

                /*ini untuk merubah ukuran array secara dinamis jika input tidak sampai 
                20 kali maka ukuran arraynya si *loop/jumlah_pesanan akan diubah menjadi
                terkhir kali loop + 1*/ 

                *loop = i;
                return;
            }

            if(cek_alfabet_dan_numeric(menu, strlen(menu)) == 1){

                if(cek_ada_dimenu_nama(items, menu, jumlah_baris_file) != 0){
                    
                    while(1){
    
                        printf("Porsi:");
                        fgets(porsi, sizeof(porsi), stdin);
                        porsi[strcspn(porsi, "\n")] = '\0';
    
                        if(cek_numeric(porsi, strlen(porsi)) == 1 && atoi(porsi) >= 1 && atoi(porsi) <= 100){
    
                            // printf("Menu: %s x %s\n\n", menu, porsi);
    
                            break;
                        }else{
                            printf("(Input hanya berupa Numeric (1-9) maks=100)\n");
                            strcpy(porsi, "");
                        }
                    }
                break;
                }else{
                    printf("(Tidak ada dimenu)\n");
                    strcpy(menu, "");
                }

            
            }else{
                printf("(Input hanya boleh berupa Alfabet/Numeric/Spasi)\n");
                strcpy(menu, "");
            }
        

        }
        strcpy(pesanan[i].nama, menu);
        pesanan[i].porsi = atoi(porsi);
    }
}

void add_harga_pesanan_dari_porsi(struct Items items[], int jumlah_baris_file, struct Items pesanan[], int jumlah_pesanan){
    char str_pesanan[9999];
    char str_items[9999];

    for(int i =0; i < jumlah_pesanan; i++){
        strcpy(str_pesanan, pesanan[i].nama);
        teks_casting(str_pesanan, strlen(str_pesanan), 'l');

        for(int j = 0; j < jumlah_baris_file; j++){
            strcpy(str_items, items[j].nama);
            teks_casting(str_items, strlen(str_items), 'l');

            if(strcmp(str_pesanan, str_items) == 0){
                pesanan[i].harga = items[j].harga;
                pesanan[i].harga_total = items[j].harga * pesanan[i].porsi;
            }
        }
    }
}

void hapus_pesanan_per_baris(struct Items pesanan[], int *panjang, int index){

    struct Items pesanan_baru[*panjang-1];

    if(index < 0 || index >= *panjang){
        printf("Pilihan tidak valid\n");
        return;
    }

    if(index == *panjang - 1){//khusus untuk elemen terakhir
        (*panjang)--;
        return;
    }

    for(int i = 0; i < *panjang - 1; i++){
        if(i >= index){
            pesanan_baru[i] = pesanan[i+1];
            
        }else{
            pesanan_baru[i] = pesanan[i];
        }
    }

    *panjang = *panjang - 1;

    for(int i = 0; i < *panjang; i++){
        pesanan[i] = pesanan_baru[i];
    }
}

void hapus_pesanan(struct Items pesanan[], int *jumlah_pesanan){

    system("cls");
    for(int i = 0; i < *jumlah_pesanan; i++){
        printf("%d. %s x %d = Rp%d\n", (i+1),pesanan[i].nama, pesanan[i].porsi, pesanan[i].harga_total);
    }

    char str_pilihan[99];
    while(1){
        printf("pilih(ketik Q untuk cancel):");
        fgets(str_pilihan, sizeof(str_pilihan), stdin);
        str_pilihan[strcspn(str_pilihan, "\n")] = '\0';

        //back
        if(str_pilihan[0] == 'q' || str_pilihan[0] == 'Q'){
            return;
        }

        int pilihan = atoi(str_pilihan);
        if(pilihan >= 1 && pilihan <= (*jumlah_pesanan)){
            hapus_pesanan_per_baris(pesanan, jumlah_pesanan, pilihan-1);
            break;
        }else{
            printf("Input hanya berupa angka 1-%d\n", (*jumlah_pesanan));
        }
    }
}

void ubah_pesanan_per_baris(struct Items items[], int jumlah_baris_file, struct Items pesanan[], int *jumlah_pesanan, int index){
    struct Items pesanan_baru[*jumlah_pesanan];

    if(index < 0 || index >= *jumlah_pesanan){
        printf("Pilihan tidak valid\n");
        return;
    }

    system("cls");
    printf("Merubah Pesanan\n");
    printf("%s x %d = Rp%d\n", pesanan[index].nama, pesanan[index].porsi, pesanan[index].harga_total);

    char nama_sebelum[9999];
    strcpy(nama_sebelum, pesanan[index].nama);
    int porsi_sebelum = pesanan[index].porsi;
    int harga_total_sebelum = pesanan[index].harga_total;

    char str_nama[9999];
    char str_porsi[9999];
    while(1){
        printf("Nama(ketik Q untuk cancel):");
        fgets(str_nama, sizeof(str_nama), stdin);
        str_nama[strcspn(str_nama, "\n")] = '\0';
        //back
        if(str_nama[0] == 'q' || str_nama[0] == 'Q'){
            strcpy(pesanan[index].nama, nama_sebelum);
            pesanan[index].porsi = porsi_sebelum;
            pesanan[index].harga_total = harga_total_sebelum;
            return;
        }

        if(cek_ada_dimenu_nama(items, str_nama, jumlah_baris_file) != 0){
            strcpy(pesanan[index].nama, str_nama);

            for(int i = 0; i < jumlah_baris_file; i++){
                char str_nama_temp[9999];
                char str_items_nama_temp[9999];
                strcpy(str_nama_temp, str_nama);
                strcpy(str_items_nama_temp, items[i].nama);
                teks_casting(str_nama_temp, strlen(str_nama_temp), 'l');
                teks_casting(str_items_nama_temp, strlen(str_items_nama_temp), 'l');

                if(strcmp(str_nama_temp, str_items_nama_temp) == 0){
                    pesanan[index].harga = items[i].harga;
                }
            }

            printf("Porsi(ketik Q untuk cancel):");
            fgets(str_porsi, sizeof(str_porsi), stdin);
            str_porsi[strcspn(str_porsi, "\n")] = '\0';
    
            //back
            if(str_porsi[0] == 'q' || str_porsi[0] == 'Q'){
                strcpy(pesanan[index].nama, nama_sebelum);
                pesanan[index].porsi = porsi_sebelum;
                pesanan[index].harga_total = harga_total_sebelum;
                return;
            }
    
            if(cek_numeric(str_porsi, strlen(str_porsi)) == 1 && atoi(str_porsi) >= 1 && atoi(str_porsi) <= 100){
                int porsi = atoi(str_porsi);
                pesanan[index].porsi = porsi;
                pesanan[index].harga_total = porsi * pesanan[index].harga;
                return;
            }else{
                printf("(Input hanya berupa Numeric maks=100)\n");
                continue;
            }

        }else{
            printf("(Pilihan mu tidak ada di menu)\n");
            continue;
        }

    }
}

void ubah_pesanan(struct Items items[], int jumlah_baris_file, struct Items pesanan[], int *jumlah_pesanan){
    
    system("cls");
    cetak_items(items, jumlah_baris_file, "Makanan");
    printf("\n");
    cetak_items(items, jumlah_baris_file, "Minuman");
    printf("\n");
    printf("==MENGUBAH PESANAN==\n");
    for(int i = 0; i < *jumlah_pesanan; i++){
        printf("%d. %s x %d = Rp%d\n", (i+1),pesanan[i].nama, pesanan[i].porsi, pesanan[i].harga_total);
    }

    char str_pilihan[99];
    while(1){   
        printf("pilih(ketik Q untuk cancel):");
        fgets(str_pilihan, sizeof(str_pilihan), stdin);
        str_pilihan[strcspn(str_pilihan, "\n")] = '\0';

        //back
        if(str_pilihan[0] == 'q' || str_pilihan[0] == 'Q'){
            return;
        }

        int pilihan = atoi(str_pilihan);
        if(pilihan >= 1 && pilihan <= (*jumlah_pesanan)){
            ubah_pesanan_per_baris(items, jumlah_baris_file, pesanan, jumlah_pesanan, pilihan-1);
            break;
        }else{
            printf("Input hanya berupa angka 1-%d\n", (*jumlah_pesanan));
            strcpy(str_pilihan, "");
            continue;
        }
    }

}

void tambah_pesanan(struct Items items[], int jumlah_baris_file, struct Items pesanan[], int *jumlah_pesanan){
    
    system("cls");
    cetak_items(items, jumlah_baris_file, "Makanan");
    printf("\n");
    cetak_items(items, jumlah_baris_file, "Minuman");
    printf("\n");
    printf("==MENAMBAH PESANAN==\n");
    for(int i = 0; i < *jumlah_pesanan; i++){
        printf("%d. %s x %d = Rp%d\n", (i+1),pesanan[i].nama, pesanan[i].porsi, pesanan[i].harga_total);
    }

    char str_nama[9999];
    char str_porsi[99];
    while(1){
        printf("Nama(ketik Q untuk cancel):");
        fgets(str_nama, sizeof(str_nama), stdin);
        str_nama[strcspn(str_nama, "\n")] = '\0';
        
        //back
        if(str_nama[0] == 'q' || str_nama[0] == 'Q'){
            return;
        }
        
        if(cek_ada_dimenu_nama(items, str_nama, jumlah_baris_file) != 0){
            
            printf("Porsi(ketik Q untuk cancel):");
            fgets(str_porsi, sizeof(str_porsi), stdin);
            str_porsi[strcspn(str_porsi, "\n")] = '\0';

            //back
            if(str_porsi[0] == 'q' || str_porsi[0] == 'Q'){
                return;
            }

            int porsi = atoi(str_porsi);

            if(cek_numeric(str_porsi, strlen(str_porsi)) == 1 && porsi >= 1){

                (*jumlah_pesanan)++;
                strcpy(pesanan[*jumlah_pesanan-1].nama, str_nama);
                pesanan[*jumlah_pesanan-1].porsi = porsi;

                for(int i = 0; i < jumlah_baris_file; i++){
                    char str_nama_temp[9999];
                    char str_items_nama_temp[9999];
                    strcpy(str_nama_temp, str_nama);
                    strcpy(str_items_nama_temp, items[i].nama);
                    teks_casting(str_nama_temp, strlen(str_nama_temp), 'l');
                    teks_casting(str_items_nama_temp, strlen(str_items_nama_temp), 'l');

                    if(strcmp(str_nama_temp, str_items_nama_temp) == 0){
                        pesanan[*jumlah_pesanan-1].harga = items[i].harga;
                    }

                    return;
                }
            }else{
                printf("(Input hanya berupa Numeric)\n");
                continue;
            }

        }else{
            printf("(Pilihan mu tidak ada di menu)\n");
            continue;
        }
    }
}

void konfirmasi_pesanan(struct Items pesanan[], int jumlah_pesanan, char nama_file[], char kasir[]){

    //proses file
    FILE *fp = fopen(nama_file, "a");
    if(fp == NULL){
        printf("Gagal membuka file\n");
        return;
    }

    //untuk waktu
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char waktu[999];
    strcpy(waktu, asctime(timeinfo));
    waktu[strcspn(waktu, "\n")] = '\0';

    //cetak ke file
    for(int i = 0; i < jumlah_pesanan; i++){
        fprintf(fp, "%d\t%s\t%d\t%d\t%s\t%s\t%d\n", i+1, pesanan[i].nama, pesanan[i].porsi, pesanan[i].harga_total, waktu, kasir, i);
    }

    fclose(fp);
    memset(pesanan, 0, jumlah_pesanan * sizeof(pesanan[0]));
}

void edit_hapus_konfirmasi_pesanan(struct Items items[], int jumlah_baris_file, struct Items pesanan[], int *jumlah_pesanan, char kasir[]){
    
    char str_input[99];
    
    while(1){
        system("cls");
        printf("\n");
        printf("Pesanan Anda:\n");
        int total_pembayaran = 0;
        for(int i= 0; i < *jumlah_pesanan; i++){
            printf("%d. %s x %d = Rp%d\n", i+1, pesanan[i].nama, pesanan[i].porsi, pesanan[i].harga_total);
            total_pembayaran += pesanan[i].harga_total;
        }
        printf("TOTAL: Rp%d\n", total_pembayaran);
        printf("\n");
        printf("(1)Hapus 1 pesanan\n");
        printf("(2)Ubah pesanan\n");
        printf("(3)Tambah 1 pesanan\n");
        printf("(4)Konfirmasi pesanan\n");
        printf("pilih(Q untuk kembali):");
        fgets(str_input, sizeof(str_input), stdin);
        str_input[strcspn(str_input, "\n")] = '\0';

        //back
        if(str_input[0] == 'q' || str_input[0] == 'Q'){
            return;
        }
        
        int input = atoi(str_input);

        if(input >= 1 && input <= 4){
            if(input == 1){
                char hapus_input[99];
                hapus_pesanan(pesanan, jumlah_pesanan);
                continue;
            }else if(input == 2){
                ubah_pesanan(items, jumlah_baris_file, pesanan, jumlah_pesanan);
                continue;
            }else if(input == 3){
                tambah_pesanan(items, jumlah_baris_file, pesanan, jumlah_pesanan);
                continue;
            }else if(input == 4){
                konfirmasi_pesanan(pesanan, *jumlah_pesanan, "record_menu.csv", kasir);
                break;
            }else{
                printf("(Input hanya berupa angka 1-4)\n");
            }
        }else{
            printf("(Input hanya berupa angka 1-4)\n");
        }
    }

}

void urutkan_menu(struct Items items[], int jumlah_baris_file, char tipe, char urutan){

    if(tipe == 'a' || tipe == 'A'){
        if(urutan == 'a' || urutan == 'A'){

            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(strcasecmp(items[j].nama, items[i].nama) < 0) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }

        }else if(urutan == 'd' || urutan == 'D'){
            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(strcasecmp(items[j].nama, items[i].nama) < 0) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }
        }
    }else if(tipe == 'h' || tipe == 'H'){
        if(urutan == 'a' || urutan == 'A'){

            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(items[j].harga < items[i].harga) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }

        }else if(urutan == 'd' || urutan == 'D'){
            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(items[j].harga > items[i].harga) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }
        }
    }else if(tipe == 'n' || tipe == 'N'){
        if(urutan == 'a' || urutan == 'A'){

            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(items[j].no < items[i].no) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }

        }else if(urutan == 'd' || urutan == 'D'){
            for(int i = 0; i < jumlah_baris_file - 1; i++){
                    for(int j = i+1; j < jumlah_baris_file; j++){
                        if(items[j].no > items[i].no) {
                            struct Items temp = items[i];
                            items[i] = items[j];
                            items[j] = temp;
                        }
                    }
                }
        }
    }else{
        printf("(Input salah! pilih 'h' untuk harga, 'a' untuk alfabet, 'n' untuk nomor)\n");
        printf("(Lalu pilih 'a' untuk ascending, pilih 'd' untuk descending)\n");
        return;
    }
}

void input_urutkan_menu(struct Items items[], int jumlah_baris_file){
    
    char tipe;
    char urutan;
    while(1){
        printf("Urutkan berdasarkan harga/alfabet/no (h/a/n) (Q untuk kembali):");
        scanf(" %c", &tipe);

        //back
        if(tipe == 'q' || tipe == 'Q'){
            return;
        }
        
        if(tipe == 'h' || tipe == 'H' || tipe == 'a' || tipe == 'A' || tipe == 'n' || tipe == 'N'){
            printf("Pilih ascending/descending (a/d):");
            scanf(" %c", &urutan);

            //back
            if(urutan == 'q' || urutan == 'Q'){
                return;
            }

            if(urutan == 'd' || urutan == 'D' || urutan == 'a' || urutan == 'A'){
                urutkan_menu(items, jumlah_baris_file, tipe, urutan);
                return;
            }else{
                printf("(Input salah! pilih 'a' untuk ascending, pilih 'd' untuk descending)\n");
                continue;
            }
        }else{
            printf("(Input salah! pilih 'h' untuk harga, 'a' untuk alfabet, 'n' untuk nomor)\n");
            continue;
        }
    }
}

void cari_teks(char cari[], char teks[]){
    int panjang_cari = strlen(cari);
    int panjang_teks = strlen(teks);

    int counter = 0;

    for(int i = 0; i < panjang_teks; i++){
        
        if(teks[i] == cari[0]){
            for(int j = 0; j < panjang_cari; j++){
                if(teks[i + j] == cari[j]){
                    counter++;
                }
            }

        }
    }

    return counter;
}

void cari_pesanan(struct Items items[], int jumlah_baris_file, struct Items target[], int *panjang_target, char input[]){
    for(int i = 0; i < jumlah_baris_file; i++){
        char harga_temp[99];
        sprintf(harga_temp, "%d", items[i].harga);

        if(strcasecmp(input, items[i].nama) == 0){
            target[i] = items[i];
            *panjang_target = i;
        }else if(strcasecmp(input, harga_temp) == 0){
            target[i] = items[i];
            *panjang_target = i;
        }else if(strcasecmp(input, items[i].kode) == 0){
            target[i] = items[i];
            *panjang_target = i;
        }else if(strcasestr(items[i].nama, input) != NULL){
            target[i] = items[i];
            *panjang_target = i;
        }else{
            printf("Tidak Ditemukan\n");
        }
    }
}

void input_cari_pesanan(struct Items items[], int jumlah_baris_file){
    char input[99];

    while(1){
        printf("Cari(Q untuk cancel/kembali):");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
    
        //back
        if(input[0] == 'q' || input[0] == 'Q'){
            return;
        }
    
        int panjang_target = 100;
        struct Items target[panjang_target];
    
        if(cek_alfabet_dan_numeric(input, strlen(input)) == 1){
            cari_pesanan(items, jumlah_baris_file, target, &panjang_target, input);
            cetak_items(target, panjang_target, "Makanan");
            cetak_items(target, panjang_target, "Minuman");
        }else{
            printf("(Input hanya boleh berupa Alfabet/Numeric/Spasi)\n");
            continue;
        }
    }

}


int main(){

    FILE *fp = fopen("makanan_minuman.csv", "r");
    if(fp == NULL){
        printf("Gagal membuka file silahkan cek lokasi file\n");
    }

    int jumlah_baris_file = jumlah_baris(fp);
    jumlah_baris_file -= 1;
    struct Items items[jumlah_baris_file];
    
    int jumlah_pesanan = 5;
    struct Items dibeli[jumlah_pesanan];

    //proses memasukan data dari file ke program
    isi_items(items, fp, jumlah_baris_file);
    fclose(fp);

    //Menu Dibagian Pembelian

    while(1){
        char str_input[3];
        system("cls");

        //mencetak menu
        cetak_items(items, jumlah_baris_file, "Makanan");
        printf("\n");
        cetak_items(items, jumlah_baris_file, "Minuman");

        printf("(1)Urutkan Menu\n");
        printf("(2)Cari Menu\n");
        printf("(3)Buat Pesanan\n");
        printf("(4)Ubah/Hapus/Konfirmasi Pesanan\n");

        // printf("Jumlah Baris = %d\n", jumlah_baris_file);
        // for(int i = 0; i < jumlah_baris_file; i++){
        //     printf("[%d] %s\n", items[i].no, items[i].nama);
        // }
        if(strcmp(dibeli[0].nama, "") != 0){
            int h = 0;
            printf("\n");
            printf("==PESANAN ANDA==\n");
            for(int i = 0; i < jumlah_pesanan; i++){
                printf("%d. %s x %d = Rp%d\n", i+1, dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
                h += dibeli[i].harga_total;
            }
            printf("Total = Rp%d\n", h);
            printf("Silahkan pilih 4 untuk konfirmasi\n");
        }

        printf("pilih:");
        fgets(str_input, sizeof(str_input), stdin);
        str_input[strcspn(str_input, "\n")] = '\0';
        int input = atoi(str_input);

        if(input >= 1 && input <= 4){
            if(input == 1){
                input_urutkan_menu(items, jumlah_baris_file);
            }else if(input == 2){

            }else if(input == 3){
                printf("Silahkan masukkan menu yang di pilih dengan mengetikan No / Nama / Kode:\n");
        
                input_menu(items, dibeli, jumlah_baris_file, &jumlah_pesanan);
                printf("\n");

                add_harga_pesanan_dari_porsi(items, jumlah_baris_file, dibeli, jumlah_pesanan);
                printf("\n");
            }else if(input == 4){
                edit_hapus_konfirmasi_pesanan(items, jumlah_baris_file, dibeli, &jumlah_pesanan, "Dinda Ayudinda");
                printf("\n");
                printf("==PESANAN ANDA==\n");
                for(int i =0; i < jumlah_pesanan; i++){
                    printf("%d. %s x %d = Rp%d\n", i+1, dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
                }
            }
        }else{
            printf("(Input hanya berupa angka 1-4)\n");
            continue;
        }
    }   
    
    getchar();

    return 0;
}