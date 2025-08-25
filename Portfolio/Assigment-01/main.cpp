// Mandatory-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>
#include "TMainForm.h"

int main(int argc, char* argv[])
{
    // Create the Qt Application instance
    QApplication app(argc, argv);

    // Force the application to use the light Fusion style
    app.setStyle(QStyleFactory::create("Fusion"));
    // 2. Create and apply a standard light color palette
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(0, 0, 255));
    lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(lightPalette);

    // Create the main window object
	TMainForm window(nullptr);



    // Show the window
    window.show();

    // Start the application's event loop
    return app.exec();
}
