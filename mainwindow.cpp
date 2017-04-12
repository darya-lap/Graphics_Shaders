#include "mainwindow.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
        setFixedSize(500,500);
        glWidget = new GLWidget(this);
        this->setCentralWidget(glWidget);
}

MainWindow::~MainWindow()
{

}
