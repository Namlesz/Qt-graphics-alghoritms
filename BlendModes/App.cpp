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

	img = new QImage(szer, wys, QImage::Format_RGB32);

	//zdjecia wrzucic do lokalizacji gdzie jest uruchamiany plik .exe lub podac sciezke bezposrednia
	layers[0].layerImg = new QImage(QCoreApplication::applicationDirPath() + "\\image_1.jpg");
	layers[1].layerImg = new QImage(QCoreApplication::applicationDirPath() + "\\image_2.jpg");
	layers[2].layerImg = new QImage(QCoreApplication::applicationDirPath() + "\\image_3.jpg");

	listWidget = new QListWidget(this);

	new QListWidgetItem(tr("Bridge"), listWidget);
	new QListWidgetItem(tr("Car"), listWidget);
	new QListWidgetItem(tr("Forest"), listWidget);

	listWidget->setCurrentRow(0);

	dropdown = new QComboBox();
	dropdown->addItem("Normal");
	dropdown->addItem("Multiply");
	dropdown->addItem("Screen");
	dropdown->addItem("Lighten");
	dropdown->addItem("Difference");

	layers[0].alpha = 1;
	layers[0].mode = 0;

	layers[1].alpha = 1;
	layers[1].mode = 0;

	layers[2].alpha = 1;
	layers[2].mode = 0;

	alphaSlider = new QSlider(Qt::Horizontal);
	alphaSlider->setRange(0, 100);
	alphaSlider->setValue(100);

	grupa = new QGroupBox("Sterowanie", this);

	grupa->setGeometry(QRect(poczX + szer + poczX, poczY, 800 - szer - 2 * poczX, poczY + 350));

	QVBoxLayout* boxLayout = new QVBoxLayout;


	boxLayout->addWidget(dropdown);
	boxLayout->addWidget(alphaSlider);
	boxLayout->addWidget(listWidget);
	grupa->setLayout(boxLayout);

	connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(itemChangedSlot(int)));
	connect(alphaSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderSlot(int)));
	connect(dropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(modeSelect(int)));


	clearImg(); //dzięki tej funkcji uzyskujemy mieszanie z warstwa podstawowa, tzn biala 
	drawBlend();
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

void App::drawBlend()
{
	clearImg();
	for (int i = ILOSC_WARSTW - 1; i >= 0; i--) {
		blend(layers[i]);
	}
	update();
}

void App::blend(Layer layers)
{
	unsigned char* foreground = layers.layerImg->bits();
	unsigned char* backgorund = img->bits();

	unsigned char c;
	for (int i = 0; i < szer * wys * 4; i++) {
		switch (layers.mode)
		{
		case 0:
			c = foreground[i];
			break;
		case 1:
			c = (foreground[i] * backgorund[i]) >> 8;
			break;
		case 2:
			c = 255 - ((255 - backgorund[i]) * (255 - foreground[i]) >> 8);
			break;
		case 3:
			if (backgorund[i] > foreground[i])
				c = backgorund[i];
			else
				c = foreground[i];
			break;
		case 4:
			c = std::abs(backgorund[i] - foreground[i]);
			break;
		default:
			break;
		}
		backgorund[i] = layers.alpha * c + (1 - layers.alpha) * backgorund[i];
	}
}

void App::clearImg() {
	for (int i = 0; i < szer; i++)
		for (int j = 0; j < wys; j++)
			setPixel(i, j, 255, 255, 255);
	update();
}

void App::itemChangedSlot(int val) {
	alphaSlider->setValue(layers[val].alpha * 100);
	dropdown->setCurrentIndex(layers[val].mode);
}

void App::modeSelect(int index)
{
	int id = listWidget->currentRow();
	layers[id].mode = index;
	drawBlend();
}

void App::sliderSlot(int val) {
	int index = listWidget->currentRow();
	layers[index].alpha = (float)val / 100;
	drawBlend();
}
