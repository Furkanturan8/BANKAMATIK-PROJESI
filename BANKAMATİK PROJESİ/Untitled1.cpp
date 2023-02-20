#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

typedef struct Musteri{
	int musteriNo;
	char tc[10];
	char adSoyad[20];
	char dogumGunu[11];
	char cinsiyet;
	char adres[10];
	char tel[10];
	char eposta[22];
	int  sifre;
	float bakiye;
	int sifreDurum; //tek kullanýmlýk sifre=0 , yenilerse 1
	int hesapDurum; //hesap aktifse 1, degilse 0;
}musteri;

typedef struct Hareket{
	int musteriNo;
	char adSoyad[20];
	char sube[20];
	char islem[20];
	time_t tarih;	
}hareket;

void hareketKaydet(musteri m1, const char * islemPtr){
	hareket h1;
	h1.musteriNo=m1.musteriNo;
	strcpy(h1.adSoyad,m1.adSoyad);
	strcpy(h1.islem,islemPtr);
	strcpy(h1.sube,"FSM Þubesi");
	h1.tarih=time(0); // þuanki tarihi zaman için time(0) gerekir
	
	char dosyaAdi[20];
	
	strcpy(dosyaAdi,m1.tc);
	strcat(dosyaAdi,".dat");
	FILE *ptr=fopen(dosyaAdi,"a+b");
	fwrite(&h1,sizeof(hareket),1,ptr);
	fclose(ptr);
//	printf("Hareket Kaydý Tamamlandý!\n");
}

void bakiyeGuncelle(musteri m1, float para){
	
	musteri m2;
	FILE *ptr=fopen("musteriler.dat","r+b");
	int sayac=0;

	while(fread(&m2,sizeof(musteri),1,ptr)!=NULL){
		if( strcmp(m1.tc,m2.tc)==0 )
			break;
	
	sayac++;
	}
	rewind(ptr); //ptr yi baþa alalým
	m2.bakiye+=para; //+ ise toplýcak - ise çýkaracak
	fseek(ptr, (sayac)*sizeof(musteri) ,0);
	fwrite(&m2,sizeof(musteri),1,ptr);
	//printf("Bakiye Güncellendi.");
	fclose(ptr);
	
}
void paraCek(musteri m1){
	
	system("cls");
	printf("Para Çekme Ekraný\n\n");
	float ucret;
	printf("Çekmek istediðiniz ücret:"); scanf("%f",&ucret);
	
	musteri m2;
	FILE *ptr=fopen("musteriler.dat","r+b");
	while(fread(&m2,sizeof(musteri),1,ptr)!=NULL){
		if( strcmp(m1.tc,m2.tc)==0 )
		break;
	}
	fclose(ptr);
	if(ucret>m2.bakiye)
		printf("Yetersiz Bakiye!!\n\n");
	
	else{
		bakiyeGuncelle(m2,-ucret); //paraçekme olursa - ucreti gonderelim	
		hareketKaydet(m2,"Para Çekme");
		printf("Para Çekme Ýþlemi Baþarýlý!\n\n");
	}
	
}
void paraYatir(musteri m1){
	
	system("cls");
	printf("Para Yatýma Ekraný\n\n");
	float ucret;
	printf("Yatýrmak istediðiniz ücret:"); scanf("%f",&ucret);
	
	musteri m2;
	FILE *ptr=fopen("musteriler.dat","r+b");
	while(fread(&m2,sizeof(musteri),1,ptr)!=NULL){
		if( strcmp(m1.tc,m2.tc)==0 )
		break;
	}
	fclose(ptr);
		
	bakiyeGuncelle(m2,ucret); //paraçekme olursa - ucreti gonderelim	
	hareketKaydet(m2,"Para Yatýrma");
	printf("Para Yatýrma Ýþlemi Baþarýlý!\n\n");
}

void havaleEft(musteri m1){
	
	system("cls");
	printf("Havale / EFT Ekraný \n\n");
	
	float ucret;
	int musteriNo;
	char adSoyad[20];
	printf("Hesap No   : "); scanf("%d",&musteriNo);
	printf("Ad Soyad   : "); scanf(" %[^\n]s",adSoyad);
	printf("Ücret (TL) : "); scanf("%f",&ucret);
	
	musteri m2;
	int sonuc=0;
	FILE * ptr=fopen("musteriler.dat","r+b");
	while( fread(&m2,sizeof(musteri),1,ptr)!=NULL){
		if(musteriNo==m2.musteriNo && strcmp(adSoyad,m2.adSoyad)==0){
			sonuc=1;
			break;
		}
	}
	fclose(ptr);
	ptr=fopen("musteriler.dat","r+b");
	
	if(sonuc==0){
		printf("\nAlýcý hesap numarasý veya ad/soyad hatalý!\n\n");
		return;
	}
	
	musteri m3;
	int sonuc2=0;
	while( fread(&m3,sizeof(musteri),1,ptr)!=NULL){
		if(strcmp(m1.tc,m3.tc)==0){
			sonuc2=1;
			break;
		}
	}
	fclose(ptr);
	if(ucret>m3.bakiye)
		printf("Yetersiz Bakiye !\n\n");
	
	else {
		bakiyeGuncelle(m3,-ucret);
		bakiyeGuncelle(m2,+ucret);
		
		hareketKaydet(m3,"Havale Gönderildi");
		hareketKaydet(m2,"Havale Aldý");
		
		printf("Havale Ýþlemi Tamamlandý! \n\n");
	}
}

void bakiyeSorgula(musteri m1){
	system("cls");
	printf("Bakiye Sorgulama Ekraný\n");

	musteri m2;
	FILE *ptr=fopen("musteriler.dat","r+b");
	while(fread(&m2,sizeof(musteri),1,ptr)!=NULL){
		if( strcmp(m1.tc,m2.tc)==0 )
		break;
	}
	fclose(ptr);
		
	printf("\nSayýn %s Hesabýnýzdaki Güncel Bakiyeniz: %.2f TL\n\n",m1.adSoyad,m2.bakiye);
	hareketKaydet(m2,"Bakiye Sorgulama");
	printf("Bakiye Sorgulama Ýþlemi Baþarýlý!\n\n");
	
}
void sifreDegistir(musteri m1){
		
	system("cls");
	int sonuc=0, sayac=0,denemesayisi=0,main();
	int eskiSifre,yeniSifre,sifreTekrari;
	printf("Þifre Deðiþtirme Ekraný\n\nÞifreniz Yalnýzca Numaralardan Olmalýdýr!\n\n");
	tekrardene:
	printf("Eski Þifreyi Giriniz  :"); scanf("%d",&eskiSifre);
	printf("Yeni Sifreyi Giriniz  :"); scanf("%d",&yeniSifre);
	printf("Sifreyi Tekrar Giriniz:"); scanf("%d",&sifreTekrari);
	
	if(eskiSifre==m1.sifre) {
		if(yeniSifre==sifreTekrari){
			
			musteri m2; 
			FILE *dosyaPtr=fopen("musteriler.dat","r+b");
			while(fread(&m2,sizeof(musteri),1,dosyaPtr)!=NULL){
				
				if( strcmp(m1.tc,m2.tc)==0 ){
					sonuc=1;
					break;
				}
				sayac++;
			}
			if(sonuc==0){
				printf("\nÞifre Deðiþtirilemedi ! \n\n");
				fclose(dosyaPtr);
			}
			else{
				rewind(dosyaPtr); //ilgili yerin konumunu baþa aldý (imlecini)
				fseek(dosyaPtr , (sayac)*sizeof(musteri) , 0 ); //arama yaptý 	
				//m2.sifreDurum=1;
				m2.sifre=yeniSifre;
				fwrite(&m2, sizeof(musteri) ,1 , dosyaPtr);
				fclose(dosyaPtr);
				printf("\nÞifre Deðiþtirme Ýþlemi Baþarýlý, Yeni Þifrenizle Giriþ Yapabilirsiniz. \n\n");
				hareketKaydet(m2,"Þifre Deðiþimi");
			}		
		}
		else{
			if(denemesayisi==2){
				printf("\n3 kez hatalý deneme yaptýnýz! Oturum Kapanýyor...\n\n");
				main();
			}
			printf("\nÞifre Tekrarý Hatalý Tekrar Deneyin!\n\n");
			denemesayisi++;
			goto tekrardene;
		}
		
	}
	else {
		if(denemesayisi==2){
			printf("\n3 kez hatalý deneme yaptýnýz! Oturum Kapanýyor...\n\n");
			main();
		}
		printf("\nHatalý Eski Þifre, Tekrar Deneyin!\n\n");
		denemesayisi++;
		goto tekrardene;
	}
}

void hareketRaporu(musteri m1){
	system("cls");
		printf("Hareket Raporu Ekraný \n\n");
	hareket h1;
	int sayac=0;
	char dosyaAdi[20];
		
	strcpy(dosyaAdi,m1.tc);
	strcat(dosyaAdi,".dat");
	FILE *ptr=fopen(dosyaAdi,"r+b");
	printf("%-12s%-30s%-20s%-20s%-30s \n ","Müþteri No  "," Ad-Soyad"," Þube"," Ýþlem"," Tarih");

	while(fread(&h1,sizeof(hareket),1,ptr)!=NULL){
		printf("%-12d%-30s%-20s%-20s%-30s \n ",h1.musteriNo,h1.adSoyad,h1.sube,h1.islem,ctime(&h1.tarih));
		sayac++;
	}
	fclose(ptr);
	printf("Toplam Ýþlem Sayýsý: %d \n\n",sayac);
	
}

int musteriMenu(){
	int secim;
	printf("   1-Para Çek\n");
	printf("   2-Para Yatýr\n");
	printf("   3-Havale / EFT / FAST \n");
	printf("   4-Bakiye Sorgula\n");
	printf("   5-Þifre Deðiþtir \n");
	printf("   6-Hareket Raporu\n");
	printf("   0-Çýkýþ Yap\n");
	printf("   Seçiminiz: "); scanf("%d",&secim);
	return secim;
}

void musterimiz(musteri m1){
	system("cls");
	printf("Sayýn %s Hoþgeldiniz!\n\n",m1.adSoyad );
	printf("Müþteri No: %d \n\n",m1.musteriNo);
	int secim=musteriMenu();
	
	while(secim!=0){
		
		switch (secim){
			
			case 1: paraCek(m1); break;
			case 2: paraYatir(m1); break;
			case 3: havaleEft(m1); break;
			case 4: bakiyeSorgula(m1); break;
			case 5: sifreDegistir(m1); break; 
			case 6: hareketRaporu(m1); break; 
			case 0: break;
			default: printf("Hatalý Seçim Yaptýnýz!\n"); break;
		}
		secim=musteriMenu();
	}
	system("cls");
	printf("\nSayýn %s Çýkýþ Yaptýnýz...\n\n",m1.adSoyad);	
}

void sifreDegistirTek( musteri m1 )
{
	system("cls");
	int sonuc=0, sayac=0;
	int tekKullanimlikParola,yeniSifre,sifreTekrari;
	printf("Þifre Deðiþtirme Ekraný\n\n");
	printf("Eski Þifreyi Giriniz  :"); scanf("%d",&tekKullanimlikParola);
	printf("Yeni Sifreyi Giriniz  :"); scanf("%d",&yeniSifre);
	printf("Sifreyi Tekrar Giriniz:"); scanf("%d",&sifreTekrari);

	if(tekKullanimlikParola==m1.sifre && yeniSifre==sifreTekrari) {
		musteri m2; 
		FILE *dosyaPtr=fopen("musteriler.dat","r+b");
		while(fread(&m2,sizeof(musteri),1,dosyaPtr)!=NULL){
			
			if( strcmp(m1.tc,m2.tc)==0 ){
				sonuc=1;
				break;
			}
			sayac++;
		}
		if(sonuc==0){
			printf("\nÞifre Deðiþtirilemedi ! \n\n");
			fclose(dosyaPtr);
		}
		else{
			rewind(dosyaPtr); //ilgili yerin konumunu baþa aldý (imlecini)
			fseek(dosyaPtr , (sayac)*sizeof(musteri) , 0 ); //arama yaptý 	
			m2.sifreDurum=1;
			m2.sifre=yeniSifre;
			fwrite(&m2, sizeof(musteri) ,1 , dosyaPtr);
			fclose(dosyaPtr);
			printf("\n\nÞifre Deðiþtirme Ýþlemi Baþarýlý, Yeni Þifrenizle Giriþ Yapabilirsiniz. \n\n");
		}
	}
	else {
		printf("\n\nHatalý Þifre Deðiþtirme Ýþlemi !\n\n");
	}	
}

void musteriGiris(){
	
	musteri m1;
	system("cls");
	printf("Müþteri Giriþ Ekraný \n\n");
	char tc[10];
	int parola,sonuc=0;	
	printf("Kullanýcý id :"); scanf("%s",tc);
	printf("Sifre        :"); scanf("%d",&parola);
	
	FILE *dosyaPtr=fopen("musteriler.dat","r+b");
	
	while (fread(&m1,sizeof(musteri),1,dosyaPtr)!=NULL){
		
		if( strcmp(tc,m1.tc)==0 && parola==m1.sifre ){
			
			sonuc=1;
			break;
		}
		
	}
	fclose(dosyaPtr);
	if(sonuc==0) 
		printf("Hatalý Giriþ Yaptýnýz!\n\n");
	else{
		
		if(m1.sifreDurum==0){
			sifreDegistirTek( m1 ); // giriste 1 defa degistirilen sifre
		}
		else if(m1.hesapDurum==0){
			printf("\nHesabýnýz Dondurulmuþtur ! Lütfen Yetkili Bankamatik Personeli Ýle Görüþünüz ! \n\nÝletiþim : 0224 542 783 / turanfurkan123@gmail.com\n\n");
		} 
		else 
		musterimiz(m1); //terkar tekrar adamým aramayalým parametre gonderelim
	}
	
}

void musteriEkle(){
	
	srand(time(0));
	system("cls");
	printf("Müþteri Kayýt Ekraný\n\n");
	musteri m1;
	
	printf("TC           :"); scanf(" %[^\n]s",m1.tc);
	printf("Ad-Soyad     :"); scanf(" %[^\n]s",m1.adSoyad);
	printf("Doðum Günü   :"); scanf(" %[^\n]s",m1.dogumGunu);
	printf("Cinsiyet(E/K):"); scanf("%s",&m1.cinsiyet);
	printf("Adres        :"); scanf(" %[^\n]s",m1.adres);
	printf("Telefon      :"); scanf(" %[^\n]s",m1.tel);
	printf("Eposta       :"); scanf(" %[^\n]s",m1.eposta);
	
	m1.musteriNo=rand()%1000+1000;
	m1.sifre=rand()%10000+10000;
	m1.bakiye=0.00;
	m1.sifreDurum=0;
	m1.hesapDurum=1;
	
	FILE *dosyaPtr=fopen("musteriler.dat","a+b");
	fwrite(&m1,sizeof(musteri),1,dosyaPtr);
	fclose(dosyaPtr);
	printf("\nMüþteri Kaydý Tamamlandý ! \n");
	printf("Tek Kullanýmlýk Þifre SMS Olarak Gönderildi !\n");
	printf("Gizli tek kullanýmlýk þifre: %d \n\n",m1.sifre);

}

void musteriSil(){
	
	system("cls");
	printf("Müþteri Silme Ekraný\n\n");
	
	musteri m1;
	char tc[10];
	
	printf("TC : "); scanf(" %[^\n]s",tc);
	FILE *dosyaPtr=fopen("musteriler.dat","r+b");
	int sonuc=0;
	
	while(fread(&m1 , sizeof(musteri) , 1 , dosyaPtr)!=NULL){
		
		if(strcmp(tc,m1.tc)==0){
			sonuc=1;
			break;
		}
	}
	fclose(dosyaPtr);
	if(sonuc=0)
		printf("%s numaralý müþteri bulunamadi !",tc);
	else {
		
		char tercih;
		printf("%s numaralý müþteriyi silmek istediðinize emin misiniz? (Evetse:E/e Deðilse:H/h)",m1.tc); scanf(" %c",&tercih);
		if(tercih=='e'|| tercih=='E'){
			
			dosyaPtr=fopen("musteriler.dat","r+b");
			FILE *yedekPtr=fopen("yedekle.dat","w+b");
			int silSonuc=0;
			
			while(fread(&m1, sizeof(musteri), 1, dosyaPtr)!=NULL){
				
				if(strcmp(tc,m1.tc)==0)
					silSonuc=1;
				else
					fwrite(&m1, sizeof(musteri), 1, yedekPtr);
			}
			fclose(dosyaPtr);
			fclose(yedekPtr);
			
			if(silSonuc==0)
				printf("\nSilme Ýþlemi Baþarýsýz!!\n");
		
			else{
				remove("musteriler.dat");
				rename("yedekle.dat","musteriler.dat");
				printf("\n%s numaralý müþteri silinmiþtir !\n\n",tc);
			}
		}
		else{
			printf("\nSilme Ýþlemi Ýptal Edilmiþtir !\n\n");
		}
	}
}

void hesapDondur(){
	
	system("cls");
	printf("Müþteri Hesap Dondurma Ekraný\n\n");
	
	musteri m1;
	char tc[10];
	
	printf("TC : "); scanf(" %[^\n]s",tc);
	FILE *dosyaPtr=fopen("musteriler.dat","r+b");
	int sonuc=0,sayac=0;
	
	while(fread(&m1 , sizeof(musteri) , 1 , dosyaPtr)!=NULL){
		
		if(strcmp(tc,m1.tc)==0){
			sonuc=1;
			break;
		}
		sayac++;
	}
	
	if(sonuc=0)
		printf("%s numaralý müþteri bulunamadi !",tc);
	else {
		rewind(dosyaPtr); // imleci baþa aldýk // rewind fonx : dosya konumunu en baþa alýr.	
		fseek(dosyaPtr, (sayac) * sizeof(musteri) , 0); //sayac sayesinde(ilgili kordinata gittik) ilgili müþteriyi bulduk VE 0 yaptýk 
		m1.hesapDurum=0; //m1 hesap durumunu 0 yaptýk
		fwrite(&m1, sizeof(musteri), 1, dosyaPtr); //ve yeni halini yeniden yazdýrdýk
		printf("\nHesap Donduruldu !\n\n");
	}
	fclose(dosyaPtr);
}

void hesapAktiflestir(){
	
	system("cls");
	printf("Müþteri Hesap Aktifleþtirme Ekraný\n\n");
	
	musteri m1;
	char tc[10];
	
	printf("TC : "); scanf(" %[^\n]s",tc);
	FILE *dosyaPtr=fopen("musteriler.dat","r+b");
	int sonuc=0,sayac=0;
	
	while(fread(&m1 , sizeof(musteri) , 1 , dosyaPtr)!=NULL){
		
		if(strcmp(tc,m1.tc)==0){
			sonuc=1;
			break;
		}
		sayac++;
	}
	
	if(sonuc=0)
		printf("%s numaralý müþteri bulunamadi !",tc);
	else {
		rewind(dosyaPtr); // imleci baþa aldýk // rewind fonx : dosya konumunu en baþa alýr.	
		fseek(dosyaPtr, (sayac) * sizeof(musteri) , 0); //sayac sayesinde(ilgili kordinata gittik) ilgili müþteriyi bulduk VE 0 yaptýk 
		m1.hesapDurum=1; //m1 hesap durumunu 0 yaptýk
		fwrite(&m1, sizeof(musteri), 1, dosyaPtr); //ve yeni halini yeniden yazdýrdýk
		printf("\nHesap Aktifleþtirildi !\n\n");
	}
	fclose(dosyaPtr);
}

void musteriListele(){
	system("cls");
	musteri m1;
	printf("Müþteri Listeleme Ekraný \n\n");
	
	int sayac=0;
	FILE *dosyaPtr=fopen("musteriler.dat","r+b");
	
	printf("%s\t %s\t %s\t %s\t %s\t %s\t %s\t       %s\t   %s    %s      %s\n\n","Müþteri-No","E-Posta","TC","AD-SOYAD","Doðum Günü","Cinsiyet","Adres","Telefon","Bakiye","SfrDur","HspDur");  
	while(fread(&m1 , sizeof(musteri) , 1 , dosyaPtr)!=NULL){
		printf("%-10d%-10s\t%-10s%-10s\t%-10s\t   %-10c    %-10s   %-10s    %-10.2f  %-10d  %-10d\n",m1.musteriNo,m1.eposta,m1.tc,m1.adSoyad,m1.dogumGunu,m1.cinsiyet,m1.adres,m1.tel,m1.bakiye,m1.sifreDurum,m1.hesapDurum );
		sayac++;
	}	
	printf("\nListedeki müþteri sayýsý: %d\n\n", sayac);
	fclose(dosyaPtr);
}

int  personelMenu(){
	int secim;
	printf("   1-Müþteri Ekle\n");
	printf("   2-Müþteri Sil\n");
	printf("   3-Hesap Dondur\n");
	printf("   4-Hesap Aktifleþtir\n");
	printf("   5-Müþterileri Listele\n");
	printf("   0-Çýkýþ Yap\n");
	printf("   Seçiminiz: "); scanf("%d",&secim);
	return secim;
}

void personel(){
	
	printf("\nSayýn Furkan Hoþgeldiniz...\n\n");
	int secim=personelMenu();
	
	while(secim!=0){
		
		switch (secim){
			
			case 1: musteriEkle(); break;
			case 2: musteriSil(); break;
			case 3: hesapDondur(); break;
			case 4: hesapAktiflestir(); break;
			case 5: musteriListele(); break; 
			case 0: break;
			default: printf("Hatalý Seçim Yaptýnýz!\n"); break;
		}
		secim=personelMenu();
	}
	system("cls");
	printf("\nSayýn Furkan Çýkýþ Yaptýnýz...\n\n");
}


void personelGiris(){
	system("cls");
	printf("Personel Giriþ Ekraný \n\n");
	char kullaniciad[10];
	char sifre[10];
	printf("Kullanýcý id :"); scanf("%s",kullaniciad);
	printf("Sifre        :"); scanf("%s",sifre);
	
	if( strcmp(kullaniciad,"furkan")==0 && strcmp(sifre,"123")==0 )
		personel();
	else 
		printf("Hatalý Giriþ Yaptýnýz!\n\n");
}



int menu(){
	
	int secim;
	
	printf("\n   BANKAMATIK OTOMASYONU\n");
	printf("   -----------------------\n");
	printf("   1-Müþteri Giriþ\n");
	printf("   2-Personel Giriþ\n");
	printf("   0-Çýkýþ\n");
	printf("   -----------------------\n");
	printf("   Seçiminiz: "); scanf("%d",&secim);
	
	return secim;
}

int main(){
	setlocale(LC_ALL,"Turkish");
	int secim=menu();
	
	while(secim!=0){
		
		switch (secim){
			case 1: musteriGiris(); break;
			case 2: personelGiris(); break; 
			case 0: break;
			default: printf("Hatalý Seçim Yaptýnýz!\n"); break;
		}
		secim=menu();
	}
	
	printf("\n\nÇýkýþ Yapýlýyor...\n");
	return 0;
}
