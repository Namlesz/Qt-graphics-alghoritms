#include <QApplication>
#include <QWidget>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QSlider>

struct Point {
	int x;
	int y;
};

struct Rgb {
	int red;
	int green;
	int blue;
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
	QImage* img_org;
	QImage* test;


	// Grupa przyciskow
	QGroupBox* grupa1;
	QGroupBox* grupa2;
	QGroupBox* grupa3;
	QGroupBox* grupa4;


	QSlider* x_translationS;
	QSlider* y_translationS;
	QSlider* rotationS;
	QSlider* x_scalingS;
	QSlider* y_scalingS;
	QSlider* x_shearingS;
	QSlider* y_shearingS;


	// Pola przechowujace szerokosc i wysokosc rysunku
	// oraz wspolrzedne jego lewego gornego naroznika
	// czy przycisk zostal klikniety oraz polozenie w ktorym znajdowal sie kursor podczas klikniecia
	int szer;
	int wys;
	int poczX;
	int poczY;

	const float x0 = 150;
	const float y0 = 150;

	float M[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
	float T[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float S[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float Shx[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float Shy[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float R[3][3] = { {1,1,0},{1,1,0},{0,0,1} };
	float T0[3][3] = { {1,0,2 * x0},{0,1,2 * y0},{0,0,1} };
	float T0n[3][3] = { {1,0,-x0},{0,1,-y0},{0,0,1} };
	
	float neg_T[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float neg_S[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float neg_Shx[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float neg_Shy[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	float neg_R[3][3] = { {1,1,0},{1,1,0},{0,0,1} };


	// Deklaracje funkcji
	void setPixel(QImage* img, int x, int y, int Red, int Green, int Blue);
	void clearImg();
	void copyImg(QImage* dest, QImage* src);
	void transform();

	Rgb getPixel(QImage* img, int x, int y);

	void multiplication(float tab1[3][3], float tab2[3][3]);
	void calculate_pos_M();
	void calculate_neg_M();
	Rgb interpolation(float x, float y);

	void multiplicationPoints(float& x, float& y);
	void clearMatrix(float tab[3][3]);

	// Deklaracje slotow, czyli funkcji wywolywanych
	// po wystapieniu zdarzen zwiazanych z GUI
	// np. klikniecie na przycisk, ruch myszka
private slots:
	void slot_czysc();

	void x_translationBox(int value);
	void y_translationBox(int value);

	void rotationSBox(int value);

	void x_scalingBox(int value);
	void y_scalingBox(int value);

	void x_shearingBox(int value);
	void y_shearingBox(int value);

	void paintEvent(QPaintEvent*);

signals:

	void valueChanged(int value);
};

