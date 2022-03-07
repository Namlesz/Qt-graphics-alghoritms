#include "Circle_and_eclipse.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>

Circle_and_eclipse::Circle_and_eclipse(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Elipsa i kolo");

	//Zmieniamy rozmiar okna
	resize(800, 650);

	szer = 600;
	wys = 600;
	poczX = poczY = 25;
	start_y = start_x = 0;
	numbersOfPoints = 3;

	img = new QImage(szer, wys, QImage::Format_RGB32);
	background = new QImage(szer, wys, QImage::Format_RGB32);

	grupa = new QGroupBox("Sterowanie", this);

	grupa->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 100));

	QVBoxLayout* boxLayout = new QVBoxLayout;

	cleanButton = new QPushButton("Czysc");
	exitButton = new QPushButton("Wyjscie");
	spinBox = new QSpinBox();
	label = new QLabel(this);
	label->setText("Liczba punktow:");
	spinBox->setRange(3, 50);

	boxLayout->addWidget(cleanButton);
	boxLayout->addWidget(exitButton);
	boxLayout->addWidget(label);
	boxLayout->addWidget(spinBox);


	grupa->setLayout(boxLayout);

	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(cleanButton, SIGNAL(clicked()), this, SLOT(slot_czysc()));
	connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) { numbersOfPoints = i; });


	//sledzenie polozenia myszki
	setMouseTracking(true);
}

// Definicja destruktora
Circle_and_eclipse::~Circle_and_eclipse()
{
	delete img;
	delete background;
}

void Circle_and_eclipse::paintEvent(QPaintEvent*)
{
	QPainter p(this);

	p.drawImage(poczX, poczY, *img);
}

void Circle_and_eclipse::slot_czysc()
{
	czysc();
	//czyscimy rowniez kopie obrazu
	copyImg(background, img);
	update();
}

void Circle_and_eclipse::czysc()
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

void Circle_and_eclipse::setPixel(int x, int y, int R, int G, int B)
{
	if ((x >= 0) && (y >= 0) && (x < szer) && (y < wys)) {
		unsigned char* ptr;
		ptr = img->bits();
		ptr[szer * 4 * y + 4 * x] = R;
		ptr[szer * 4 * y + 4 * x + 1] = G;
		ptr[szer * 4 * y + 4 * x + 2] = B;
	}
}

void Circle_and_eclipse::drawLine(int beg_x, int beg_y, int end_x, int end_y)
{


	// w przypadku linii pionowej
	if (end_x == beg_x) {
		//sortujemy 2 wartosci
		sortPos(beg_y, end_y);
		//ustawiamy kolor pikseli
		for (int i = beg_y; i < end_y; i++) {
			setPixel(end_x, i, 0, 0, 0);
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

			for (int i = beg_x; i < end_x ; i++) {
				calcY = round(a * i + b);
				setPixel(i, calcY , 0, 0, 0);
			}
		}

		//jesli |a| > 1 to liczymy wartosc x dla kolejnych y 
		else {

			sortPos(beg_y, end_y); //sortowanie 
			int calcX = 0;

			for (int i = beg_y; i < end_y ; i++) {
				calcX = round(double(i - b) / double(a));
				setPixel(calcX , i, 0, 0, 0);
			}
		}
	}
}

void Circle_and_eclipse::drawCircle(int beg_x, int beg_y, int end_x, int end_y)
{
	int R = sqrt(pow(end_x - beg_x, 2) + pow(end_y - beg_y, 2));
	int cX = 0 , cY = 0;
	int temp = R / sqrt(2);

	for (int i = 0; i <= temp; i++) {
		cY = sqrt(pow(R, 2) - pow(i, 2));

		setPixel(i + beg_x, cY + beg_y, 0, 0, 0);
		setPixel(-i + beg_x, cY + beg_y, 0, 0, 0);
		setPixel(-i + beg_x, -cY + beg_y, 0, 0, 0);
		setPixel(i + beg_x, -cY + beg_y, 0, 0, 0);
	}
	for (int i = 0; i <= temp; i++) {
		cX = sqrt(pow(R, 2) - pow(i, 2));

		setPixel(cX + beg_x, i + beg_y, 0, 0, 0);
		setPixel(-cX + beg_x, i + beg_y, 0, 0, 0);
		setPixel(-cX + beg_x, -i + beg_y, 0, 0, 0);
		setPixel(cX + beg_x, -i + beg_y, 0, 0, 0);
	}
}

void Circle_and_eclipse::_drawEllipse(int beg_x, int beg_y, int end_x, int end_y) {

	double A = 0;
	double x_3 = (end_x - beg_x) * cos(A);
	double y_3 = (end_y - beg_y) * sin(A);

	A += 2 * 3.14 / numbersOfPoints;

	double x_last = x_3;
	double y_last = y_3;

	for (int k = 1; k <= numbersOfPoints; k++) {
		x_3 = (end_x - beg_x) * cos(A*k);
		y_3 = (end_y - beg_y) * sin(A*k);

		drawLine(x_3 + beg_x, y_3 + beg_y, x_last + beg_x, y_last + beg_y);
	
		x_last = x_3;
		y_last = y_3;
	}
}

void Circle_and_eclipse::copyImg(QImage* dest, QImage* src)
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
			ptr_dest[szer * 4 * i + 4 * j + 1] = ptr_src[szer * 4 * i + 4 * j];
			ptr_dest[szer * 4 * i + 4 * j + 2] = ptr_src[szer * 4 * i + 4 * j];
		}
	}
}

void Circle_and_eclipse::sortPos(int& val_1, int& val_2)
{
	if (val_1 > val_2) {
		int tmp = val_1;
		val_1 = val_2;
		val_2 = tmp;
	}
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void Circle_and_eclipse::mousePressEvent(QMouseEvent* event)
{
	// Pobieramy wspolrzedne punktu klikniecia
	// Sa to wspolrzedne wzgledem glownego okna 
	int x = event->x();
	int y = event->y();

	// Przesuwamy je zeby nie rysowac od brzegu
	x = x - poczX;
	y = y - poczY;

	// Sprawdzamy ktory przycisk myszy zostal klkniety i czy zostal klikniety w g ranicach obrazu
	if (Qt::RightButton == event->button() || Qt::LeftButton == event->button())
	{
		start_x = x; //ustawiamy poczatkowe wartosci x i y na miejsce klikniecia
		start_y = y;
	}

}

void Circle_and_eclipse::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->x(); 
	int y = event->y(); 
	x = x - poczX;
	y = y - poczY;

	if (event->button() == Qt::LeftButton)
	{
		drawCircle(start_x, start_y, x, y);
		copyImg(background, img); //kopiujemy obraz zmodyfikowany do tla
		update();
	}
	else if (event->button() == Qt::RightButton) {
		_drawEllipse(start_x, start_y, x, y);
		copyImg(background, img); //kopiujemy obraz zmodyfikowany do tla
		update();
	}

}

void Circle_and_eclipse::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();
	x = x - poczX;
	y = y - poczY;
	//jesli poruszamy myszką i mamy klikniety przycisk
	if (event->buttons() == Qt::LeftButton) {
		copyImg(img, background); //kopiujemy tlo do miejsca w ktorym rysuyjemy
		drawCircle(start_x, start_y, x, y);
		update();
	}
	else if (event->buttons() == Qt::RightButton) {
		copyImg(img, background); //kopiujemy tlo do miejsca w ktorym rysuyjemy
		_drawEllipse(start_x, start_y, x, y);
		update();
	}
}
