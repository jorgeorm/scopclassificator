#ifndef HCLUSTFORM_H
#define HCLUSTFORM_H

#include <QWidget>

namespace Ui {
class HClustForm;
}

class HClustForm : public QWidget
{
    Q_OBJECT

public:
    explicit HClustForm(QWidget *parent = 0);
    ~HClustForm();

private:
    Ui::HClustForm *ui;
};

#endif // HCLUSTFORM_H
