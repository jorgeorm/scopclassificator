#ifndef CLUSTERINGVIEW_H
#define CLUSTERINGVIEW_H

#include <QGroupBox>
#include <QThread>

#include <views/clusteringSpecificViews/algorithmoptions.h>

#include <models/dataset.h>
#include <models/featuredefinition.h>
#include <controllers/modeltrainingcontroller.h>

namespace Ui {
class ModelTrainingView;
}

class ModelTrainingView : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(Dataset* dataset READ dataset WRITE setDataset NOTIFY datasetChanged)
    Q_PROPERTY(FeatureDefinition* featureDef READ featureDef WRITE setFeatureDef NOTIFY featureDefChanged)


public:
    explicit ModelTrainingView(QWidget *parent = 0);
    ~ModelTrainingView();

    /**
     * @brief dataset Obtains the dataset loaded
     * @return
     */
    Dataset *dataset() const;

    /**
     * @brief setDataset Sets required dataset for the model training
     * @param dataset
     */
    void setDataset(Dataset *dataset);

    /**
     * @brief featureDef Obtains feature definition
     * @return
     */
    FeatureDefinition *featureDef() const;

    /**
     * @brief setFeatureDef loads feature definition
     * @param featureDef
     */
    void setFeatureDef(FeatureDefinition *featureDef);

signals:
    /**
     * @brief datasetChanged Notifies changes on dataset
     */
    void datasetChanged();
    /**
     * @brief featureDefChanged Notifies changes on feature definition
     */
    void featureDefChanged();

private slots:
    void on_clustTech_qcbx_activated(int index);

    /**
     * @brief onFeatureDefinitionChanged
     */
    void onFeatureDefinitionChanged();

    /**
     * @brief onDatasetChanged
     */
    void onDatasetChanged();

    /**
     * @brief onModelFound What should happen once the model has been found
     */
    void onModelFound();

    void on_localFeatures_qsb_valueChanged(int arg1);

    void on_procesesNumber_qsbx_valueChanged(int arg1);

    void on_generate_btn_clicked();

    void on_stop_btn_clicked();

    void on_save_btn_clicked();

    void on_globalFeatures_qsb_valueChanged(int arg1);

    void onModelSearchCancelled();

private:
    Ui::ModelTrainingView *ui;
    QList<AlgorithmOptions *> _algorithmsSpecifics;
    AlgorithmOptions * _currentOptions;
    ModelTrainingController _controller;
    QThread _controllerThread;
};

#endif // CLUSTERINGVIEW_H
