#include "dialog.h"
#include "ui_dialog.h"

/*
# ---------------------------
# - Ad-Soyad: Mohamed Hamdo
# - Öğrenci num: 20010011524
# - 1.Ödev
# ---------------------------
*/

/*
# ---------- NOTLAR ----------
# 1) Kullanılan ekranın boyutları: 1350 x 750
# 2) Sonuçlarını dosya (skorlar.txt)'ye kaydetmek için oyuncu adı özelliği eklendi
# 3) Ekranda her saniye bir karpuz gösteriliyor
# 4) Oyun seviyesi özelliği eklendi (kolay-orta-zor)
    - Kolay: Her saniyede bir karpuz görüntülenir (1000 ms)
    - Orta: Her 800 milisaniyede bir karpuz görüntüleniyor
    - Zor: Her yarım saniyede bir (500 milisaniye) bir karpuz görüntülenir
# 5) Dosya (konumlar.txt)'deki tüm konum verileri kullanılmışsa ve oyunda hala
     zaman varsa, (index) değişkeni verileri ilk konumdan tekrar kullanmaya döner.
# 6) Kullanılan dosyaların yolunu değiştirmek için Lütfen 26. ve 27. satırlardaki
     değişken adlarını değiştirin.
# 7) Global değişkenler/fonksiyonlar ve bunların kullanımı hakkında bilgi edinmek için
     (dialog.h) dosyasını görebilirsiniz.
*/

QString Dosya1 = "C:\\Users\\mhmdm\\Desktop\\Programming\\Qt\\Odevler\\Odev1\\konumlar.txt";
QString Dosya2 = "C:\\Users\\mhmdm\\Desktop\\Programming\\Qt\\Odevler\\Odev1\\skorlar.txt";

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_start_clicked()
{

    if (ui->name->text().isEmpty())
    {
        QMessageBox::warning(this, "Uyari", "Lütfen adınız giriniz");
        return;
    }

    /* Dosya (konumlar.txt) ve (skorlar.txt)'deki verileri okuyun
        ve karpuz koordinatlarını ve maksimum sonucu çıkarın */
    dosya();

    // Ekrandaki saati her saniye güncelleyen zamanlayıcı
    QTimer *Timer_Sure = new QTimer(this);
    Timer_Sure->start(1000);
    connect(Timer_Sure, SIGNAL(timeout()), this, SLOT(sure_degistir()));

    // Zamanlayıcıyı oyunun bitip bitmediğini kontrol eden bir fonksiyona bağlamak
    connect(Timer_Sure, SIGNAL(timeout()), this, SLOT(oyun_bitti()));

    // Oyunun zorluk seviyesini belirleyin ve buna göre bir zamanlayıcı oluşturun
    // Zamanlayıcı karpuzun ekranda gösterilmesi için kullanılır
    int time = 1000;
    if (ui->kolay->isChecked())
    {
        time = 1000;
    }
    if (ui->orta->isChecked())
    {
        time -= 200;
    }
    if (ui->zor->isChecked())
    {
        time -= 500;
    }
    QTimer *Timer_kar = new QTimer(this);
    Timer_kar->start(time);
    connect(Timer_kar, SIGNAL(timeout()), this, SLOT(KarpuzYarat()));
}

void Dialog::dosya()
{
    // Dosya (konumlar.txt) için
    QFile file(Dosya1);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString Message = "Dosya (konumlar.txt) okumak için açılamıyor.\nLütfen dosyayı kontrol edip tekrar deneyin.";
        QMessageBox::warning(this, "Bilgi", Message);
        close();
    }

    // Koordinatları okuyun ve bunları matris konumlar_x ve konumlar_y'de saklayın
    // Dosya Formatı: x y
    QTextStream veriler(&file);
    while (!veriler.atEnd())
    {
        QString line = veriler.readLine();
        QStringList values = line.split(" ");

        if (values.size() == 2)
        {
            int x = values[0].toInt();
            int y = values[1].toInt();

            konumlar_x.append(x);
            konumlar_y.append(y);
        }
    }
    file.close();

    /////////////////////////////

    // Dosya (skorlar.txt) için
    int Max = 0;
    QFile file2(Dosya2);
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString Message = "Dosya (skorlar.txt) okumak için açılamıyor.\nLütfen dosyayı kontrol edip tekrar deneyin.";
        QMessageBox::warning(this, "Bilgi", Message);
        close();
    }

    // Tüm sonuçları okuyun ve maksimum değeri saklayın
    QTextStream veriler2(&file2);
    while (!veriler2.atEnd())
    {
        QString line = veriler2.readLine();
        QStringList values = line.split(" ");

        if (values[0].toInt())
        {
            if (Max <= values[0].toInt())
            {
                Max = values[0].toInt();
            }
        }
    }
    file2.close();

    // Global değişken Max_Skor'un maksimum değerini saklayın
    if (Max > Max_Skor)
    {
        Max_Skor = Max;
    }
}

void Dialog::sure_degistir()
{
    if (sure >= 30)
    {
        return;
    }
    sure++;
    QString txt = "(" + QString::number(sure) + "/30)";
    ui->sureci->setText(txt);
}

void Dialog::KarpuzYarat()
{
    if (sure >= 30)
    {
        return;
    }

    /*Tüm koordinatların kullanılıp kullanılmadığını kontrol edin,
      öyleyse yeniden başlayın*/
    if (index >= konumlar_y.size())
    {
        index = 0;
    }

    // Bir karpuz nesnesi oluşturun, görüntüyü yerleştirin ve konumunu belirleyin
    QPushButton *Krpuz = new QPushButton(this);

    int x = konumlar_x[index];
    int y = konumlar_y[index];
    index++;
    Krpuz->setGeometry(x, y, 81, 81);

    QPixmap Karpuz_Resmi(":/images/images/1.png");
    QPixmap scaledPixmap = Karpuz_Resmi.scaled(81, 81, Qt::KeepAspectRatio);
    QIcon icon(scaledPixmap);

    Krpuz->setIcon(icon);
    Krpuz->setIconSize(scaledPixmap.size());
    Krpuz->setFlat(true);
    Krpuz->show();

    Karpuz_Liste.append(Krpuz); // Nesneyi nesneler listesine ekleyin

    /*Nesnenin tıklanıp tıklanmadığını kontrol edin ve tıklanıyorsa onu
      başka bir nesneye dönüştürün (karpuz kesme)*/
    Krpuz->setCheckable(true); // Yalnızca tek tıklamayla etkileşim kurmak için
    connect(Krpuz, &QPushButton::clicked, [this, Krpuz]()
            {
        if(Krpuz->isChecked()){
            //Kesilen++;
            ui->kesilen->setText(QString::number(++Kesilen));

            //Bu nesneye yapılan diğer tıklamalarla etkileşime girmeyin
            Krpuz->setCheckable(false);

            //Üzerine tıkladığınızda nesnenin konumunu kaydedin ve gizleyin
            int x = Krpuz->x();
            int y = Krpuz->y();
            Krpuz->hide();

            /*Yeni bir nesne oluşturun (kesilmiş bir karpuz) ve
            onu gizli nesnenin yerine yerleştirin*/
            QLabel *Karpuz_kes = new QLabel(this);
            QPixmap KarpuzK_Resmi(":/images/images/2.png");
            QPixmap scaledPixmap = KarpuzK_Resmi.scaled(81, 81, Qt::KeepAspectRatio);
            Karpuz_kes->setGeometry(x, y, 81, 81);
            Karpuz_kes->setPixmap(scaledPixmap);
            Karpuz_kes->setFixedSize(scaledPixmap.size());
            Karpuz_kes->show();

            //Yeni nesneyi 700 milisaniye sonra gizleyen zamanlayıcı
            QTimer *timerHidden = new QTimer(this);
            connect(timerHidden, &QTimer::timeout, [timerHidden,Karpuz_kes]() {
                if(!(Karpuz_kes->isHidden())){
                    Karpuz_kes->hide();
                    timerHidden->stop();
                }
            });
            timerHidden->start(700);
        } });

    ////////////////////////

    /*
     - Nesnenin konumunu y koordinatlarında 5'lik artışlarla değiştiren
       çok hızlı bir zamanlayıcı (15 ms)
     - Düşen karpuz efektini uygulayın
    */
    QTimer *timer3 = new QTimer(this);
    timer3->start(10);
    connect(timer3, &QTimer::timeout, [Krpuz, this, timer3]()
            {
        Krpuz->move(Krpuz->x(), Krpuz->y()+4);;
        if (Krpuz->y() > 750 && !(Krpuz->isHidden())) {
            Krpuz->hide();
            //Kacirilan++;
            ui->kacirilan->setText(QString::number(++Kacirilan));

            timer3->stop();
        } });
}

void Dialog::oyun_bitti()
{
    if (sure < 30)
    {
        return;
    }

    /*Oyun süresi bittiğinde ekranda hala nesnelerin (karpuz) bulunup
      bulunmadığını kontrol edin, varsa oyun bitmesin.*/
    int bol = 0;
    for (int i = 0; i < Karpuz_Liste.size(); ++i)
    {
        if (!(Karpuz_Liste[i]->isHidden()))
        {
            bol = 1;
        }
    }

    if (bol == 1)
    {
        return;
    }

    // Oyun sonucunu ekranda göster
    if (Kesilen >= Max_Skor)
    {
        QString text = "Oyun Bitti! Tebrikler Maksimum Skor Sizde! \nKesilen Karpuz Sayısı: " + QString::number(Kesilen) + "\nKaçırılan Karpuz Sayısı: " + QString::number(Kacirilan) + "\nÖnceki Maximum Skor: " + QString::number(Max_Skor);

        QMessageBox::information(this, "Bilgi", text);
        close();
    }
    else
    {
        QString text = "Oyun Bitti! Maksimum Skoru Geçemediniz! \nKesilen Karpuz Sayısı: " + QString::number(Kesilen) + "\nKaçırılan Karpuz Sayısı: " + QString::number(Kacirilan) + "\nMaximum Skor: " + QString::number(Max_Skor);
        QMessageBox::information(this, "Bilgi", text);
        close();
    }

    // Sonucu (skorlar.txt) dosyasına kaydedin
    QFile file(Dosya2);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::warning(this, "Bilgi", "Sonuçlar kaydedilemedi");
        close();
    }

    QString Ad = ui->name->text();
    QString matin = "\n" + QString::number(Kesilen) + " " + Ad;
    QTextStream veriler(&file);
    veriler << matin;
    file.close();
}

// dialog.h Dosya:

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QTimer>
#include <QFile>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_start_clicked(); // Başlat düğmesine basın

    void sure_degistir(); // Ekrandaki süreyi sürekli değiştirin

    void dosya(); //"skorlar.txt" ve "konumlar.txt" dosyalardan verileri okuyun

    void KarpuzYarat(); // Bir karpuz oluşturun ve ekrana yazdırın

    void oyun_bitti(); // Tamamlandığında oyun sonucunu gösterin ve "skorlar.txt" dosyasına kaydedin

private:
    Ui::Dialog *ui;

    int sure = 0; // Süre değişkeni

    int index = 0; // Konum değişkeni x ve y matrisinde gezinmek için kullanılır

    int Kacirilan = 0; // Kacirilan karpuz sayısı için değişken

    int Kesilen = 0; // Kesilen karpuz sayısı için değişken

    int Max_Skor = 0; // Dosya "skorlar.txt"'den okunan verilere dayalı olarak sonuçlardaki maksimum değeri saklayan bir değişken

    QList<int> konumlar_x; // Dosya "konumlar.txt"'dan okunan x değerlerinin saklandığı dizi

    QList<int> konumlar_y; // Dosya "konumlar.txt"'dan okunan y değerlerinin saklandığı dizi

    QList<QPushButton *> Karpuz_Liste; // fonkisyon KarpuzYarat()'den oluşturulan karpuzları saklayacak bir dizi
};
#endif // DIALOG_H

// main.cpp Dosya:

#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}