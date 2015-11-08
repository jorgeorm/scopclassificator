#include "kmeansoptions.h"
#include "ui_kmeansoptions.h"
#include <QStringBuilder>

KMeansOptions::KMeansOptions(QWidget *parent) :
    AlgorithmOptions(parent),
    ui(new Ui::KMeansOptions)
{
    ui->setupUi(this);
}

KMeansOptions::~KMeansOptions()
{
    delete ui;
}

QString KMeansOptions::optionsAsString(){

}
