#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>
#define IMG_SIZE 300

App::App(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Elipsa i kolo");

	//Zmieniamy rozmiar okna
	resize(850, 650);

	szer = 600;
	wys = 600;
	poczX = poczY = 25;

	img = new QImage(szer, wys, QImage::Format_RGB32);
	img_org = new QImage("../Circle_and_eclipse/img.jpg");

	x_translationS = new QSlider(Qt::Horizontal);
	y_translationS = new QSlider(Qt::Horizontal);

	rotationS = new QSlider(Qt::Horizontal);

	x_scalingS = new QSlider(Qt::Horizontal);
	y_scalingS = new QSlider(Qt::Horizontal);

	x_shearingS = new QSlider(Qt::Horizontal);
	y_shearingS = new QSlider(Qt::Horizontal);

	x_translationS->setRange(-200, 200);
	y_translationS->setRange(-200, 200);

	rotationS->setRange(0, 360);

	x_scalingS->setRange(0, 200);
	y_scalingS->setRange(0, 200);
	x_scalingS->setValue(100);
	y_scalingS->setValue(100);

	x_shearingS->setRange(-100, 100);
	y_shearingS->setRange(-100, 100);


	grupa1 = new QGroupBox("Translation", this);
	grupa2 = new QGroupBox("Rotation", this);
	grupa3 = new QGroupBox("Scaling", this);
	grupa4 = new QGroupBox("Shearing", this);


	grupa1->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 100));
	grupa2->setGeometry(QRect(poczX + szer + poczX, poczY + 150, 800 - szer - 2 * poczX, poczY + 100));
	grupa3->setGeometry(QRect(poczX + szer + poczX, poczY + 300, 800 - szer - 2 * poczX, poczY + 100));
	grupa4->setGeometry(QRect(poczX + szer + poczX, poczY + 450, 800 - szer - 2 * poczX, poczY + 100));


	QVBoxLayout* boxLayout1 = new QVBoxLayout;
	QVBoxLayout* boxLayout2 = new QVBoxLayout;
	QVBoxLayout* boxLayout3 = new QVBoxLayout;
	QVBoxLayout* boxLayout4 = new QVBoxLayout;


	boxLayout1->addWidget(x_translationS);
	boxLayout1->addWidget(y_translationS);

	boxLayout2->addWidget(rotationS);

	boxLayout3->addWidget(x_scalingS);
	boxLayout3->addWidget(y_scalingS);

	boxLayout4->addWidget(x_shearingS);
	boxLayout4->addWidget(y_shearingS);


	grupa1->setLayout(boxLayout1);
	grupa2->setLayout(boxLayout2);
	grupa3->setLayout(boxLayout3);
	grupa4->setLayout(boxLayout4);

	connect(x_translationS, SIGNAL(valueChanged(int)), this, SLOT(x_translationBox(int)));
	connect(y_translationS, SIGNAL(valueChanged(int)), this, SLOT(y_translationBox(int)));

	connect(rotationS, SIGNAL(valueChanged(int)), this, SLOT(rotationSBox(int)));

	connect(x_scalingS, SIGNAL(valueChanged(int)), this, SLOT(x_scalingBox(int)));
	connect(y_scalingS, SIGNAL(valueChanged(int)), this, SLOT(y_scalingBox(int)));

	connect(x_shearingS, SIGNAL(valueChanged(int)), this, SLOT(x_shearingBox(int)));
	connect(y_shearingS, SIGNAL(valueChanged(int)), this, SLOT(y_shearingBox(int)));

	R[0][0] = cos(0);
	R[0][1] = -sin(0);
	R[1][0] = sin(0);
	R[1][1] = cos(0);

	neg_R[0][0] = cos(0);
	neg_R[0][1] = sin(0);
	neg_R[1][0] = -sin(0);
	neg_R[1][1] = cos(0);
	transform();
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

void App::setPixel(QImage* img, int x, int y, int Red, int Green, int Blue)
{
	if ((x >= 0) && (y >= 0) && (x < img->width()) && (y < img->height())) {
		unsigned char* ptr;
		ptr = img->bits();
		ptr[szer * 4 * y + 4 * x] = Blue;
		ptr[szer * 4 * y + 4 * x + 1] = Green;
		ptr[szer * 4 * y + 4 * x + 2] = Red;
	}
}

Rgb App::getPixel(QImage* image, int x, int y)
{
	Rgb temp;
	if ((x >= 0) && (y >= 0) && (x < image->width()) && (y < image->height())) {
		unsigned char* ptr;
		ptr = image->bits();
		temp.blue = ptr[image->width() * 4 * y + 4 * x];
		temp.green = ptr[image->width() * 4 * y + 4 * x + 1];
		temp.red = ptr[image->width() * 4 * y + 4 * x + 2];
	}
	else {
		temp.blue = -1;
		temp.green = -1;
		temp.red = -1;
	}

	return temp;
}

void App::multiplication(float tab1[3][3], float tab2[3][3])
{
	float tmp[3][3] = { {0,0,0},{0,0,0} ,{0,0,0} };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int inner = 0; inner < 3; inner++) {
				tmp[i][j] += tab1[i][inner] * tab2[inner][j];
			}
		}
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M[i][j] = tmp[i][j];
}

void App::calculate_pos_M()
{
	clearMatrix(M);
	multiplication(T0, T);
	multiplication(M, Shx);
	multiplication(M, Shy);
	multiplication(M, R);
	multiplication(M, S);
	multiplication(M, T0n);
}

void App::calculate_neg_M()
{
	float _T0[3][3] = { {1,0,-2 * x0},{0,1,-2 * y0},{0,0,1} };
	float _T0n[3][3] = { {1,0,x0},{0,1,y0},{0,0,1} };

	clearMatrix(M);

	multiplication(_T0n, neg_S);
	multiplication(M, neg_R);
	multiplication(M, neg_Shy);
	multiplication(M, neg_Shx);
	multiplication(M, _T0);
	multiplication(M, neg_T);
}

Rgb App::interpolation(float x, float y)
{
	int P4_x = floor(x);
	int P4_y = floor(y);

	int P3_x = P4_x + 1;
	int P3_y = P4_y;

	int P2_x = P4_x + 1;
	int P2_y = P4_y + 1;

	int P1_x = P4_x;
	int P1_y = P4_y + 1;

	float a = x - P4_x;
	float b = y - P4_y;

	Rgb P4_color = getPixel(img_org, P4_x, P4_y);
	Rgb P3_color = getPixel(img_org, P3_x, P3_y);
	Rgb P2_color = getPixel(img_org, P2_x, P2_y);
	Rgb P1_color = getPixel(img_org, P1_x, P1_y);

	Rgb result;
	result.red = b * ((1 - a) * P1_color.red + (a * P2_color.red)) + (1 - b) * ((1 - a) * P4_color.red + (a * P3_color.red));
	result.green = b * ((1 - a) * P1_color.green + (a * P2_color.green)) + (1 - b) * ((1 - a) * P4_color.green + (a * P3_color.green));
	result.blue = b * ((1 - a) * P1_color.blue + (a * P2_color.blue)) + (1 - b) * ((1 - a) * P4_color.blue + (a * P3_color.blue));

	return result;
}

void App::multiplicationPoints(float& x, float& y)
{
	float tmp[3] = { x,y,1 };
	float res[3] = { 0,0,0 };
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			res[i] += M[i][j] * tmp[j];
	x = res[0];
	y = res[1];
}

void App::clearMatrix(float tab[3][3])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			tab[i][j] = 0;

}

void App::x_translationBox(int value)
{
	T[0][2] = value;
	neg_T[0][2] = -value;
	transform();
	update();
}

void App::y_translationBox(int value)
{
	T[1][2] = value;
	neg_T[1][2] = -value;
	transform();
	update();
}

void App::rotationSBox(int value)
{
	double pi = 3.14159265359;
	double alpha = value * (pi / 180);
	R[0][0] = cos(alpha);
	R[0][1] = -sin(alpha);
	R[1][0] = sin(alpha);
	R[1][1] = cos(alpha);

	neg_R[0][0] = cos(alpha);
	neg_R[0][1] = sin(alpha);
	neg_R[1][0] = -sin(alpha);
	neg_R[1][1] = cos(alpha);
	transform();
	update();
}

void App::x_scalingBox(int value)
{
	S[0][0] = value / 100.0;
	neg_S[0][0] = 1.0 / S[0][0];

	transform();
	update();
}

void App::y_scalingBox(int value)
{
	S[1][1] = value / 100.0;
	neg_S[1][1] = 1.0 / S[1][1];

	transform();
	update();
}

void App::x_shearingBox(int value)
{
	Shx[0][1] = value / 100.0;
	neg_Shx[0][1] = -Shx[0][1];

	transform();
	update();
}

void App::y_shearingBox(int value)
{
	Shy[1][0] = value / 100.0;
	neg_Shy[1][0] = -Shy[1][0];

	transform();
	update();
}

void App::clearImg() {
	for (int i = 0; i < szer; i++)
		for (int j = 0; j < wys; j++)
			setPixel(this->img, i, j, 255, 255, 255);
}

void App::slot_czysc()
{
	update();
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

void App::transform()
{
	clearImg();
	Rgb color;
	calculate_pos_M();

	for (int i = 0; i < IMG_SIZE; i++)
		for (int j = 0; j < IMG_SIZE; j++) {
			color = getPixel(img_org, i, j);

			float x = i;
			float y = j;

			multiplicationPoints(x, y);
			setPixel(img, x, y, color.red, color.green, color.blue);

		}

	calculate_neg_M();
	for (int i = 0; i < szer; i++)
		for (int j = 0; j < wys; j++) {

			float x = i;
			float y = j;

			multiplicationPoints(x, y);
			if ((x >= 0) && (y >= 0) && (x < IMG_SIZE) && (y < IMG_SIZE)) {
				color = interpolation(x, y);
				//color = getPixel(img_org, x, y);
				setPixel(img, i, j, color.red, color.green, color.blue);
			}
		}

}