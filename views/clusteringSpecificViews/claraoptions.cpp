#include "claraoptions.h"
#include "ui_claraoptions.h"
#include <QStringBuilder>

ClaraOptions::ClaraOptions(QWidget *parent) :
    AlgorithmOptions(parent),
    ui(new Ui::ClaraOptions)
{
    ui->setupUi(this);
}



ClaraOptions::~ClaraOptions()
{
    delete ui;
}

QString ClaraOptions::optionsAsString(){

    QString metric;
    switch (ui->metric_combo->currentIndex()){
    case 0:
        metric = "euclidean";
        break;
    case 1:
        metric = "manhattan";
        break;
    }

    QString medoidsX;
    if(ui->medoids_check->isChecked()){
        medoidsX = "TRUE";
    } else {
        medoidsX = "FALSE";
    }

    QString keepData;
    if(ui->copyData_check->isChecked()){
        keepData = "TRUE";
    } else {
        keepData = "FALSE";
    }

    QString rRand;
    if(ui->rRandom_check->isChecked()){
        rRand = "TRUE";
    } else {
        rRand = "FALSE";
    }

    return "metric = \"" % metric %
            "\", stand = FALSE, samples = " % ui->samples_spin->text() %
            ",sampsize = " % ui->sampSize_spin->text() %
            ", trace = 0, medoids.x = " % medoidsX %
            ", keep.data = " % keepData %
            ", rngR = " % rRand;
}

unsigned ClaraOptions::clusters() const
{
    return _clusters;
}

void ClaraOptions::setClusters(const unsigned &clusters)
{
    _clusters = clusters;
}

void ClaraOptions::setDatasetSize(const unsigned &datasetSize)
{
    _datasetSize = datasetSize;
}
