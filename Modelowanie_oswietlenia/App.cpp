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
			setPixel(this->img, i, j, 0, 0, 0);
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

void App::drawSquare(Point2D points[8], bool visible[], double light[]) {
	//sciana gorna
	if (visible[0]) {
		fillWall(points[0], points[1], points[5], points[4], light[0]);
		drawLine(points[0].x, points[0].y, points[1].x, points[1].y, 0, 0, 0);
		drawLine(points[1].x, points[1].y, points[5].x, points[5].y, 0, 0, 0);
		drawLine(points[4].x, points[4].y, points[5].x, points[5].y, 0, 0, 0);
		drawLine(points[0].x, points[0].y, points[4].x, points[4].y, 0, 0, 0);
	}

	//prawa boczna
	if (visible[1]) {
		fillWall(points[1], points[5], points[6], points[2], light[1]);
		drawLine(points[1].x, points[1].y, points[2].x, points[2].y, 0, 0, 0);
		drawLine(points[6].x, points[6].y, points[2].x, points[2].y, 0, 0, 0);
		drawLine(points[5].x, points[5].y, points[6].x, points[6].y, 0, 0, 0);
		drawLine(points[5].x, points[5].y, points[1].x, points[1].y, 0, 0, 0);
	}

	//przednia
	if (visible[2]) {
		fillWall(points[0], points[1], points[2], points[3], light[2]);
		drawLine(points[0].x, points[0].y, points[1].x, points[1].y, 0, 0, 0);
		drawLine(points[1].x, points[1].y, points[2].x, points[2].y, 0, 0, 0);
		drawLine(points[3].x, points[3].y, points[2].x, points[2].y, 0, 0, 0);
		drawLine(points[0].x, points[0].y, points[3].x, points[3].y, 0, 0, 0);
	}

	//dolna
	if (visible[3]) {
		fillWall(points[3], points[2], points[6], points[7], light[3]);
		drawLine(points[3].x, points[3].y, points[2].x, points[2].y, 0, 0, 0);
		drawLine(points[2].x, points[2].y, points[6].x, points[6].y, 0, 0, 0);
		drawLine(points[7].x, points[7].y, points[6].x, points[6].y, 0, 0, 0);
		drawLine(points[7].x, points[7].y, points[3].x, points[3].y, 0, 0, 0);
	}

	//lewa
	if (visible[4]) {
		fillWall(points[3], points[0], points[4], points[7], light[4]);
		drawLine(points[3].x, points[3].y, points[0].x, points[0].y, 0, 0, 0);
		drawLine(points[0].x, points[0].y, points[4].x, points[4].y, 0, 0, 0);
		drawLine(points[7].x, points[7].y, points[4].x, points[4].y, 0, 0, 0);
		drawLine(points[7].x, points[7].y, points[3].x, points[3].y, 0, 0, 0);
	}

	//tylnia
	if (visible[5]) {
		fillWall(points[4], points[5], points[6], points[7], light[5]);
		drawLine(points[4].x, points[4].y, points[5].x, points[5].y, 0, 0, 0);
		drawLine(points[5].x, points[5].y, points[6].x, points[6].y, 0, 0, 0);
		drawLine(points[6].x, points[6].y, points[7].x, points[7].y, 0, 0, 0);
		drawLine(points[7].x, points[7].y, points[4].x, points[4].y, 0, 0, 0);
	}
}

void App::isVisible(bool visible[], Point3D squarePos[], double light[])
{
	Point3D a, b, vc, n;
	double res;
	int w[6][3] = { {0,1,4}, {2,6,1}, {3,2,0}, {7,6,3}, {7,3,4}, {6,7,5} };

	for (int i = 0; i < 6; i++) {
		a.x = squarePos[w[i][1]].x - squarePos[w[i][0]].x;
		a.y = squarePos[w[i][1]].y - squarePos[w[i][0]].y;
		a.z = squarePos[w[i][1]].z - squarePos[w[i][0]].z;

		b.x = squarePos[w[i][2]].x - squarePos[w[i][0]].x;
		b.y = squarePos[w[i][2]].y - squarePos[w[i][0]].y;
		b.z = squarePos[w[i][2]].z - squarePos[w[i][0]].z;

		vc.x = -squarePos[w[i][0]].x;
		vc.y = -squarePos[w[i][0]].y;
		vc.z = -squarePos[w[i][0]].z;

		n.x = a.y * b.z - a.z * b.y;
		n.y = a.z * b.x - a.x * b.z;
		n.z = a.x * b.y - a.y * b.x;

		res = n.x * vc.x + n.y * vc.y + n.z * vc.z;

		if (res > 0)
			visible[i] = true;
		else
			visible[i] = false;

		Point3D s;
		s.x = 0 - squarePos[w[i][0]].x;
		s.y = 0 - squarePos[w[i][0]].y;
		s.z = 0 - squarePos[w[i][0]].z;

		double _s, _n;

		_s = sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
		_n = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);

		s.x = s.x / _s;
		s.y = s.y / _s;
		s.z = s.z / _s;

		n.x = n.x / _n;
		n.y = n.y / _n;
		n.z = n.z / _n;

		light[i] = n.x * s.x + n.y * s.y + n.z * s.z;
		if (light[i] < 0.25)
			light[i] = 0.25;
	}
}

void App::fillWall(Point2D point_1, Point2D point_2, Point2D point_3, Point2D point_4, double light)
{
	int max = 0, min = 0, dl = 0;
	dl = 4;
	max = min = point_1.y;

	Point2D points[4] = { point_1,point_2 ,point_3 ,point_4 };

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

		if (pointSize > 1) {
			for (int j = 0; j < pointSize; j += 2) {
				drawLine(tmp[j] + 1, i, tmp[j + 1] - 1, i, light * 255, light * 0, light * 0);
			}
		}

		tmp.clear();
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
		points[i].x = (double(squarePos[i].x) / squarePos[i].z) * F + 300.0;
		points[i].y = (double(squarePos[i].y) / squarePos[i].z) * F + 300.0;
	}

	bool visible[6];
	double light[6];
	isVisible(visible, squarePos, light);
	drawSquare(points, visible, light);
	update();
}