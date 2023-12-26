#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Kisi {
    char ad[50];
    char soyad[50];
    int yas;
    int engelliMi;
    int hamileMi;
    int acilMi;
} Kisi;

typedef struct Bolum {
    char ad[50];
    char doktor[50];
    struct Bolum* next;
} Bolum;

typedef struct Doktor {
    char ad[50];
    char bolum[50];
    struct Doktor* next;
} Doktor;

typedef struct Hasta {
    int hastaNo;
    Kisi kisi;
    int oncelikPuani;
    Bolum* bolum;
    Doktor* doktor;
    struct Hasta* next;
} Hasta;

typedef struct HastaListesi {
    Hasta* bas;
    Hasta* son;
    int toplamHastaSayisi;
} HastaListesi;

typedef struct BolumListesi {
    Bolum* bas;
} BolumListesi;

typedef struct DoktorListesi {
    Doktor* bas;
} DoktorListesi;

void oncelikPuaniHesapla(Hasta* hasta) {
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

Hasta* hastaOlustur(Kisi yeniKisi, Bolum* bolum, Doktor* doktor) {
    Hasta* yeniHasta = (Hasta*)malloc(sizeof(Hasta));
    yeniHasta->hastaNo = -1;
    yeniHasta->kisi = yeniKisi;
    yeniHasta->next = NULL;
    oncelikPuaniHesapla(yeniHasta);
    yeniHasta->bolum = bolum;
    yeniHasta->doktor = doktor;

    return yeniHasta;
}

void hastaEkle(HastaListesi* liste, Kisi yeniKisi, Bolum* bolum, Doktor* doktor) {
    Hasta* yeniHasta = hastaOlustur(yeniKisi, bolum, doktor);

    if (liste->bas == NULL) {
        liste->bas = yeniHasta;
        liste->son = yeniHasta;
    } else {
        liste->son->next = yeniHasta;
        liste->son = yeniHasta;
    }

    liste->toplamHastaSayisi++;
    yeniHasta->hastaNo = liste->toplamHastaSayisi;

    printf("Yeni hasta eklendi. Hasta No: %d\n", yeniHasta->hastaNo);
}

void hastaCikar(HastaListesi* liste) {
    if (liste->bas == NULL) {
        printf("Hasta bulunmuyor.\n");
        return;
    }

    Hasta* cikarilan = liste->bas;
    liste->bas = cikarilan->next;

    if (liste->bas == NULL) {
        liste->son = NULL;
    }

    printf("Hasta cikarildi. Hasta No: %d\n", cikarilan->hastaNo);
    free(cikarilan);
    liste->toplamHastaSayisi--;
}

void hastaListele(HastaListesi* liste) {
    Hasta* current = liste->bas;

    if (current == NULL) {
        printf("Kayitli hasta bulunmuyor.\n");
        return;
    }

    printf("######Hasta Listesi######\n");

    while (current != NULL) {
        printf("Hasta No: %d\n", current->hastaNo);
        printf("Ad: %s\n", current->kisi.ad);
        printf("Soyad: %s\n", current->kisi.soyad);
        printf("Yas: %d\n", current->kisi.yas);
        printf("Engelli Mi? %s\n", current->kisi.engelliMi ? "Evet" : "Hayir");
        printf("Hamile Mi? %s\n", current->kisi.hamileMi ? "Evet" : "Hayir");
        printf("Acilden Mi Geldi? %s\n", current->kisi.acilMi ? "Evet" : "Hayir");
        printf("Oncelik Puani: %d\n", current->oncelikPuani);
        printf("Bolum: %s\n", current->bolum->ad);
        printf("Doktor: %s\n", current->doktor->ad);

        current = current->next;

        if (current != NULL) {
            printf("\n");
        }
    }

    printf("######Hasta Listesi Sonu######\n");
}

void bolumEkle(BolumListesi* bolumListesi, const char* bolumAdi, const char* doktorAdi) {
    Bolum* yeniBolum = (Bolum*)malloc(sizeof(Bolum));
    snprintf(yeniBolum->ad, sizeof(yeniBolum->ad), "%s", bolumAdi);
    snprintf(yeniBolum->doktor, sizeof(yeniBolum->doktor), "%s", doktorAdi);
    yeniBolum->next = bolumListesi->bas;
    bolumListesi->bas = yeniBolum;
}

void doktorEkle(DoktorListesi* doktorListesi, const char* doktorAdi, const char* bolumAdi) {
    Doktor* yeniDoktor = (Doktor*)malloc(sizeof(Doktor));
    snprintf(yeniDoktor->ad, sizeof(yeniDoktor->ad), "%s", doktorAdi);
    snprintf(yeniDoktor->bolum, sizeof(yeniDoktor->bolum), "%s", bolumAdi);
    yeniDoktor->next = doktorListesi->bas;
    doktorListesi->bas = yeniDoktor;
}

void bolumListele(BolumListesi* bolumListesi) {
    Bolum* current = bolumListesi->bas;

    if (current == NULL) {
        printf("Bolum bulunmuyor.\n");
        return;
    }

    printf("######Bolum Listesi######\n");

    while (current != NULL) {
        printf("Bolum Adi: %s\n", current->ad);
        printf("Doktor Adi: %s\n", current->doktor);

        current = current->next;

        if (current != NULL) {
            printf("\n");
        }
    }

    printf("######Bolum Listesi Sonu######\n");
}

int main() {
    HastaListesi hastaListesi;
    hastaListesi.bas = NULL;
    hastaListesi.son = NULL;
    hastaListesi.toplamHastaSayisi = 0;

    BolumListesi bolumListesi;
    bolumListesi.bas = NULL;

    DoktorListesi doktorListesi;
    doktorListesi.bas = NULL;

    int secim_menu;

    printf("********HASTAHANE OTOMASYONUNA HOSGELDINIZ*********\n");

    while (1) {
        printf("1. Hasta Listesini Listele\n");
        printf("2. Yeni Hasta Ekle\n");
        printf("3. Hasta Cikar\n");
        printf("4. Bolum Ekle\n");
        printf("5. Bolum Listele\n");
        printf("6. Cikis\n");

        printf("Seciminiz : ");
        scanf("%d", &secim_menu);

        switch (secim_menu) {
            case 1:
                hastaListele(&hastaListesi);
                break;

            case 2: {
                Kisi yeniKisi;
                char bolumAdi[50], doktorAdi[50];
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
                printf("Hangi Bolumden Randevu Alacak: ");
                scanf("%s", bolumAdi);
                printf("Hangi Doktora Gidecek: ");
                scanf("%s", doktorAdi);

                Bolum* bolum = NULL;
                Bolum* currentBolum = bolumListesi.bas;
                while (currentBolum != NULL) {
                    if (strcmp(currentBolum->ad, bolumAdi) == 0) {
                        bolum = currentBolum;
                        break;
                    }
                    currentBolum = currentBolum->next;
                }

                if (bolum == NULL) {
                    printf("Boyle bir bolum bulunamadi. Once bolum eklemelisiniz.\n");
                } else {
                    Doktor* doktor = NULL;
                    Doktor* currentDoktor = doktorListesi.bas;
                    while (currentDoktor != NULL) {
                        if (strcmp(currentDoktor->ad, doktorAdi) == 0) {
                            doktor = currentDoktor;
                            break;
                        }
                        currentDoktor = currentDoktor->next;
                    }

                    if (doktor == NULL) {
                        printf("Boyle bir doktor bulunamadi. Once doktor eklemelisiniz.\n");
                    } else {
                        hastaEkle(&hastaListesi, yeniKisi, bolum, doktor);
                    }
                }
                break;
            }

            case 3:
                hastaCikar(&hastaListesi);
                break;

            case 4: {
                char bolumAdi[50], doktorAdi[50];
                printf("Yeni Bolum Bilgileri:\n");
                printf("Bolum Adi: ");
                scanf("%s", bolumAdi);
                printf("Doktor Adi: ");
                scanf("%s", doktorAdi);
                bolumEkle(&bolumListesi, bolumAdi, doktorAdi);
                printf("Yeni bolum eklendi.\n");
                break;
            }

            case 5:
                bolumListele(&bolumListesi);
                break;

            case 6:
                printf("Cikis Yaptiniz\n");
                Hasta* currentHasta = hastaListesi.bas;
                while (currentHasta != NULL) {
                    Hasta* tempHasta = currentHasta;
                    currentHasta = currentHasta->next;
                    free(tempHasta);
                }

                Bolum* currentBolum = bolumListesi.bas;
                while (currentBolum != NULL) {
                    Bolum* tempBolum = currentBolum;
                    currentBolum = currentBolum->next;
                    free(tempBolum);
                }

                Doktor* currentDoktor = doktorListesi.bas;
                while (currentDoktor != NULL) {
                    Doktor* tempDoktor = currentDoktor;
                    currentDoktor = currentDoktor->next;
                    free(tempDoktor);
                }
                return 0;

            default:
                printf("Yanlis Secim Yaptiniz!!! Lutfen Tekrar Deneyiniz\n\n");
        }
    }
}
