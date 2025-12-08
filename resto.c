#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// login
int login() {
    char username[20], password[20];

    printf("=== LOGIN STAFF ===\n");
    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < jumlahUser; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            printf("\nLogin berhasil!\n\n");
            return 1;
        }
    }

    printf("\nLogin gagal, coba lagi!\n\n");
    return 0;
}

// tampilkan menu
void tampilkanMenuMakanan() {
    FILE *file = fopen("makanan_minuman.csv", "r");
    if (!file) {
        printf("Menu tidak ada\n");
        return;
    }

    char buffer[100];
    int baris = 0;

    printf("\n=== DAFTAR MAKANAN & MINUMAN ===\n");

    while (fgets(buffer, sizeof(buffer), file)) {
        if (baris == 0) { 
            baris++; 
            continue;  
        }

        int id, harga;
        char nama[50];

        sscanf(buffer, "%d,%49[^,],%d", &id, nama, &harga);

        printf("%d. %s - Rp %d\n", id, nama, harga);
    }

    fclose(file);
}

// menubeli
void menuBeli() {
    printf("\n=== MENU BELI ===\n");
   
}

// menu reserv
void reservasi() {
    printf("\n=== MENU RESERVASI ===\n");
   
}

// menu edit
void pengeditan() {
    printf("\n=== MENU PENGEDITAN ===\n");
   
}

// menu utama
void menuUtama() {
    int pilihan;

    do {
        printf("=== MENU UTAMA ===\n");
        printf("1. Daftar Makanan dan Minuman\n");
        printf("2. Beli\n");
        printf("3. Reservasi\n");
        printf("4. Pengeditan\n");
        printf("5. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tampilkanMenuMakanan();
                break;

            case 2:
                menuBeli();
                break;

            case 3:
                reservasi();
                break;

            case 4:
                pengeditan();
                break;

            case 5:
                printf("\nTerima kasih!\n");
                break;

            default:
                printf("\nPilihan tidak valid!\n\n");
        }

    } while (pilihan != 5);
}

// ==== PROGRAM UTAMA ====
int main() {
   
    while (!login()) {}    
    menuUtama();

    return 0;
}

