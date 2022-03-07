#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>
#include <vector>
#include <queue>


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
	bg = {205,205,205};

	img = new QImage(szer, wys, QImage::Format_RGB32);
	background = new QImage(szer, wys, QImage::Format_RGB32);
	info = new QLabel();
	info->setText("Lewy - linia\nPrawy - okrag\nSrodkowy - zamalowanie");
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
			setPixel(j, i, bg.red, bg.green, bg.blue );
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

void App::fill(int x, int y, int Red, int Green, int Blue)
{
	std::queue<std::pair<int,int>> kolejka;
	kolejka.push(std::pair<int, int>(x, y));;
	while (!kolejka.empty())
	{
		int tmp_x = kolejka.front().first;
		int tmp_y = kolejka.front().second;
		kolejka.pop();
		if (isSameAsBackground(tmp_x, tmp_y)) {
			setPixel(tmp_x, tmp_y, Red, Green, Blue);

			if((tmp_x - 1 >= 0) && (tmp_y >= 0) && (tmp_x - 1 < szer) && (tmp_y < wys))
				kolejka.push(std::pair<int, int>(tmp_x - 1, tmp_y));
			
			if ((tmp_x + 1 >= 0) && (tmp_y >= 0) && (tmp_x + 1 < szer) && (tmp_y < wys))
				kolejka.push(std::pair<int, int>(tmp_x + 1, tmp_y));
			
			if ((tmp_x>= 0) && (tmp_y - 1 >= 0) && (tmp_x < szer) && (tmp_y - 1 < wys))
				kolejka.push(std::pair<int, int>(tmp_x , tmp_y - 1));
			
			if ((tmp_x >= 0) && (tmp_y + 1 >= 0) && (tmp_x < szer) && (tmp_y + 1 < wys))
				kolejka.push(std::pair<int, int>(tmp_x, tmp_y + 1));
		}
	}
}

bool App::isSameAsBackground(int x, int y)
{
	unsigned char* ptr;
	ptr = img->bits();
	if((ptr[szer * 4 * y + 4 * x] == bg.red) && (ptr[szer * 4 * y + 4 * x + 1] == bg.green) && (ptr[szer * 4 * y + 4 * x + 2] == bg.blue))
		return true;

	return false;
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

void App::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;
	
	_x = x;
	_y = y;

	if (Qt::LeftButton == event->button())
	{
		copyImg(img, background);
		update();
	}
	else if (Qt::RightButton == event->button())
	{
		copyImg(img, background);
		update();
	}
	else if (event->buttons() == Qt::MiddleButton) {
		fill(x, y, 0, 250, 10);
	}
}

void App::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	x = x - poczX;
	y = y - poczY;
	copyImg(background, img);
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
		drawLine(_x,_y,x,y,0, 0, 0);
	}
	else if (event->buttons() == Qt::RightButton) {
		drawCircle(_x, _y, x,y,0,0,0);
	}
	else if (event->buttons() == Qt::MiddleButton) {
		fill(x, y,0,250,10);
	}

	update();

}
