#include <QApplication>
#include <QWidget>
#include <QGroupBox>
#include <QPainter>
#include <QImage>
#include <QSlider>
#include <math.h>
#define POS 0
struct Point3D {
	double x;
	double y;
	double z;
};

struct Point2D {
	double x;
	double y;
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
	App(QWidget* parent = 0);
	~App();

private:
	QImage* img;

	QGroupBox* translation;
	QGroupBox* rotation;
	QGroupBox* scaling;

	QSlider* translation_x;
	QSlider* translation_y;
	QSlider* translation_z;

	QSlider* rotation_x;
	QSlider* rotation_y;
	QSlider* rotation_z;

	QSlider* scaling_x;
	QSlider* scaling_y;
	QSlider* scaling_z;

	Point3D square[8];

	int szer;
	int wys;
	int poczX;
	int poczY;

	const float x0 = 150;
	const float y0 = 150;

	double M[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
	double T[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
	double To[4][4] = { {1,0,0,POS}, {0,1,0,POS}, {0,0,1,300}, {0,0,0,1} };
	double ToN[4][4] = { {1,0,0,-50}, {0,1,0,-50}, {0,0,1,-50}, {0,0,0,1} };
	double S[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
	double Rx[4][4] = { {1,0,0,0}, {0,cos(0),-sin(0),0}, {0,sin(0),cos(0),0}, {0,0,0,1} };
	double Ry[4][4] = { {cos(0),0,sin(0),0}, {0,1,0,0}, {-sin(0),0,cos(0),0}, {0,0,0,1} };
	double Rz[4][4] = { {cos(0),-sin(0),0,0}, {sin(0),cos(0),0,0}, {0,0,1,0}, {0,0,0,1} };

	void setPixel(QImage* img, int x, int y, int Red, int Green, int Blue);
	void clearImg();
	void sortPos(int& val_1, int& val_2);
	void copyImg(QImage* dest, QImage* src);
	void transform();
	void multiplicationPoints(Point3D& p);
	void clearMatrix(double tab[4][4]);
	void drawLine(int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue);
	void multiplicationMatrix(double tab1[4][4], double tab2[4][4]);
	void calc_M_matrix();
	void drawSquare(Point2D points[8], bool visible[]);
	void isVisible(bool visible[], Point3D squarePos[]);

private slots:

	void trans_funct_x(int value);
	void trans_funct_y(int value);
	void trans_funct_z(int value);

	void rotate_funct_x(int value);
	void rotate_funct_y(int value);
	void rotate_funct_z(int value);

	void scal_funct_x(int value);
	void scal_funct_y(int value);
	void scal_funct_z(int value);

	void paintEvent(QPaintEvent*);

signals:

	void valueChanged(int value);
};

