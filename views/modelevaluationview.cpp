#include "modelevaluationview.h"
#include "ui_modelevaluationview.h"

ModelEvaluationView::ModelEvaluationView(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ModelEvaluationView) {

    ui->setupUi(this);
    this->setVisible(false);
}

ModelEvaluationView::~ModelEvaluationView()
{
    delete ui;
}
