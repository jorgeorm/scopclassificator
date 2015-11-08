#ifndef KMEANSOPTIONS_H
#define KMEANSOPTIONS_H

#include <QFrame>
#include "algorithmoptions.h"

namespace Ui {
class KMeansOptions;
}

class KMeansOptions : public AlgorithmOptions {
    Q_OBJECT

public:
    explicit KMeansOptions(QWidget *parent = 0);
    ~KMeansOptions();

    virtual QString optionsAsString();


private:
    Ui::KMeansOptions *ui;
};

#endif // KMEANSOPTIONS_H
