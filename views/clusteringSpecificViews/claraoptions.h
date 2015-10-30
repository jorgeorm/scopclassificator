#ifndef CLARAOPTIONS_H
#define CLARAOPTIONS_H

#include <QFrame>
#include "algorithmoptions.h"

namespace Ui {
class ClaraOptions;
}

class ClaraOptions : public AlgorithmOptions
{
    Q_OBJECT

public:
    explicit ClaraOptions(QWidget *parent = 0);
    ~ClaraOptions();

    virtual QString optionsAsString();



    unsigned clusters() const;

    void setClusters(const unsigned &clusters);

    void setDatasetSize(const unsigned &datasetSize);

private:
    Ui::ClaraOptions *ui;
    unsigned _clusters, _datasetSize;
};

#endif // CLARAOPTIONS_H
