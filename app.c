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

// data login
typedef struct {
    char username[20];
    char password[20];
} User;

User users[] = {
    {"fikri", "fikri"},
    {"zakhi", "zakhi"},
    {"jasten", "jasten"},
    {"admin", "admin"}
};

int jumlahUser = 4;

int login(char nama[]) {
    char username[20], password[20];

    printf("=== LOGIN STAFF ===\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    for (int i = 0; i < jumlahUser; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            printf("\nLogin berhasil!\n\n");
            strcpy(nama, username);
            return 1;
        }
    }

    printf("\nLogin gagal, coba lagi!\n\n");
    return 0;
}

#define BATASAN_PESANAN 20
void baca_per_baris_items(struct Items *item, FILE *file){

    char buffer[99999];

    fgets(buffer, 99999, file);

    sscanf(buffer, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d\t%d\t%[^\n]",
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
    int space_between = 3, correction = 4;
    int no = 1, harga = 5, rp = 2;
    terpanjang += 0;

    printf("+");
    cetak_garis("=", no + space_between);
    cetak_garis("=", terpanjang + space_between);
    cetak_garis("=", harga + space_between);
    cetak_garis("=", correction);
    printf("+\n");  

    printf("|%-*s|\n", 
            no+terpanjang+harga+(space_between*3)+2+rp,label
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
            harga+space_between+rp, "Harga"
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
            printf("|%-*d|%-*s|Rp%-*d|\n", 
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

    for(int i = 0; i < panjang; i++){
        char items_nama[9999];
        strcpy(items_nama, items[i].nama);
        teks_casting(items_nama, strlen(items_nama),'l');

        if(strcmp(items_nama, dicari) == 0){
            strcpy(teks, items[i].nama);
            return 1;
        }
    }

    return 0;
}

int cek_ada_dimenu_no(struct Items items[], char teks[], int panjang){
    int dicari = atoi(teks);

    for(int i = 0; i < panjang; i++){
        if(dicari == items[i].no){
            strcpy(teks, items[i].nama);
            return 1;
        }
    }

    return 0;
}

int cek_ada_dimenu_kode(struct Items items[], char teks[], int panjang){
    char dicari[10];
    strcpy(dicari, teks);

    for(int i = 0; i < panjang; i++){
        if(strcmp(dicari, items[i].kode) == 0){
            strcpy(teks, items[i].nama);
            return 1;
        }
    }

    return 0;
}

void input_menu(struct Items items[], struct Items pesanan[], int jumlah_baris_file, int *loop){
    char menu[9999];
    char porsi[9999];
    int ada_dimenu_nama = 0;
    int ada_dimenu_no = 0;
    int ada_dimenu_kode = 0;

    *loop = BATASAN_PESANAN;
    for(int i = 0; i < *loop; i++){

        while(1){

            printf("Menu(ketik Q untuk selesai):");
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
                ada_dimenu_nama = cek_ada_dimenu_nama(items, menu, jumlah_baris_file);
                ada_dimenu_no = cek_ada_dimenu_no(items, menu, jumlah_baris_file);
                ada_dimenu_kode = cek_ada_dimenu_kode(items, menu, jumlah_baris_file);

                if(ada_dimenu_nama != 0 || ada_dimenu_no != 0 || ada_dimenu_kode != 0){
                    
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
        //untuk memasukkan menu ke dalam array pesanan
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

    int ada_dimenu_nama = 0;
    int ada_dimenu_no = 0;
    int ada_dimenu_kode = 0;

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

        ada_dimenu_nama = cek_ada_dimenu_nama(items, str_nama, jumlah_baris_file);
        ada_dimenu_no = cek_ada_dimenu_no(items, str_nama, jumlah_baris_file);
        ada_dimenu_kode = cek_ada_dimenu_kode(items, str_nama, jumlah_baris_file);

        if(ada_dimenu_nama != 0 || ada_dimenu_no != 0 || ada_dimenu_kode != 0){
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

    int ada_dimenu_nama = 0;
    int ada_dimenu_no = 0;
    int ada_dimenu_kode = 0;

    while(1){
        printf("Nama(ketik Q untuk cancel):");
        fgets(str_nama, sizeof(str_nama), stdin);
        str_nama[strcspn(str_nama, "\n")] = '\0';
        
        //back
        if(str_nama[0] == 'q' || str_nama[0] == 'Q'){
            return;
        }

        ada_dimenu_nama = cek_ada_dimenu_nama(items, str_nama, jumlah_baris_file);
        ada_dimenu_no = cek_ada_dimenu_no(items, str_nama, jumlah_baris_file);
        ada_dimenu_kode = cek_ada_dimenu_kode(items, str_nama, jumlah_baris_file);
        
        if(ada_dimenu_nama != 0 || ada_dimenu_no != 0 || ada_dimenu_kode != 0){
            
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
                    // printf("%s\n", str_nama);
                    // printf("%s\n", items[i].nama);
                    // printf("%d\n", jumlah_baris_file);
                    
                    if(strcasecmp(str_nama, items[i].nama) == 0){
                        printf("Masuk kedalam sini\n");
                        printf("nama yg input = %s\n", str_nama);
                        printf("nama yg ada di items = %s\n", items[i].nama);
                        printf("porsi yg di input = %d\n", porsi);
                        printf("harga yg ada di items = %d\n", items[i].harga);

                        pesanan[*jumlah_pesanan-1].harga = items[i].harga;
                        pesanan[*jumlah_pesanan-1].harga_total = items[i].harga * porsi;
                        return;
                    }
                    
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

    if(pesanan[0].nama == "" || pesanan[0].harga == 0){
        return;
    }
    
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
                        if(strcasecmp(items[j].nama, items[i].nama) > 0) {
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

int cari_teks(char cari[], char teks[]){
    int panjang_cari = strlen(cari);
    int panjang_teks = strlen(teks);

    int counter = 0;

    for(int i = 0; i < panjang_teks; i++){
        counter = 0;

        if(tolower(teks[i]) == tolower(cari[0])){
            
            for(int j = 0; j < panjang_cari; j++){
                if(tolower(teks[i + j]) == tolower(cari[j])){
                    counter++;
                }

                if(counter == panjang_cari){
                    return 1;
                }
            }

        }
    }

    return 0;
}

void cari_pesanan(struct Items items[], int jumlah_baris_file, struct Items target[], int *panjang_target, char input[]){
    
    int counter = 0;

    for(int i = 0; i < jumlah_baris_file; i++){

        char harga_temp[99];
        sprintf(harga_temp, "%d", items[i].harga);

        if(strcasecmp(input, harga_temp) == 0){
            target[counter] = items[i];
            counter++;
        }else if(strcmp(input, items[i].kode) == 0){
            target[counter] = items[i];
            counter++;
        }else if(cari_teks(input, items[i].nama) == 1){
            target[counter] = items[i];
            counter++;
        }

        *panjang_target = counter;
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

            if(panjang_target == 0){
                printf("(Tidak Ditemukan)\n");
                continue;
            }

            cetak_items(target, panjang_target, "Makanan");
            printf("\n");
            cetak_items(target, panjang_target, "Minuman");
        }else{
            printf("(Input hanya boleh berupa Alfabet/Numeric/Spasi)\n");
            continue;
        }
    }

}

void record_pembelian(FILE *file){
    system("cls");

    // Cek apakah file kosong
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET); // Kembali ke awal
    
    if(file_size <= 0){
        printf("File kosong, tidak ada data yang dimuat\n");
        fclose(file);
        return;
    }

    char buffer[9999];

    int no;
    char nama[999];
    int porsi;
    int harga;
    char tanggal[999];
    char staff[99];
    int index;
    
    int loop = 0;
    int counter = 0;
    while(fgets(buffer, sizeof(buffer), file)) {
        // Hapus newline di akhir
        buffer[strcspn(buffer, "\n")] = 0;
        sscanf(buffer, "%d\t%[^\t]\t%d\t%d\t%[^\t]\t%[^\t]\t%d",
                   &no, nama, &porsi, &harga, tanggal, staff, &index);

        loop++;
        if(index == 0){
            counter++;
            loop = 0;
            printf("\n");
            printf("== PESANAN KE %d ==\n", counter);
        }

        printf("%d. %s x %d = Rp%d [%s] kasir=%s\n", 
            loop+1, nama, porsi, harga, tanggal, staff);
    }

    fclose(file);

    while(1){
    printf("\nQ untuk kembali, C untuk MENGHAPUS data Record:");
    char input[2];
    fgets(input, sizeof(input), stdin);

        if(input[0] == 'q' || input[0] == 'Q'){
            return;
        }else if(input[0] == 'c' || input[0] == 'C'){
            file = fopen("record_menu.csv", "w");
            fclose(file);
            return;
        }
    }

}

#define MAX 200

struct Reservasi {
    int nomor_meja;
    int kapasitas;
    char tipe_meja[30];
    char ruangan[30];
    char status[20];
    char jadwal[50];
};

struct Reservasi data[MAX];
int total = 0;

void trim(char *s) {
    int len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

void load() {
    FILE *f = fopen("reservasi.csv", "r");
    if(f == NULL){
        printf("Gagal membuka file silahkan cek lokasi file\n");
    }
    char line[256];
    fgets(line, sizeof(line), f);
    total = 0;
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        char *t = strtok(line, ";"); if (!t) continue;
        data[total].nomor_meja = atoi(t);
        t = strtok(NULL, ";"); data[total].kapasitas = atoi(t);
        t = strtok(NULL, ";"); strcpy(data[total].tipe_meja, t);
        t = strtok(NULL, ";"); strcpy(data[total].ruangan, t);
        t = strtok(NULL, ";"); strcpy(data[total].status, t);
        t = strtok(NULL, ";"); strcpy(data[total].jadwal, t);
        total++;
    }
    fclose(f);
}

void save() {
    FILE *f = fopen("reservasi.csv", "w");
    fprintf(f, "nomor_meja;kapasitas;tipe_meja;ruangan;status;jadwal\n");
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%d;%s;%s;%s;%s\n",
            data[i].nomor_meja,
            data[i].kapasitas,
            data[i].tipe_meja,
            data[i].ruangan,
            data[i].status,
            data[i].jadwal
        );
    }
    fclose(f);
}

void tampil() {
    system("cls");
    printf("\n%-12s %-10s %-12s %-12s %-12s %-20s\n",
           "Meja", "Kapasitas", "Tipe", "Ruangan", "Status", "Jadwal");
    for (int i = 0; i < total; i++) {
        printf("%-12d %-10d %-12s %-12s %-12s %-20s\n",
               data[i].nomor_meja,
               data[i].kapasitas,
               data[i].tipe_meja,
               data[i].ruangan,
               data[i].status,
               data[i].jadwal);
    }
}

int cari(int meja) {
    for (int i = 0; i < total; i++)
        if (data[i].nomor_meja == meja) return i;
    return -1;
}

int cek_format_tanggal(char teks[]){
    int year;
    int month;
    int day;
    int hour;
    int minute;

    sscanf(teks, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute);

    if((year <= 2100 && year > 0) && (month <= 12 && month > 0) && (day <= 31 && day > 0) && (hour <= 23 && hour >= 0) && (minute <= 59 && minute >= 0)){
        return 1;
    }else{
        return 0;
    }
    
}

void reservasi() {
    system("cls");
    tampil();
    char str_meja[9];
    int meja;
    char jadwal[50];
    printf("Nomor meja: ");
    fgets(str_meja, sizeof(str_meja), stdin);
    meja = atoi(str_meja);

    int idx = cari(meja);
    if (idx < 0) return;
    if (strcmp(data[idx].status, "booked") == 0) {
        printf("Meja sudah dibooking.\n");
        return;
    }
    printf("Masukkan jadwal (YYYY-MM-DD HH:MM): ");
    fgets(jadwal, sizeof(jadwal), stdin); 
    if(cek_format_tanggal(jadwal) == 0){
        printf("Pastikan format tanggal benar\n");
        return;
    }
    trim(jadwal);
    strcpy(data[idx].status, "booked");
    strcpy(data[idx].jadwal, jadwal);
    save();
    printf("Reservasi berhasil.\n");
}

void cancel() {
    system("cls");
    tampil();
    char str_meja[9];
    int meja;
    printf("Nomor meja: ");
    fgets(str_meja, sizeof(str_meja), stdin);
    meja = atoi(str_meja);

    int idx = cari(meja);
    if (idx < 0) return;
    if (strcmp(data[idx].status, "available") == 0) {
        printf("Meja belum direservasi.\n");
        return;
    }
    strcpy(data[idx].status, "available");
    strcpy(data[idx].jadwal, "-");
    save();
    printf("Reservasi dibatalkan.\n");
}

void ganti() {
    system("cls");
    tampil();
    char str_lama[9], str_baru[9];
    int lama, baru;

    printf("Meja lama: ");
    fgets(str_lama, sizeof(str_lama), stdin);
    lama = atoi(str_lama);

    int i1 = cari(lama);
    if (i1 < 0) return;
    if (strcmp(data[i1].status, "booked") != 0) {
        printf("Meja lama tidak sedang direservasi.\n");
        return;
    }
    printf("Meja baru: ");
    fgets(str_baru, sizeof(str_baru), stdin);
    baru = atoi(str_baru);
    
    int i2 = cari(baru);
    if (i2 < 0) return;
    if (strcmp(data[i2].status, "available") != 0) {
        printf("Meja baru tidak tersedia.\n");
        return;
    }
    strcpy(data[i2].status, "booked");
    strcpy(data[i2].jadwal, data[i1].jadwal);
    strcpy(data[i1].status, "available");
    strcpy(data[i1].jadwal, "-");
    save();
    printf("Meja berhasil dipindah.\n");
}


int main(){

    char nama_login[999];
    while (!login(nama_login)) {}

    FILE *fp = fopen("makanan_minuman.csv", "r");
    if(fp == NULL){
        printf("Gagal membuka file silahkan cek lokasi file\n");
    }

    int jumlah_baris_file = jumlah_baris(fp);
    jumlah_baris_file -= 1;
    struct Items items[jumlah_baris_file];
    
    int jumlah_pesanan = BATASAN_PESANAN;
    struct Items dibeli[jumlah_pesanan];

    //proses memasukan data dari file ke program
    isi_items(items, fp, jumlah_baris_file);
    fclose(fp);









    //MENU UTAMA
    char str_pilihan[9];
    int pilihan;

    while(1){
    system("cls");
    printf("=== MENU UTAMA ===\n");
    printf("1. Daftar Makanan dan Minuman\n");
    printf("2. Reservasi\n");
    printf("3. Record Pembelian\n");
    printf("4. Keluar\n");
    printf("Pilih menu: ");
    fgets(str_pilihan, sizeof(str_pilihan), stdin);
    pilihan = atoi(str_pilihan);

    //Menu Dibagian Pembelian (1)
    if(pilihan == 1){
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
            printf("(5)Keluar\n");

            if(strcasecmp(dibeli[0].nama, "") != 0){ //memastikan tidak geprint kalau tidak ada yg dibeli
                if(jumlah_pesanan > 0){
                    int h = 0;
                    printf("\n");
                    printf("==PESANAN ANDA==\n");
                    for(int i = 0; i < jumlah_pesanan; i++){
                        printf("%d. %s x %d = Rp%d\n", i+1, dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
                        h += dibeli[i].harga_total;
                    }
                    printf("Total = Rp%d\n", h);
                    printf("\n");
                    printf("Silahkan pilih 4 untuk konfirmasi\n");
                }
            }    
            
            printf("pilih:");
            fgets(str_input, sizeof(str_input), stdin);
            str_input[strcspn(str_input, "\n")] = '\0';
            int input = atoi(str_input);

            if(input >= 1 && input <= 5){
                if(input == 1){
                    input_urutkan_menu(items, jumlah_baris_file);
                }else if(input == 2){
                    system("cls");
                    input_cari_pesanan(items, jumlah_baris_file);
                }else if(input == 3){
                    printf("Silahkan masukkan menu yang di pilih dengan mengetikan No / Nama / Kode:\n");
            
                    input_menu(items, dibeli, jumlah_baris_file, &jumlah_pesanan);
                    printf("\n");

                    add_harga_pesanan_dari_porsi(items, jumlah_baris_file, dibeli, jumlah_pesanan);
                    printf("\n");
                }else if(input == 4){
                    if(jumlah_pesanan <= 0){//memastikan tidak masuk menu ini kalau memang belum pesan / pesanannya kosong
                        continue;
                    }

                    edit_hapus_konfirmasi_pesanan(items, jumlah_baris_file, dibeli, &jumlah_pesanan, nama_login);
                    printf("\n");
                    printf("==PESANAN ANDA==\n");
                    for(int i =0; i < jumlah_pesanan; i++){
                        printf("%d. %s x %d = Rp%d\n", i+1, dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
                    }
                }else if(input == 5){
                    break;
                }
            }else{
                printf("(Input hanya berupa angka 1-5)\n");
                continue;
            }
        }   
    }else if(pilihan == 2){
        load();
        while (1) {
            char str_p[9];
            int p;
            printf("\n--- Sistem Reservasi Meja ---\n");
            printf("1. Cek Availability\n");
            printf("2. Reservasi Meja\n");
            printf("3. Cancel Reservasi\n");
            printf("4. Ganti Meja\n");
            printf("5. Exit\n");
            printf("Pilih: ");
            fgets(str_p, sizeof(str_p), stdin);
            p = atoi(str_p);

            if (p == 1) tampil();
            else if (p == 2) reservasi();
            else if (p == 3) cancel();
            else if (p == 4) ganti();
            else if (p == 5) break;
        }
    }else if(pilihan == 3){
        FILE *fp2 = fopen("record_menu.csv", "r");
        if(fp2 == NULL){
            printf("Gagal membuka file\n");
            continue;
        }

        record_pembelian(fp2);
        
    }else if(pilihan == 4){
        printf("\nTerima kasih!\n");
        break;
    }else{
        printf("(Input tidak valid)\n");
        continue;
    }

    }
    
    printf("Press [Enter] key to exit\n");
    getchar();

    return 0;
} 