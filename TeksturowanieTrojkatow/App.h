#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>

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
	QImage* img_texture;
	QImage* img_draw;
	QImage* img_texture_COPY;
	QImage* img_draw_COPY;

	std::vector<Point> point_t; // punkty na texturze
	std::vector<Point> point_d; // punkty na obszarze rysujacym 


	int szer;
	int wys;
	int poczX_Texture;
	int poczX_Draw;
	int poczY;

	int id;

	void setPixel(QImage* img, int x, int y, int Red, int Green, int Blue);
	void copyImg(QImage* dest, QImage* src);
	void drawPoint(QImage* img, int x, int y, int Red, int Green, int Blue);
	void drawPoints();
	void drawLines();
	void drawLine(QImage* img, int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue);
	void sortPos(int& val_1, int& val_2);
	void triangleTexture();
	bool checkPoints(std::vector<Point> points, int p1, int p2, int k);
	int calcX(std::vector<Point> points, int p1, int p2, int k);
	Rgb getPixel(QImage* img, int x, int y);
	Rgb interpolation(double x, double y);

	int pointInArea(int x, int y, std::vector<Point> points);


private slots:
	void slot_czysc();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent*);
};

