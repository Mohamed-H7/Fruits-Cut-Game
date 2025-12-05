#ifndef DIALOG_H
#define DIALOG_H

/*
# ---------------------------
# - Ad-Soyad: Mohamed Hamdo
# - Öğrenci num: 20010011524
# - 1.Ödev
# ---------------------------
*/

#include <QDialog>

#include <QTimer>
#include <QFile>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui {
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

    void sure_degistir(); //Ekrandaki süreyi sürekli değiştirin

    void dosya(); //"skorlar.txt" ve "konumlar.txt" dosyalardan verileri okuyun

    void KarpuzYarat(); //Bir karpuz oluşturun ve ekrana yazdırın

    void oyun_bitti(); //Tamamlandığında oyun sonucunu gösterin ve "skorlar.txt" dosyasına kaydedin

private:
    Ui::Dialog *ui;

    int sure = 0; // Süre değişkeni

    int index = 0; // Konum değişkeni x ve y matrisinde gezinmek için kullanılır

    int Kacirilan = 0; // Kacirilan karpuz sayısı için değişken

    int Kesilen = 0; // Kesilen karpuz sayısı için değişken

    int Max_Skor = 0; // Dosya "skorlar.txt"'den okunan verilere dayalı olarak sonuçlardaki maksimum değeri saklayan bir değişken

    QList<int> konumlar_x; // Dosya "konumlar.txt"'dan okunan x değerlerinin saklandığı dizi

    QList<int> konumlar_y; // Dosya "konumlar.txt"'dan okunan y değerlerinin saklandığı dizi

    QList<QPushButton*> Karpuz_Liste; // fonkisyon KarpuzYarat()'den oluşturulan karpuzları saklayacak bir dizi
};
#endif // DIALOG_H
