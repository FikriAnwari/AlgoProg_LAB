#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

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
    
    for(int i = 0; i < jumlah_baris - 1; i++){
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
    for(int i = 0; i < jumlah -1; i++){
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
            scanf("%[^\n]", menu); getchar();

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
                        scanf("%[^\n]", porsi); getchar();
    
                        if(cek_numeric(porsi, strlen(porsi)) == 1){
    
                            // printf("Menu: %s x %s\n\n", menu, porsi);
    
                            break;
                        }else{
                            printf("(Input hanya berupa Numeric)\n");
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
        printf("Index tidak valid\n");
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
        scanf("%s", str_pilihan);

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

void edit_hapus_pesanan(struct Items pesanan[], int *jumlah_pesanan){
    
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
        printf("pilih:");
        scanf("%s", str_input);
        
        int input = atoi(str_input);

        if(input >= 1 && input <= 4){
            if(input == 1){
                char hapus_input[99];
                //hapus
                hapus_pesanan(pesanan, jumlah_pesanan);
                continue;
            }else if(input == 2){
                //ubah 1
                printf("UBAH\n");
                continue;
            }else if(input == 3){
                printf("TAMBAH\n");
                continue;;
            }else if(input == 4){
                printf("KONFIRMASI\n");
                break;
            }else{
                printf("Input hanya berupa angka 1-3\n");
            }
        }else{
            printf("Input hanya berupa angka 1-3\n");
        }
    }

}


int main(){

    FILE *fp = fopen("makanan_minuman.csv", "r");
    if(fp == NULL){
        printf("Gagal membuka file silahkan cek lokasi file\n");
    }

    int jumlah_baris_file = jumlah_baris(fp);
    struct Items items[jumlah_baris_file];
    
    int jumlah_pesanan = 5;
    struct Items dibeli[jumlah_pesanan];


    isi_items(items, fp, jumlah_baris_file);

    cetak_items(items, jumlah_baris_file, "Makanan");
    printf("\n");
    cetak_items(items, jumlah_baris_file, "Minuman");

    // printf("(1) Pesan menu\n");
    // printf("(2) Cari di menu\n");

    // int pilihan_menu;
    // scanf("%d", &pilihan_menu);

    printf("Silahkan masukkan menu yang di pilih dengan mengetikan No / Nama / Kode:\n");
    
    input_menu(items, dibeli, jumlah_baris_file, &jumlah_pesanan);
    printf("\n");
    // for(int i =0; i < jumlah_pesanan; i++){
    //     printf("%s * %d\n", dibeli[i].nama, dibeli[i].porsi);
    // }
    add_harga_pesanan_dari_porsi(items, jumlah_baris_file, dibeli, jumlah_pesanan);
    printf("\n");
    // for(int i =0; i < jumlah_pesanan; i++){
    //     printf("%s x %d = Rp%d\n", dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
    // }
    edit_hapus_pesanan(dibeli, &jumlah_pesanan);

    printf("\n");
    for(int i =0; i < jumlah_pesanan; i++){
        printf("%s x %d = Rp%d\n", dibeli[i].nama, dibeli[i].porsi, dibeli[i].harga_total);
    }
    


    // //konfirmasi
    // printf("Pastikan menu yang anda pesan sudah sesuai\n");
    // printf("(1)Konfirmasi pesanan\n");
    // printf("(2)Edit kembali pesanan\n");
    
    

    getchar();

    return 0;
}