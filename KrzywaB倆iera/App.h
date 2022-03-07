#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSpinBox>
#include <QLabel>

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

    // Pole przechowujace obrazek
    QImage* img;
    QImage* background;

    // Przyciski
    QPushButton* cleanButton;
    QPushButton* exitButton;
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
    std::vector<Point> points;


    // Deklaracje funkcji
    void czysc();

    void setPixel(int x, int y, int Red, int Green, int Blue);
    void drawLine(int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue);

    void drawCircle(int beg_x, int beg_y, int R, int Red, int Green, int Blue);
    void drawPoint(int x, int y, int rozmiar, int Red, int Green, int Blue);
    void drawBeziera();

    void copyImg(QImage* dest, QImage* src);

    void sortPos(int& val_1, int& val_2);

    int pointInArea(int x, int y, std::vector<Point> points);

    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void slot_czysc();
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent*);
};

