#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QSlider>
#include <QGroupBox>

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
	App(QWidget* parent = 0);
	~App();

private:
	QImage* img_texture0;
	QImage* img_texture1;
	QImage* img_texture0_COPY;
	QImage* img_texture1_COPY;
	QImage* img_draw;

	std::vector<Point> point_t0; // punkty na texturze_1
	std::vector<Point> point_t1; // punkty na texturze_2 
	std::vector<Point> point_draw; // punkty na texturze_2 


	QGroupBox* grupa;
	QSlider* morphSlider;

	int szer;
	int wys;
	int poczX_Texture0;
	int poczX_Texture1;
	int poczX_Draw;
	int poczY;
	int N;
	int id;

	void setPixel(QImage* img, int x, int y, int Red, int Green, int Blue);
	void copyImg(QImage* dest, QImage* src);
	void drawPoint(QImage* img, int x, int y, int Red, int Green, int Blue);
	void drawPoints();
	void drawLines();
	void drawLine(QImage* img, int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue);
	void sortPos(int& val_1, int& val_2);
	void morphing();
	void clearDraw();
	bool checkPoints(std::vector<Point> points, int p1, int p2, int k);
	int calcX(std::vector<Point> points, int p1, int p2, int k);
	int pointInArea(int x, int y, std::vector<Point> points);
	Rgb getPixel(QImage* img, int x, int y);


private slots:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent*);
	void sliderFunc(int value);
signals:
	void valueChanged(int value);
};

