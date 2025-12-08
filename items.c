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
    int stok;
    char kode[10];
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


int main(){

    FILE *fp = fopen("makanan_minuman.csv", "r");
    if(fp == NULL){
        printf("Gagal membuka file silahkan cek lokasi file\n");
    }

    int jumlah_baris_file = jumlah_baris(fp);
    struct Items items[jumlah_baris_file];

    isi_items(items, fp, jumlah_baris_file);

    cetak_items(items, jumlah_baris_file, "Makanan");
    printf("\n");
    cetak_items(items, jumlah_baris_file, "Minuman");

    // printf("(1) Pesan menu\n");
    // printf("(2) Cari di menu\n");

    // int pilihan_menu;
    // scanf("%d", &pilihan_menu);

    printf("Silahkan masukkan menu yang di pilih dengan mengetikan No / Nama / Kode:\n");
    
    char menu[9999];
    int porsi;

        while(1){
            printf("Menu(ketik Q untuk selesai):");
            scanf("%[^\n]", menu); getchar();
            if(cek_alfabet_dan_numeric(menu, strlen(menu)) == 1){
                printf("aman\nmenu:%s", menu);
                break;
            }else{
                printf("(Input hanya boleh berupa Alfabet/Numeric/Spasi)\n");
            }
        }

    
    

    getchar();

    return 0;
}