#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>
#define IMG_SIZE 300
#define PI 3.14159265359
#define SQR 100

App::App(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Przeksztalcenia 3D");
	resize(850, 650);

	szer = 600;
	wys = 600;
	poczX = poczY = 25;

	img = new QImage(szer, wys, QImage::Format_RGB32);

	translation_x = new QSlider(Qt::Horizontal);
	translation_y = new QSlider(Qt::Horizontal);
	translation_z = new QSlider(Qt::Horizontal);

	rotation_x = new QSlider(Qt::Horizontal);
	rotation_y = new QSlider(Qt::Horizontal);
	rotation_z = new QSlider(Qt::Horizontal);

	scaling_x = new QSlider(Qt::Horizontal);
	scaling_y = new QSlider(Qt::Horizontal);
	scaling_z = new QSlider(Qt::Horizontal);

	translation_x->setRange(-200, 200);
	translation_y->setRange(-200, 200);
	translation_z->setRange(-200, 200);

	rotation_x->setRange(0, 360);
	rotation_y->setRange(0, 360);
	rotation_z->setRange(0, 360);

	scaling_x->setRange(0, 200);
	scaling_y->setRange(0, 200);
	scaling_z->setRange(0, 200);

	scaling_x->setValue(100);
	scaling_y->setValue(100);
	scaling_z->setValue(100);

	translation_x->setValue(0);
	translation_y->setValue(0);
	translation_z->setValue(0);

	translation = new QGroupBox("Translacja", this);
	rotation = new QGroupBox("Rotacja", this);
	scaling = new QGroupBox("Skalowanie", this);


	translation->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 100));
	rotation->setGeometry(QRect(poczX + szer + poczX, poczY + 150, 800 - szer - 2 * poczX, poczY + 100));
	scaling->setGeometry(QRect(poczX + szer + poczX, poczY + 300, 800 - szer - 2 * poczX, poczY + 100));


	QVBoxLayout* boxLayout1 = new QVBoxLayout;
	QVBoxLayout* boxLayout2 = new QVBoxLayout;
	QVBoxLayout* boxLayout3 = new QVBoxLayout;
	QVBoxLayout* boxLayout4 = new QVBoxLayout;


	boxLayout1->addWidget(translation_x);
	boxLayout1->addWidget(translation_y);
	boxLayout1->addWidget(translation_z);

	boxLayout2->addWidget(rotation_x);
	boxLayout2->addWidget(rotation_y);
	boxLayout2->addWidget(rotation_z);

	boxLayout3->addWidget(scaling_x);
	boxLayout3->addWidget(scaling_y);
	boxLayout3->addWidget(scaling_z);

	translation->setLayout(boxLayout1);
	rotation->setLayout(boxLayout2);
	scaling->setLayout(boxLayout3);

	connect(translation_x, SIGNAL(valueChanged(int)), this, SLOT(trans_funct_x(int)));
	connect(translation_y, SIGNAL(valueChanged(int)), this, SLOT(trans_funct_y(int)));
	connect(translation_z, SIGNAL(valueChanged(int)), this, SLOT(trans_funct_z(int)));

	connect(rotation_x, SIGNAL(valueChanged(int)), this, SLOT(rotate_funct_x(int)));
	connect(rotation_y, SIGNAL(valueChanged(int)), this, SLOT(rotate_funct_y(int)));
	connect(rotation_z, SIGNAL(valueChanged(int)), this, SLOT(rotate_funct_z(int)));


	connect(scaling_x, SIGNAL(valueChanged(int)), this, SLOT(scal_funct_x(int)));
	connect(scaling_y, SIGNAL(valueChanged(int)), this, SLOT(scal_funct_y(int)));
	connect(scaling_z, SIGNAL(valueChanged(int)), this, SLOT(scal_funct_z(int)));

	square[0].x = 0;
	square[0].y = 0;
	square[0].z = 0;

	square[1].x = SQR;
	square[1].y = 0;
	square[1].z = 0;

	square[2].x = SQR;
	square[2].y = SQR;
	square[2].z = 0;

	square[3].x = 0;
	square[3].y = SQR;
	square[3].z = 0;

	square[4].x = 0;
	square[4].y = 0;
	square[4].z = SQR;

	square[5].x = SQR;
	square[5].y = 0;
	square[5].z = SQR;

	square[6].x = SQR;
	square[6].y = SQR;
	square[6].z = SQR;

	square[7].x = 0;
	square[7].y = SQR;
	square[7].z = SQR;

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

void App::clearImg() {
	for (int i = 0; i < szer; i++)
		for (int j = 0; j < wys; j++)
			setPixel(this->img, i, j, 255, 255, 255);
}

void App::sortPos(int& val_1, int& val_2)
{
	if (val_1 > val_2) {
		int tmp = val_1;
		val_1 = val_2;
		val_2 = tmp;
	}
}

void App::drawLine(int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue)
{
	if (end_x == beg_x) {
		sortPos(beg_y, end_y);
		for (int i = beg_y; i < end_y; i++) {
			setPixel(img, end_x, i, Red, Green, Blue);
		}
	}
	else {

		double a, b;
		a = (end_y - beg_y) / double(end_x - beg_x);
		b = beg_y - a * beg_x;

		if (abs(a) < 1) {

			sortPos(beg_x, end_x);
			int calcY = 0;

			for (int i = beg_x; i <= end_x; i++) {
				calcY = round(a * i + b);
				setPixel(img, i, calcY, Red, Green, Blue);
			}
		}
		else {

			sortPos(beg_y, end_y);
			int calcX = 0;

			for (int i = beg_y; i <= end_y; i++) {
				calcX = round(double(i - b) / double(a));
				setPixel(img, calcX, i, Red, Green, Blue);
			}
		}
	}
}

void App::multiplicationMatrix(double tab1[4][4], double tab2[4][4])
{
	double tmp[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int inner = 0; inner < 4; inner++) {
				tmp[i][j] += tab1[i][inner] * tab2[inner][j];
			}
		}
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i][j] = tmp[i][j];
}

void App::clearMatrix(double tab[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tab[i][j] = 0;
}

void App::multiplicationPoints(Point3D& p)
{
	double tmp[4] = { p.x,p.y,p.z,1 };
	double res[4] = { 0,0,0,0 };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			res[i] += M[i][j] * tmp[j];
	p.x = res[0];
	p.y = res[1];
	p.z = res[2];
}

void App::trans_funct_x(int value)
{
	T[0][3] = value;
	transform();
	update();
}

void App::trans_funct_y(int value)
{
	T[1][3] = value;
	transform();
	update();
}

void App::trans_funct_z(int value)
{
	T[2][3] = value;
	transform();
	update();
}

void App::scal_funct_x(int value)
{
	S[0][0] = value / 100.0;
	transform();
	update();
}

void App::scal_funct_y(int value)
{
	S[1][1] = value / 100.0;
	transform();
	update();
}

void App::scal_funct_z(int value)
{
	S[2][2] = value / 100.0;
	transform();
	update();
}

void App::rotate_funct_x(int value)
{
	double alpha = value * (PI / 180.0);
	Rx[1][1] = cos(alpha);
	Rx[1][2] = -sin(alpha);
	Rx[2][1] = sin(alpha);
	Rx[2][2] = cos(alpha);

	transform();
	update();
}

void App::rotate_funct_y(int value)
{
	double alpha = value * (PI / 180.0);
	Ry[0][0] = cos(alpha);
	Ry[0][2] = sin(alpha);
	Ry[2][0] = -sin(alpha);
	Ry[2][2] = cos(alpha);

	transform();
	update();
}

void App::rotate_funct_z(int value)
{
	double alpha = value * (PI / 180.0);
	Ry[0][0] = cos(alpha);
	Ry[0][1] = -sin(alpha);
	Ry[1][0] = sin(alpha);
	Ry[1][1] = cos(alpha);

	transform();
	update();
}

void App::calc_M_matrix()
{
	clearMatrix(M);
	multiplicationMatrix(To, T);
	multiplicationMatrix(M, Rx);
	multiplicationMatrix(M, Ry);
	multiplicationMatrix(M, Rz);
	multiplicationMatrix(M, S);
	multiplicationMatrix(M, ToN);

}

void App::drawSquare(Point2D points[8]) {
	for (int i = 0; i < 8; i++)
		for (int j = 1; j < 8; j++) {
			if ((i == 0 && j == 6) || (i == 1 && j == 7) || (i == 2 && j == 4) || (i == 3 && j == 5)
				|| (j == 0 && i == 6) || (j == 1 && i == 7) || (j == 2 && i == 4) || (j == 3 && i == 5)) //pominiecie wewnetrznych krawedzi
				continue;
			else if (points[j].x == -1 || points[i].x == -1) //przekroczenie widoku 
				continue;
			else
				drawLine(points[i].x, points[i].y, points[j].x, points[j].y, 0, 0, 0);
		}
}

void App::transform()
{
	clearImg();
	calc_M_matrix();
	Point3D squarePos[8];
	Point2D points[8];
	int F = 300;
	for (int i = 0; i < 8; i++) {
		squarePos[i].x = square[i].x;
		squarePos[i].y = square[i].y;
		squarePos[i].z = square[i].z;
	}

	for (int i = 0; i < 8; i++) {
		multiplicationPoints(squarePos[i]);
		std::cout << "[" << i << "] = " << squarePos[i].z << "\n";
		if (squarePos[i].z < 50) {
			points[i].x = -1;
			points[i].y = -1;
		}
		else {
			points[i].x = (double(squarePos[i].x) / squarePos[i].z) * F + 300.0;
			points[i].y = (double(squarePos[i].y) / squarePos[i].z) * F + 300.0;
		}

	}
	std::cout << std::endl;
	drawSquare(points);
	update();
}