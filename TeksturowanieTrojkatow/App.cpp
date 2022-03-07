#include "App.h"
#include <QVBoxLayout>
#include <math.h>
#include <iostream>

#define TEXTURE_SIZE 600

App::App(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("Teksturowanie trojkatow");

	resize(1275, 650);

	szer = 600;
	wys = 600;
	poczX_Texture = poczY = 25;
	poczX_Draw = 2 * poczX_Texture + TEXTURE_SIZE;
	img_draw = new QImage(szer, wys, QImage::Format_RGB32);
	img_texture = new QImage("../Circle_and_eclipse/img.jpg");
	img_draw_COPY = new QImage(szer, wys, QImage::Format_RGB32);
	img_texture_COPY = new QImage("../Circle_and_eclipse/img.jpg");
}

App::~App()
{
	delete img_draw;
	delete img_texture;
}

void App::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.drawImage(poczX_Draw, poczY, *img_draw);
	p.drawImage(poczX_Texture, poczY, *img_texture);
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

Rgb App::interpolation(double x, double y)
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

	Rgb P4_color = getPixel(img_texture, P4_x, P4_y);
	Rgb P3_color = getPixel(img_texture, P3_x, P3_y);
	Rgb P2_color = getPixel(img_texture, P2_x, P2_y);
	Rgb P1_color = getPixel(img_texture, P1_x, P1_y);

	Rgb result;
	result.red = b * ((1 - a) * P1_color.red + (a * P2_color.red)) + (1 - b) * ((1 - a) * P4_color.red + (a * P3_color.red));
	result.green = b * ((1 - a) * P1_color.green + (a * P2_color.green)) + (1 - b) * ((1 - a) * P4_color.green + (a * P3_color.green));
	result.blue = b * ((1 - a) * P1_color.blue + (a * P2_color.blue)) + (1 - b) * ((1 - a) * P4_color.blue + (a * P3_color.blue));

	return result;
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
	for (int i = 0; i < point_t.size(); i++)
		drawPoint(img_texture, point_t[i].x, point_t[i].y, 255 - i * 255, i * 90, i * 150);
	for (int i = 0; i < point_d.size(); i++)
		drawPoint(img_draw, point_d[i].x, point_d[i].y, 255 - i * 255, i * 90, i * 150);
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
	if (point_d.size() == 3) {
		drawLine(img_draw, point_d[0].x, point_d[0].y, point_d[1].x, point_d[1].y, 0, 0, 0);
		drawLine(img_draw, point_d[1].x, point_d[1].y, point_d[2].x, point_d[2].y, 0, 0, 0);
		drawLine(img_draw, point_d[0].x, point_d[0].y, point_d[2].x, point_d[2].y, 0, 0, 0);
	}
	if (point_t.size() == 3) {
		drawLine(img_texture, point_t[0].x, point_t[0].y, point_t[1].x, point_t[1].y, 0, 0, 0);
		drawLine(img_texture, point_t[1].x, point_t[1].y, point_t[2].x, point_t[2].y, 0, 0, 0);
		drawLine(img_texture, point_t[0].x, point_t[0].y, point_t[2].x, point_t[2].y, 0, 0, 0);
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

void App::triangleTexture()
{
	int Xa = point_d[0].x;
	int Xb = point_d[1].x;
	int Xc = point_d[2].x;
	int Ya = point_d[0].y;
	int Yb = point_d[1].y;
	int Yc = point_d[2].y;
	int W = (Xb - Xa) * (Yc - Ya) - (Yb - Ya) * (Xc - Xa);

	// ALGORYTM SCAN LINE
	int max = 0, min = 0, dl = 0;
	dl = point_d.size();
	max = min = point_d[0].y;

	for (int i = 0; i < dl; i++) {
		if (point_d[i].y < min)
			min = point_d[i].y;
		if (point_d[i].y > max)
			max = point_d[i].y;
	}

	std::vector<int> tmp;
	int x;
	for (int i = min; i <= max; i++) {

		for (int j = 0; j < dl - 1; j++) {

			if (checkPoints(point_d, j, j + 1, i)) {
				x = calcX(point_d, j, j + 1, i);
				tmp.push_back(x);
			}
		}

		if (checkPoints(point_d, 0, dl - 1, i)) {
			x = calcX(point_d, 0, dl - 1, i);
			tmp.push_back(x);
		}

		std::sort(tmp.begin(), tmp.end());
		int pointSize = tmp.size();

		if (pointSize > 1) {
			for (int j = 0; j < pointSize; j += 2) { //iteracja po znalezionych punktach
				for (int c = tmp[j]; c < tmp[j + 1]; c++) { // c -> x; i -> y 
					int Wv = (c - Xa) * (Yc - Ya) - (i - Ya) * (Xc - Xa);
					int Ww = (Xb - Xa) * (i - Ya) - (Yb - Ya) * (c - Xa);
					double v = (double)Wv / W;
					double w = (double)Ww / W;
					double u = 1 - v - w;
					double x_t = u * point_t[0].x + v * point_t[1].x + w * point_t[2].x;
					double y_t = u * point_t[0].y + v * point_t[1].y + w * point_t[2].y;
					Rgb color = interpolation(x_t, y_t);
					setPixel(img_draw, c, i, color.red, color.green, color.blue);
				}
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

	int x_t = x - poczX_Texture;
	int x_d = x - poczX_Draw;
	copyImg(img_draw, img_draw_COPY);
	copyImg(img_texture, img_texture_COPY);

	if (Qt::LeftButton == event->button()) {
		bool inImgArea = false;
		std::vector<Point>* points;

		if (x_d > 0 && x_d < TEXTURE_SIZE) {
			x = x_d;
			inImgArea = true;
			points = &point_d;
		}
		else if (x_t < TEXTURE_SIZE && x_t > 0) {
			x = x_t;
			inImgArea = true;
			points = &point_t;
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

	if (point_d.size() == 3 && point_t.size() == 3)
		triangleTexture();
	drawPoints();
	drawLines();
	update();

}

void App::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	y = y - poczY;

	int x_t = x - poczX_Texture;
	int x_d = x - poczX_Draw;

	copyImg(img_draw, img_draw_COPY);
	copyImg(img_texture, img_texture_COPY);

	if (event->buttons() == Qt::LeftButton) {
		id = pointInArea(x_d, y, point_d);
		if (id >= 0) {
			point_d.at(id).x = x_d;
			point_d.at(id).y = y;
		}

		id = pointInArea(x_t, y, point_t);
		if (id >= 0) {
			point_t.at(id).x = x_t;
			point_t.at(id).y = y;
		}
	}
	if (point_d.size() == 3 && point_t.size() == 3)
		triangleTexture();

	drawPoints();
	drawLines();
	update();
}
