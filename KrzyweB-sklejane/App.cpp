#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>
#include <vector>


App::App(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Elipsa i kolo");

	//Zmieniamy rozmiar okna
	resize(800, 650);

	szer = 600;
	wys = 600;
	poczX = poczY = 25;
	ID = 0;

	img = new QImage(szer, wys, QImage::Format_RGB32);
	background = new QImage(szer, wys, QImage::Format_RGB32);

	grupa = new QGroupBox("Sterowanie", this);

	grupa->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 100));

	QVBoxLayout* boxLayout = new QVBoxLayout;

	cleanButton = new QPushButton("Czysc");
	exitButton = new QPushButton("Wyjscie");

	boxLayout->addWidget(cleanButton);
	boxLayout->addWidget(exitButton);


	grupa->setLayout(boxLayout);

	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(cleanButton, SIGNAL(clicked()), this, SLOT(slot_czysc()));

	//sledzenie polozenia myszki
	setMouseTracking(true);
}

// Definicja destruktora
App::~App()
{
	delete img;
	delete background;
}

void App::paintEvent(QPaintEvent*)
{
	QPainter p(this);

	p.drawImage(poczX, poczY, *img);
}

void App::slot_czysc()
{
	czysc();
	points.clear();
	//czyscimy rowniez kopie obrazu
	copyImg(background, img);
	update();
}

void App::czysc()
{
	int i, j;

	// ustawiamy piksele na wartosc poczatkowa obrazu
	for (i = 0; i < wys; i++)
	{
		for (j = 0; j < szer; j++)
		{
			setPixel(j, i, 205, 205, 205);
		}
	}
}

void App::setPixel(int x, int y, int Red, int Green, int Blue)
{
	if ((x >= 0) && (y >= 0) && (x < szer) && (y < wys)) {
		unsigned char* ptr;
		ptr = img->bits();
		ptr[szer * 4 * y + 4 * x] = Red;
		ptr[szer * 4 * y + 4 * x + 1] = Green;
		ptr[szer * 4 * y + 4 * x + 2] = Blue;
	}
}

void App::drawLine(int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue)
{
	// w przypadku linii pionowej
	if (end_x == beg_x) {
		//sortujemy 2 wartosci
		sortPos(beg_y, end_y);
		//ustawiamy kolor pikseli
		for (int i = beg_y; i < end_y; i++) {
			setPixel(end_x, i, Red, Green, Blue);
		}
	}
	else {

		double a, b;
		//wyliczamy a i b 
		a = (end_y - beg_y) / double(end_x - beg_x);
		b = beg_y - a * beg_x;

		//jesli |a| < 1 to liczymy wartosc y dla kolejnych x 
		if (abs(a) < 1) {

			sortPos(beg_x, end_x); //sortowanie 
			int calcY = 0;

			for (int i = beg_x; i <= end_x; i++) {
				calcY = round(a * i + b);
				setPixel(i, calcY, Red, Green, Blue);
			}
		}

		//jesli |a| > 1 to liczymy wartosc x dla kolejnych y 
		else {

			sortPos(beg_y, end_y); //sortowanie 
			int calcX = 0;

			for (int i = beg_y; i <= end_y; i++) {
				calcX = round(double(i - b) / double(a));
				setPixel(calcX, i, Red, Green, Blue);
			}
		}
	}
}

void App::drawCircle(int beg_x, int beg_y, int R, int Red, int Green, int Blue)
{
	int cX = 0, cY = 0;
	int temp = R / sqrt(2);

	for (int i = 0; i <= temp; i++) {
		cY = sqrt(pow(R, 2) - pow(i, 2));

		setPixel(i + beg_x, cY + beg_y, Red, Green, Blue);
		setPixel(-i + beg_x, cY + beg_y, Red, Green, Blue);
		setPixel(-i + beg_x, -cY + beg_y, Red, Green, Blue);
		setPixel(i + beg_x, -cY + beg_y, Red, Green, Blue);
	}
	for (int i = 0; i <= temp; i++) {
		cX = sqrt(pow(R, 2) - pow(i, 2));

		setPixel(cX + beg_x, i + beg_y, Red, Green, Blue);
		setPixel(-cX + beg_x, i + beg_y, Red, Green, Blue);
		setPixel(-cX + beg_x, -i + beg_y, Red, Green, Blue);
		setPixel(cX + beg_x, -i + beg_y, Red, Green, Blue);
	}
}

void App::drawPoint(int x, int y, int rozmiar, int Red, int Green, int Blue)
{
	for (int i = 0; i < rozmiar; i++) {
		drawCircle(x, y, i, Red, Green, Blue);
	}
}

void App::drawB_sklejane()
{
	for (int i = 0; i < points.size(); i++) {
		drawPoint(points[i].x, points[i].y, 3, 0, 0, 0);
	}

	if (points.size() >= 4) {
		double t;
		int x0, y0, x1, y1;
		int rozmiar = points.size() - 4;
		int k = 0;
		double tmp = 0.1666666666666667;

		while (rozmiar >= 0) {
			t = 0;

			for (int i = 0; i < 20; i++) {
				x0 = tmp * (-t * t * t + 3 * t * t - 3 * t + 1) * points[k].x + tmp * (3 * t * t * t - 6 * t * t + 4) * points[k + 1].x +
					tmp * (-3 * t * t * t + 3 * t * t + 3 * t + 1) * points[k + 2].x + tmp * t * t * t * points[k + 3].x;
				y0 = tmp * (-t * t * t + 3 * t * t - 3 * t + 1) * points[k].y + tmp * (3 * t * t * t - 6 * t * t + 4) * points[k + 1].y +
					tmp * (-3 * t * t * t + 3 * t * t + 3 * t + 1) * points[k + 2].y + tmp * t * t * t * points[k + 3].y;
				t += 0.05;
				x1 = tmp * (-t * t * t + 3 * t * t - 3 * t + 1) * points[k].x + tmp * (3 * t * t * t - 6 * t * t + 4) * points[k + 1].x +
					tmp * (-3 * t * t * t + 3 * t * t + 3 * t + 1) * points[k + 2].x + tmp * t * t * t * points[k + 3].x;
				y1 = tmp * (-t * t * t + 3 * t * t - 3 * t + 1) * points[k].y + tmp * (3 * t * t * t - 6 * t * t + 4) * points[k + 1].y +
					tmp * (-3 * t * t * t + 3 * t * t + 3 * t + 1) * points[k + 2].y + tmp * t * t * t * points[k + 3].y;

				drawLine(x0, y0, x1, y1, 0, 0, 0);
			}
			k += 1;
			rozmiar -= 1;
		}
	}

}

void App::copyImg(QImage* dest, QImage* src)
{
	unsigned char* ptr_dest, * ptr_src;

	ptr_dest = dest->bits();
	ptr_src = src->bits();
	int i, j;

	//iterujemy po kolejnych pikselach obrazu i ustawiamy taki sam kolor w 2 obrazie
	for (i = 0; i < wys; i++)
	{
		for (j = 0; j < szer; j++)
		{
			ptr_dest[szer * 4 * i + 4 * j] = ptr_src[szer * 4 * i + 4 * j];
			ptr_dest[szer * 4 * i + 4 * j + 1] = ptr_src[szer * 4 * i + 4 * j + 1];
			ptr_dest[szer * 4 * i + 4 * j + 2] = ptr_src[szer * 4 * i + 4 * j + 2];
		}
	}
}

void App::sortPos(int& val_1, int& val_2)
{
	if (val_1 > val_2) {
		int tmp = val_1;
		val_1 = val_2;
		val_2 = tmp;
	}
}

int App::pointInArea(int x, int y, std::vector<Point> points)
{
	for (int i = 0; i < points.size(); i++) {

		double dl = sqrt(pow(x - points[i].x, 2) + pow(y - points[i].y, 2));
		if (dl < 15)
		{
			return i;
		}
	}

	return -1;
}

void App::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;

	if (Qt::LeftButton == event->button())
	{
		copyImg(img, background);
		ID = pointInArea(x, y, points);
		//rysowanie
		if (ID == -1) {
			points.push_back(Point{ x,y });
			ID = points.size() - 1;
		}
		else {
			points[ID].x = x;
			points[ID].y = y;
		}
		drawB_sklejane();
		update();
	}
	else if (Qt::RightButton == event->button())
	{
		copyImg(img, background);
		//rysowanie
		ID = pointInArea(x, y, points);
		if (ID >= 0) {
			points.erase(points.begin() + ID);
		}
		drawB_sklejane();
		update();
	}
}

void App::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;
	copyImg(img, background);
	drawB_sklejane();
	update();
}

void App::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;

	copyImg(img, background);

	if (event->buttons() == Qt::LeftButton) {
		points.at(ID).x = x;
		points.at(ID).y = y;
		update();
	}
	else {
		ID = pointInArea(x, y, points);
		if (ID >= 0) {
			drawCircle(points[ID].x, points[ID].y, 15, 0, 255, 0);
			update();
		}
	}
	drawB_sklejane();
	update();

}
