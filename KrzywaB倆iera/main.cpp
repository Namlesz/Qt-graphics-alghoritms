#include <QApplication>
#include "App.h"
//naglowki potrzebne do dolaczenia konsoli do programu
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    //kod linkujacy konsole do naszego programu (przydatne przy debugowaniu np. uzywaniu std::cout)
    FreeConsole();
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    freopen("CON", "r", stdin);

    QApplication a(argc, argv);
    App w;
    w.show();
    return a.exec();
}