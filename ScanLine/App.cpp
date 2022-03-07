#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>
#include <vector>
#include <stdlib.h>     
#include <time.h>       

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
	bg = { 205,205,205 };

	img = new QImage(szer, wys, QImage::Format_RGB32);
	background = new QImage(szer, wys, QImage::Format_RGB32);
	info = new QLabel();
	info->setText("Lewy - dodanie punktu\nPrawy - zamalowanie");
	grupa = new QGroupBox("Sterowanie", this);

	grupa->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 150));

	QVBoxLayout* boxLayout = new QVBoxLayout;

	cleanButton = new QPushButton("Czysc");
	exitButton = new QPushButton("Wyjscie");

	boxLayout->addWidget(cleanButton);
	boxLayout->addWidget(exitButton);
	boxLayout->addWidget(info);


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
			setPixel(j, i, bg.red, bg.green, bg.blue);
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

void App::drawCircle(int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue)
{
	int R = sqrt(pow(end_x - beg_x, 2) + pow(end_y - beg_y, 2));
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

void App::fill()
{
	int max = 0, min = 0, dl = 0;
	dl = points.size();
	max = min = points[0].y;

	for (int i = 0; i < dl; i++) {
		if (points[i].y < min)
			min = points[i].y;
		if (points[i].y > max)
			max = points[i].y;
	}

	std::vector<int> tmp;
	int x;
	for (int i = min; i <= max; i++) {

		for (int j = 0; j < dl - 1; j++) {
			if ((points[j].y <= i && points[j + 1].y > i) || ((points[j + 1].y <= i && points[j].y > i))) {
				x = points[j].x + (i - points[j].y) * (points[j + 1].x - points[j].x) / (points[j + 1].y - points[j].y);
				tmp.push_back(x);
			}
		}

		if ((points[0].y <= i && points[dl - 1].y > i) || ((points[dl - 1].y <= i && points[0].y > i))) {
			x = points[dl - 1].x + (i - points[dl - 1].y) * (points[0].x - points[dl - 1].x) / (points[0].y - points[dl - 1].y);
			tmp.push_back(x);
		}
		std::sort(tmp.begin(), tmp.end());

		int pointSize = tmp.size();
		if (pointSize % 2 == 1)
			pointSize--;

		srand(time(NULL));
		bgColor color;
		color.red = rand() % 255;
		color.green = rand() % 255;
		color.blue = rand() % 255;
		if (pointSize > 1) {
			for (int j = 0; j < pointSize; j += 2) {
				drawLine(tmp[j], i, tmp[j + 1], i, color.red, color.green, color.blue);
			}
		}

		tmp.clear();
	}
}

void App::drawPoint(int x, int y, int rozmiar, int Red, int Green, int Blue)
{
	for (int i = 0; i < rozmiar; i++) {
		drawCircle(x, y, x + i, y + i, Red, Green, Blue);
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

void App::drawShape(int x, int y) {
	if (points.size() == 1)
		drawPoint(points[0].x, points[0].y, 3, 0, 0, 0);
	else if (points.size() == 2) {
		drawLine(points[0].x, points[0].y, points[1].x, points[1].y, 0, 0, 0);
	}
	else if (points.size() > 2) {
		int dl = points.size() - 1;

		drawLine(points[0].x, points[0].y, points[dl].x, points[dl].y, 0, 0, 0);

		for (int i = 0; i < dl; i++) {
			drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, 0, 0, 0);
		}
	}
}

void App::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;

	_x = x;
	_y = y;

	if (Qt::LeftButton == event->button() && (x >= 0) && (y >= 0) && (x < szer) && (y < wys))
	{
		copyImg(img, background);
		points.push_back(Point{ x,y });
		drawShape(x, y);
	}
	else if (Qt::RightButton == event->button() && points.size() > 2)
	{
		copyImg(img, background);
		fill();
		copyImg(background, img);
		points.clear();
	}
	update();

}

void App::mouseReleaseEvent(QMouseEvent* event)
{
	//int x = event->x();
	//int y = event->y();

	//x = x - poczX;
	//y = y - poczY;
	//copyImg(background, img);
	//update();
}

void App::mouseMoveEvent(QMouseEvent* event)
{
	//int x = event->x();
	//int y = event->y();

	//x = x - poczX;
	//y = y - poczY;

	//copyImg(img, background);
	//drawShape(x, y);

	//if (event->buttons() == Qt::LeftButton) {
	//}
	//else if (event->buttons() == Qt::RightButton) {

	//}
	//else if (event->buttons() == Qt::MiddleButton) {
	//}

	//update();

}
