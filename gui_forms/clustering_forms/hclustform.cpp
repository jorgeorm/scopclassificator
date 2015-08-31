#include "hclustform.h"
#include "ui_hclustform.h"

HClustForm::HClustForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HClustForm)
{
    ui->setupUi(this);
}

HClustForm::~HClustForm()
{
    delete ui;
}
