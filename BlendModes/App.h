#include <QApplication>
#include <QWidget>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QSlider>
#include <QListWidget>
#include <QComboBox>

#define ILOSC_WARSTW 3

struct Layer {
    QImage* layerImg;
    int mode;
    float alpha;
    //int layerID;

};

class App : public QWidget
{
    Q_OBJECT
public:
    // Typowa deklaracja konstruktora w Qt.
    // Parametr "parent" okresla rodzica komponenetu.
    // W przypadku naszej klasy parametr ten wskazuje na null
    // co oznacza, ze komponenet nie ma rodzica, jest to
    // komponenet najwyzszego poziomu
    App(QWidget* parent = 0);

    // Deklaracja destruktora
    ~App();

private:
    // Pole przechowujace obrazek
    QImage* img;
    QListWidget* listWidget;
    QComboBox* dropdown;
    QSlider* alphaSlider;

    Layer layers[ILOSC_WARSTW];

    // Grupa przyciskow
    QGroupBox* grupa;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    // czy przycisk zostal klikniety oraz polozenie w ktorym znajdowal sie kursor podczas klikniecia
    int szer;
    int wys;
    int poczX;
    int poczY;


    // Deklaracje funkcji
    void setPixel(int x, int y, int Red, int Green, int Blue);
    void drawBlend();
    void blend(Layer layers);
    void clearImg();

    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void sliderSlot(int val);
    void itemChangedSlot(int val);
    void modeSelect(int index);

    void paintEvent(QPaintEvent*);
signals:
    void valueChanged(int value);
    void currentRowChanged(int value);
    void currentIndexChanged(int index);
};

