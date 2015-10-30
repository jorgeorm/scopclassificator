#ifndef FEATUREDEFINITIONVIEW_H
#define FEATUREDEFINITIONVIEW_H

#include <QGroupBox>
#include <QSvgWidget>
#include <QThread>


#include <RInside.h>
#include <models/dataset.h>
#include <models/featuredefinition.h>

#include <controllers/featurecontroller.h>

namespace Ui {
class FeatureDefinitionView;
}

class FeatureDefinitionView : public QGroupBox
{
    Q_OBJECT

public:
    explicit FeatureDefinitionView(QWidget *parent = 0);
    ~FeatureDefinitionView();

    void setRInstance( RInside *RInstance);

    FeatureDefinition *featuresDefinition() const;
    void setFeaturesDefinition(FeatureDefinition *features);

    void syncFeatureDefinition();

    void setDataset(const Dataset *dataset);

signals:
    void stepCompleted();
    void notify(QString);

private slots:
    void on_showFeat_qpb_clicked();
    void on_finish_qpb_clicked();

    void on_featureSize_qsb_valueChanged(int newVal);

    void paintFeatureGraphics();
    void taskNotified(QString task);
    void displayProgress(unsigned int progress);

    void on_featMethod_qcb_activated(const QString &arg1);

private:
    Ui::FeatureDefinitionView *ui;
    const RInside * _RInstance;
    const Dataset * _dataset;
    FeatureDefinition * _featureDef;
    FeatureController _controller;
    QThread _threadController;
    FeatureDefinition::GeneratorMethod selectedMethod();
    QSvgWidget *_svg;
};

#endif // FEATUREDEFINITIONVIEW_H
