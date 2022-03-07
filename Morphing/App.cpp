#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>

#define TEXTURE_SIZE 500
#define max_N 50

App::App(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Morphing");

	resize(1600, 650);

	szer = 500;
	wys = 500;
	poczX_Texture0 = poczY = 25;
	poczX_Texture1 = 2 * poczX_Texture0 + TEXTURE_SIZE;
	poczX_Draw = 3 * poczX_Texture0 + 2 * TEXTURE_SIZE;

	img_texture1 = new QImage("../Circle_and_eclipse/img2.jpg");
	img_texture0 = new QImage("../Circle_and_eclipse/img.jpg");
	img_texture1_COPY = new QImage("../Circle_and_eclipse/img2.jpg");
	img_texture0_COPY = new QImage("../Circle_and_eclipse/img.jpg");
	img_draw = new QImage(szer, wys, QImage::Format_RGB32);

	morphSlider = new QSlider(Qt::Horizontal);
	morphSlider->setRange(0, max_N);

	grupa = new QGroupBox("Morphing", this);
	grupa->setGeometry(QRect(poczX_Texture1 + 140, 2 * poczY + wys, 200, 75));

	QVBoxLayout* boxLayout = new QVBoxLayout;
	boxLayout->addWidget(morphSlider);

	grupa->setLayout(boxLayout);

	N = 0;

	connect(morphSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderFunc(int)));

	clearDraw();
	update();
}

App::~App()
{
	delete img_texture1;
	delete img_texture0;
	delete img_draw;
}

void App::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.drawImage(poczX_Texture1, poczY, *img_texture1);
	p.drawImage(poczX_Texture0, poczY, *img_texture0);
	p.drawImage(poczX_Draw, poczY, *img_draw);
}

void App::sliderFunc(int value)
{
	N = value;
	if (point_t1.size() == 3 && point_t0.size() == 3) {
		clearDraw();
		morphing();
	}
	update();
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

int App::pointInArea(int x, int y, std::vector<Point> points)
{
	for (int i = 0; i < points.size(); i++) {

		double dl = sqrt(pow(x - points[i].x, 2) + pow(y - points[i].y, 2));
		if (dl < 30)
		{
			return i;
		}
	}

	return -1;
}

void App::clearDraw()
{
	for (int i = 0; i < TEXTURE_SIZE; i++)
		for (int j = 0; j < TEXTURE_SIZE; j++)
			setPixel(img_draw, i, j, 200, 200, 200);
}

void App::copyImg(QImage* dest, QImage* src)
{
	unsigned char* ptr_dest, * ptr_src;

	ptr_dest = dest->bits();
	ptr_src = src->bits();
	int i, j;

	for (i = 0; i < wys; i++)
		for (j = 0; j < szer; j++)
		{
			ptr_dest[szer * 4 * i + 4 * j] = ptr_src[szer * 4 * i + 4 * j];
			ptr_dest[szer * 4 * i + 4 * j + 1] = ptr_src[szer * 4 * i + 4 * j + 1];
			ptr_dest[szer * 4 * i + 4 * j + 2] = ptr_src[szer * 4 * i + 4 * j + 2];
		}
}

void App::drawPoint(QImage* img, int x, int y, int Red, int Green, int Blue)
{
	int rozmiar = 3;
	for (int i = x - rozmiar; i < x + rozmiar; i++)
		for (int j = y - rozmiar; j < y + rozmiar; j++)
			setPixel(img, i, j, Red, Green, Blue);

}

void App::sortPos(int& val_1, int& val_2)
{
	if (val_1 > val_2) {
		int tmp = val_1;
		val_1 = val_2;
		val_2 = tmp;
	}
}

void App::drawPoints()
{
	for (int i = 0; i < point_t0.size(); i++)
		drawPoint(img_texture0, point_t0[i].x, point_t0[i].y, 255 - i * 255, i * 90, i * 150);
	for (int i = 0; i < point_t1.size(); i++)
		drawPoint(img_texture1, point_t1[i].x, point_t1[i].y, 255 - i * 255, i * 90, i * 150);
}

void App::drawLine(QImage* img, int beg_x, int beg_y, int end_x, int end_y, int Red, int Green, int Blue)
{
	// w przypadku linii pionowej
	if (end_x == beg_x) {
		//sortujemy 2 wartosci
		sortPos(beg_y, end_y);
		//ustawiamy kolor pikseli
		for (int i = beg_y; i < end_y; i++) {
			setPixel(img, end_x, i, Red, Green, Blue);
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
				setPixel(img, i, calcY, Red, Green, Blue);
			}
		}

		//jesli |a| > 1 to liczymy wartosc x dla kolejnych y 
		else {

			sortPos(beg_y, end_y); //sortowanie 
			int calcX = 0;

			for (int i = beg_y; i <= end_y; i++) {
				calcX = round(double(i - b) / double(a));
				setPixel(img, calcX, i, Red, Green, Blue);
			}
		}
	}
}

void App::drawLines()
{
	if (point_t1.size() == 3) {
		drawLine(img_texture1, point_t1[0].x, point_t1[0].y, point_t1[1].x, point_t1[1].y, 0, 0, 0);
		drawLine(img_texture1, point_t1[1].x, point_t1[1].y, point_t1[2].x, point_t1[2].y, 0, 0, 0);
		drawLine(img_texture1, point_t1[0].x, point_t1[0].y, point_t1[2].x, point_t1[2].y, 0, 0, 0);
	}
	if (point_t0.size() == 3) {
		drawLine(img_texture0, point_t0[0].x, point_t0[0].y, point_t0[1].x, point_t0[1].y, 0, 0, 0);
		drawLine(img_texture0, point_t0[1].x, point_t0[1].y, point_t0[2].x, point_t0[2].y, 0, 0, 0);
		drawLine(img_texture0, point_t0[0].x, point_t0[0].y, point_t0[2].x, point_t0[2].y, 0, 0, 0);
	}
}

bool App::checkPoints(std::vector<Point> points, int p1, int p2, int k)
{
	return (points[p1].y <= k && points[p2].y > k) || ((points[p2].y <= k && points[p1].y > k));
}

int App::calcX(std::vector<Point> points, int p1, int p2, int k)
{
	return points[p1].x + (k - points[p1].y) * (points[p2].x - points[p1].x) / (points[p2].y - points[p1].y);
}

void App::morphing()
{
	// trojkat P,Q,R
	Point P, Q, R, _P, _Q, _R, A, B, C;

	P.x = point_t1[0].x;
	P.y = point_t1[0].y;

	Q.x = point_t1[1].x;
	Q.y = point_t1[1].y;

	R.x = point_t1[2].x;
	R.y = point_t1[2].y;

	// trojkat P',Q',R'
	_P.x = point_t0[0].x;
	_P.y = point_t0[0].y;

	_Q.x = point_t0[1].x;
	_Q.y = point_t0[1].y;

	_R.x = point_t0[2].x;
	_R.y = point_t0[2].y;

	// wyliczone przejscie
	A.x = (1.0 - (double)N / max_N) * _P.x + ((double)N / max_N) * P.x;
	A.y = (1.0 - (double)N / max_N) * _P.y + ((double)N / max_N) * P.y;

	B.x = (1.0 - (double)N / max_N) * _Q.x + ((double)N / max_N) * Q.x;
	B.y = (1.0 - (double)N / max_N) * _Q.y + ((double)N / max_N) * Q.y;

	C.x = (1.0 - (double)N / max_N) * _R.x + ((double)N / max_N) * R.x;
	C.y = (1.0 - (double)N / max_N) * _R.y + ((double)N / max_N) * R.y;

	point_draw.clear();
	point_draw.push_back(A);
	point_draw.push_back(B);
	point_draw.push_back(C);

	int W = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);

	// ALGORYTM SCAN LINE
	int max = 0, min = 0, dl = 0;
	dl = point_draw.size();
	max = min = point_draw[0].y;

	for (int i = 0; i < dl; i++) {
		if (point_draw[i].y < min)
			min = point_draw[i].y;
		if (point_draw[i].y > max)
			max = point_draw[i].y;
	}

	std::vector<int> tmp;
	int x;
	for (int i = min; i <= max; i++) {

		for (int j = 0; j < dl - 1; j++)
			if (checkPoints(point_draw, j, j + 1, i)) {
				x = calcX(point_draw, j, j + 1, i);
				tmp.push_back(x);
			}

		if (checkPoints(point_draw, 0, dl - 1, i)) {
			x = calcX(point_draw, 0, dl - 1, i);
			tmp.push_back(x);
		}

		std::sort(tmp.begin(), tmp.end());
		int pointSize = tmp.size();

		if (pointSize > 1) {
			for (int j = 0; j < pointSize; j += 2) //iteracja po znalezionych punktach
				for (int c = tmp[j]; c < tmp[j + 1]; c++) { // c -> x; i -> y 

					int Wv = (c - A.x) * (C.y - A.y) - (i - A.y) * (C.x - A.x);
					int Ww = (B.x - A.x) * (i - A.y) - (B.y - A.y) * (c - A.x);

					double v = (double)Wv / W;
					double w = (double)Ww / W;
					double u = 1 - v - w;

					double x_t1 = u * P.x + v * Q.x + w * R.x;
					double y_t1 = u * P.y + v * Q.y + w * R.y;

					double x_t0 = u * _P.x + v * _Q.x + w * _R.x;
					double y_t0 = u * _P.y + v * _Q.y + w * _R.y;

					Rgb color0 = getPixel(img_texture1_COPY, x_t1, y_t1);
					Rgb color1 = getPixel(img_texture0_COPY, x_t0, y_t0);
					Rgb result;
					
					result.red = ((double)N / max_N) * color0.red + (1.0 - ((double)N / max_N)) * color1.red;
					result.green = ((double)N / max_N) * color0.green + (1.0 - ((double)N / max_N)) * color1.green;
					result.blue = ((double)N / max_N) * color0.blue + (1.0 - ((double)N / max_N)) * color1.blue;

					setPixel(img_draw, c, i, result.red, result.green, result.blue);
				}
		}
		tmp.clear();
	}
}

void App::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	y = y - poczY;

	int x_t0 = x - poczX_Texture0;
	int x_t1 = x - poczX_Texture1;

	copyImg(img_texture1, img_texture1_COPY);
	copyImg(img_texture0, img_texture0_COPY);

	if (Qt::LeftButton == event->button()) {
		bool inImgArea = false;
		std::vector<Point>* points;

		if (x_t1 > 0 && x_t1 < TEXTURE_SIZE && y < wys) {
			x = x_t1;
			inImgArea = true;
			points = &point_t1;
		}
		else if (x_t0 < TEXTURE_SIZE && x_t0 > 0 && y < wys) {
			x = x_t0;
			inImgArea = true;
			points = &point_t0;
		}
		if (inImgArea) {
			id = pointInArea(x, y, *points);

			if (id == -1 && points->size() < 3)
				points->push_back(Point{ x, y });
			else if (id >= 0) {
				points->at(id).x = x;
				points->at(id).y = y;
			}
		}

	}

	if (point_t1.size() == 3 && point_t0.size() == 3) {
		clearDraw();
		morphing();
	}

	drawPoints();
	drawLines();
	update();
}

void App::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	y = y - poczY;

	int x_t0 = x - poczX_Texture0;
	int x_t1 = x - poczX_Texture1;

	copyImg(img_texture1, img_texture1_COPY);
	copyImg(img_texture0, img_texture0_COPY);

	if (event->buttons() == Qt::LeftButton) {
		id = pointInArea(x_t1, y, point_t1);
		if (id >= 0) {
			point_t1.at(id).x = x_t1;
			point_t1.at(id).y = y;
		}

		id = pointInArea(x_t0, y, point_t0);
		if (id >= 0) {
			point_t0.at(id).x = x_t0;
			point_t0.at(id).y = y;
		}
	}

	if (point_t1.size() == 3 && point_t0.size() == 3) {
		clearDraw();
		morphing();
	}

	drawPoints();
	drawLines();
	update();
}
