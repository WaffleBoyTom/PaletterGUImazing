#include "paletterwindow.h"
#include "./ui_paletterwindow.h"

PaletterWindow::PaletterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PaletterWindow)
{
    ui->setupUi(this);
}

PaletterWindow::~PaletterWindow()
{
    delete ui;
}
