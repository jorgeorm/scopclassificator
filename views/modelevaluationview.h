#ifndef MODELEVALUATIONVIEW_H
#define MODELEVALUATIONVIEW_H

#include <QGroupBox>

namespace Ui {
class ModelEvaluationView;
}

class ModelEvaluationView : public QGroupBox
{
    Q_OBJECT

public:
    explicit ModelEvaluationView(QWidget *parent = 0);
    ~ModelEvaluationView();

private:
    Ui::ModelEvaluationView *ui;
};

#endif // MODELEVALUATIONVIEW_H
