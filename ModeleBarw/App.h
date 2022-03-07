#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSlider>
#include <QLabel>

struct bgColor {
    int red;
    int green;
    int blue;
};

struct Point {
    int x;
    int y;
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
    bgColor bg;
    std::vector<Point> points;
    // Pole przechowujace obrazek
    QImage* img;
    QImage* background;

    QSlider* redSpin;
    QSlider* greenSpin;
    QSlider* blueSpin;

    QSlider* hSpin;
    QSlider* sSpin;
    QSlider* vSpin;

    // Przyciski
    QPushButton* exitButton;
    QLabel *R, *G, *B, *H, *S, *V;
    QLabel *rgb, *hsv;
    // Grupa przyciskow
    QGroupBox* grupa;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    // czy przycisk zostal klikniety oraz polozenie w ktorym znajdowal sie kursor podczas klikniecia
    int szer;
    int wys;
    int poczX;
    int poczY;

    int ID;
    int _x, _y;

    // Deklaracje funkcji
    void setPixel(int x, int y, int Red, int Green, int Blue);

    void copyImg(QImage* dest, QImage* src);


    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void redSpinbox(int value);
    void greenSpinbox(int value);
    void blueSpinbox(int value);

    void hSpinbox(int value);
    void sSpinbox(int value);
    void vSpinbox(int value);


    void rgbSpinbox(int value, char rgb);
    void hsvSpinbox(int value, char hsv);

    bgColor hsvToRgb(int H, int S, int V);


    void paintEvent(QPaintEvent*);
signals:
    void valueChanged(int value);
};

