#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSpinBox>
#include <QLabel>

class Circle_and_eclipse : public QWidget
{
    Q_OBJECT
public:
    // Typowa deklaracja konstruktora w Qt.
    // Parametr "parent" okresla rodzica komponenetu.
    // W przypadku naszej klasy parametr ten wskazuje na null
    // co oznacza, ze komponenet nie ma rodzica, jest to
    // komponenet najwyzszego poziomu
    Circle_and_eclipse(QWidget* parent = 0);

    // Deklaracja destruktora
    ~Circle_and_eclipse();

private:

    // Pole przechowujace obrazek
    QImage* img;
    QImage* background;

    // Przyciski
    QPushButton* cleanButton;
    QPushButton* exitButton;
    QSpinBox *spinBox;
    QLabel* label;
    // Grupa przyciskow
    QGroupBox* grupa;

    // Pola przechowujace szerokosc i wysokosc rysunku
    // oraz wspolrzedne jego lewego gornego naroznika
    // czy przycisk zostal klikniety oraz polozenie w ktorym znajdowal sie kursor podczas klikniecia
    int szer;
    int wys;
    int poczX;
    int poczY;

    int start_x, start_y;

    int numbersOfPoints;

    // Deklaracje funkcji
    void czysc();

    void setPixel(int x, int y, int R, int G, int B);
    void drawLine(int beg_x, int beg_y, int end_x, int end_y);

    void drawCircle(int beg_x, int beg_y, int end_x, int end_y);
    void _drawEllipse(int beg_x, int beg_y, int end_x, int end_y);

    void copyImg(QImage* dest, QImage* src);

    void sortPos(int& val_1, int& val_2);

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

