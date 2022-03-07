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

	R = new QLabel;
	G = new QLabel;
	B = new QLabel;
	H = new QLabel;
	S = new QLabel;
	V = new QLabel;
	rgb = new QLabel;
	hsv = new QLabel;

	R->setText("R");
	G->setText("G");
	B->setText("B");
	H->setText("H");
	S->setText("S");
	V->setText("V");
	rgb->setText("_________RGB_________");
	hsv->setText("_________HSV_________");

	redSpin = new QSlider(Qt::Horizontal);
	greenSpin = new QSlider(Qt::Horizontal);
	blueSpin = new QSlider(Qt::Horizontal);

	hSpin = new QSlider(Qt::Horizontal);
	sSpin = new QSlider(Qt::Horizontal);
	vSpin = new QSlider(Qt::Horizontal);


	redSpin->setRange(0, 255);
	greenSpin->setRange(0, 255);
	blueSpin->setRange(0, 255);

	hSpin->setRange(0, 360);
	sSpin->setRange(0, 100);
	vSpin->setRange(0, 100);

	grupa = new QGroupBox("Sterowanie", this);

	grupa->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 350));

	QVBoxLayout* boxLayout = new QVBoxLayout;

	exitButton = new QPushButton("Wyjscie");

	boxLayout->addWidget(exitButton);
	boxLayout->addWidget(rgb);
	boxLayout->addWidget(R);
	boxLayout->addWidget(redSpin);

	boxLayout->addWidget(G);
	boxLayout->addWidget(greenSpin);

	boxLayout->addWidget(B);
	boxLayout->addWidget(blueSpin);

	boxLayout->addWidget(hsv);
	boxLayout->addWidget(H);
	boxLayout->addWidget(hSpin);

	boxLayout->addWidget(S);
	boxLayout->addWidget(sSpin);

	boxLayout->addWidget(V);
	boxLayout->addWidget(vSpin);

	grupa->setLayout(boxLayout);

	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	connect(redSpin, SIGNAL(valueChanged(int)), this, SLOT(redSpinbox(int)));
	connect(greenSpin, SIGNAL(valueChanged(int)), this, SLOT(greenSpinbox(int)));
	connect(blueSpin, SIGNAL(valueChanged(int)), this, SLOT(blueSpinbox(int)));

	connect(hSpin, SIGNAL(valueChanged(int)), this, SLOT(hSpinbox(int)));
	connect(sSpin, SIGNAL(valueChanged(int)), this, SLOT(sSpinbox(int)));
	connect(vSpin, SIGNAL(valueChanged(int)), this, SLOT(vSpinbox(int)));
	redSpinbox(0);
}

// Definicja destruktora
App::~App()
{
	delete img;
}

void App::paintEvent(QPaintEvent*)
{
	QPainter p(this);

	p.drawImage(poczX, poczY, *img);
}

void App::setPixel(int x, int y, int Red, int Green, int Blue)
{
	if ((x >= 0) && (y >= 0) && (x < szer) && (y < wys)) {
		unsigned char* ptr;
		ptr = img->bits();
		ptr[szer * 4 * y + 4 * x] = Blue;
		ptr[szer * 4 * y + 4 * x + 1] = Green;
		ptr[szer * 4 * y + 4 * x + 2] = Red;
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

void App::redSpinbox(int value) {
	rgbSpinbox(value, 'r');
}

void App::greenSpinbox(int value) {
	rgbSpinbox(value, 'g');
}

void App::blueSpinbox(int value) {
	rgbSpinbox(value, 'b');
}

void App::hSpinbox(int value)
{
	hsvSpinbox(value, 'h');
}

void App::sSpinbox(int value)
{
	hsvSpinbox(value, 's');
}

void App::vSpinbox(int value)
{
	hsvSpinbox(value, 'v');
}

void App::rgbSpinbox(int value, char rgb) {
	double color_1 = 0, color_2 = 0;
	for (int i = 0; i < szer; i++) {
		color_1 = 0;
		for (int j = 0; j < wys; j++) {
			if (rgb == 'r')
				setPixel(i, j, value, color_1, color_2);
			else if (rgb == 'g')
				setPixel(i, j, color_1, value, color_2);
			else 
				setPixel(i, j, color_1, color_2, value);

			color_1 += 255.0 / wys;
		}
		color_2 += 255.0 / szer;
	}
	update();
}

void App::hsvSpinbox(int value, char hsv)
{
	double color_1 = 0, color_2 = 0;
	bgColor hsvPallete;
	for (int i = 0; i < szer; i++) {
		color_1 = 0;
		for (int j = 0; j < wys; j++) {
			
			if (hsv == 'h')
				hsvPallete = hsvToRgb(value, color_2, color_1);
			else if (hsv == 's')
				hsvPallete = hsvToRgb(color_1, value, color_2);
			else
				hsvPallete = hsvToRgb(color_1, color_2, value);
				
			setPixel(i, j, hsvPallete.red, hsvPallete.green, hsvPallete.blue);

			if (hsv == 'h')
				color_1 += 100.0 / wys;
			else
				color_1 += 360.0 / wys;
		}
			color_2 += 100.0 / szer;
	}
	update();
}

bgColor App::hsvToRgb(int H, int S, int V)
{
	float s = S / 100.0;
	float v = V / 100.0;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	bgColor result;
	result.red = (r + m) * 255;
	result.green = (g + m) * 255;
	result.blue = (b + m) * 255;
	
	return result;
}
