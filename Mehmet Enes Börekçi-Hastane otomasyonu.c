//Her bir seçenek, ilgili fonksiyonu çağırarak hastane otomasyonunda belirli bir işlemi gerçekleştirir. Bu işlevler hasta ekleme, hasta silme, hasta listeleme ve programdan çıkış gibi temel hastane yönetimi görevlerini içerir.

#include <stdio.h>  // stdio.h : Standart giriş/çıkış işlemleri için.
#include <stdlib.h>  //stdlib.h: Bellek yönetimi ve diğer genel işlemler için.
#include <string.h> //string.h: Dizgi işlemleri için.

// Kişi yapısı
typedef struct Kisi //Kisi: Bir kişinin temel bilgilerini içeren yapı.
{
    char ad[50];
    char soyad[50];
    int yas;
    int engelliMi;
    int hamileMi;
    int acilMi;
} Kisi;

// Hasta yapısı
typedef struct Hasta  //Hasta: Hasta bilgilerini ve öncelik puanını içeren yapı.
{
    int hastaNo;
    Kisi kisi;
    int oncelikPuani;
    struct Hasta* next;
} Hasta;

// Hasta Kuyruğu yapısı
typedef struct HastaKuyruk //    HastaKuyruk: Hasta düğümlerini içeren bir kuyruk yapısı.
{
    Hasta* bas;
    Hasta* son;
    int toplamHastaSayisi;
} HastaKuyruk;

// Öncelik puanı hesaplama fonksiyonu
void oncelikPuaniHesapla(Hasta* hasta) //    Bir hasta düğümünün öncelik puanını hesaplayan fonksiyon.
{
    hasta->oncelikPuani = 0;
    if (hasta->kisi.engelliMi) {
        hasta->oncelikPuani += 30;
    }
    if (hasta->kisi.yas >= 65) {
        hasta->oncelikPuani += (hasta->kisi.yas - 20);
    }
    if (hasta->kisi.acilMi) {
        hasta->oncelikPuani += 60;
    }
    if (hasta->kisi.hamileMi) {
        hasta->oncelikPuani += 40;
    }
}

// Yeni hasta ekleme fonksiyonu
Hasta* hastaEkle(HastaKuyruk* kuyruk, Kisi yeniKisi) //    Yeni bir hasta ekleyen fonksiyon.
{
    Hasta* yeniHasta = (Hasta*)malloc(sizeof(Hasta));
    yeniHasta->hastaNo = kuyruk->toplamHastaSayisi + 1;
    yeniHasta->kisi = yeniKisi;
    yeniHasta->next = NULL;
    oncelikPuaniHesapla(yeniHasta);

    if (kuyruk->son) {
        kuyruk->son->next = yeniHasta;
    } else {
        kuyruk->bas = yeniHasta;
    }

    kuyruk->son = yeniHasta;
    kuyruk->toplamHastaSayisi++;
    return yeniHasta;
}

// Hasta silme fonksiyonu
void hastaSil(HastaKuyruk* kuyruk, int hastaNo) //    Belirtilen hasta numarasına sahip hastayı silen fonksiyon.
{
    Hasta* current = kuyruk->bas;
    Hasta* prev = NULL;

    while (current) {
        if (current->hastaNo == hastaNo) {
            if (prev) {
                prev->next = current->next;
            } else {
                kuyruk->bas = current->next;
            }

            if (!current->next) {
                kuyruk->son = prev;
            }

            free(current);
            kuyruk->toplamHastaSayisi--;
            return;
        }

        prev = current;
        current = current->next;
    }
}

// Hasta listeleme fonksiyonu
void hastaListele(HastaKuyruk* kuyruk) //    Hasta kuyruğunu listelemek için kullanılan fonksiyon.
{
    Hasta* current = kuyruk->bas;

    if (!current) {
        printf("Kayitli hasta bulunmuyor.\n");
        return;
    }

    printf("######Hasta Listesi######\n");

    while (current) {
        printf("Hasta No: %d\n", current->hastaNo);
        printf("Ad: %s\n", current->kisi.ad);
        printf("Soyad: %s\n", current->kisi.soyad);
        printf("Yas: %d\n", current->kisi.yas);
        printf("Engelli Mi? %s\n", current->kisi.engelliMi ? "Evet" : "Hayir");
        printf("Hamile Mi? %s\n", current->kisi.hamileMi ? "Evet" : "Hayir");
        printf("Acilden Mi Geldi? %s\n", current->kisi.acilMi ? "Evet" : "Hayir");
        printf("Oncelik Puani: %d\n", current->oncelikPuani);

        current = current->next;

        if (current) {
            printf("\n");
        }
    }

    printf("######Hasta Listesi Sonu######\n");
}

int main() //    Programın ana kontrolü, kullanıcıdan seçim yapmasını bekleyen bir döngü içerir.
{
    HastaKuyruk hastaKuyrugu;
    hastaKuyrugu.bas = NULL;
    hastaKuyrugu.son = NULL;
    hastaKuyrugu.toplamHastaSayisi = 0;

    int secim_menu;

    printf("********HASTAHANE OTOMASYONUNA HOSGELDINIZ*********\n");

    while (1) {
        printf("1. Hasta Listesini Listele\n");
        printf("2. Yeni Hasta Ekle\n");
        printf("3. Hasta Sil\n");
        printf("4. Cikis\n");

        printf("Seciminiz : ");
        scanf("%d", &secim_menu);

        switch (secim_menu) {
            case 1:
                hastaListele(&hastaKuyrugu);
                break;

            case 2: {
                Kisi yeniKisi;
                printf("Yeni Hasta Bilgileri:\n");
                printf("Ad: ");
                scanf("%s", yeniKisi.ad);
                printf("Soyad: ");
                scanf("%s", yeniKisi.soyad);
                printf("Yas: ");
                scanf("%d", &yeniKisi.yas);
                printf("Engelli Mi? (1=Evet, 0=Hayir): ");
                scanf("%d", &yeniKisi.engelliMi);
                printf("Hamile Mi? (1=Evet, 0=Hayir): ");
                scanf("%d", &yeniKisi.hamileMi);
                printf("Acilden Mi Geldi? (1=Evet, 0=Hayir): ");
                scanf("%d", &yeniKisi.acilMi);

                Hasta* yeniHasta = hastaEkle(&hastaKuyrugu, yeniKisi);
                printf("Yeni hasta eklendi. Hasta No: %d\n", yeniHasta->hastaNo);
                break;
            }

            case 3: {
                int hastaNo;
                printf("Silinecek Hasta No'yu girin: ");
                scanf("%d", &hastaNo);
                hastaSil(&hastaKuyrugu, hastaNo);
                printf("Hasta silindi.\n");} }}
}