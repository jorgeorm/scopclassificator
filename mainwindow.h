#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QMainWindow>
#include <QAbstractAnimation>

#include <models/dataset.h>
#include <models/featuredefinition.h>
#include <models/modelevaluation.h>
#include <models/predictivemodel.h>

#include <views/datasetview.h>
#include <views/featuredefinitionview.h>
#include <views/modelevaluationview.h>
#include <views/modeltrainingview.h>

#include <RInside.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(RInside &R, QWidget *parent = 0);
    ~MainWindow(); 

public slots:
    /**
     * @brief notifyEvent
     * @param event
     */
    void notifyEvent(QString event);

protected:
    /**
     * @brief resizeSidebar
     * @param width
     */
    void resizeSidebar(int _width);
    /**
     * @brief fadeVariableWidget
     * @param fade_in
     */
    void fadeInVariableWidget(bool fade_in = true);
private slots:
    /**
     * @brief on_clb_selectDataset_clicked
     */
    void on_clb_selectDataset_clicked();

    void onAnimationResizeFinish ();

    void onAnimationFadeInFinish ();

    void onDatasetObtained();

    void onDatasetDestroyed();

    void onFeatureConfigObtained();

    void on_clb_defineCharact_clicked();

    void on_clb_trainModel_clicked();

    void on_clb_testModel_clicked();

    void onNotification(QString notification);

private:
    // UI attributes
    Ui::MainWindow *ui;
    int animationDuration;
    bool _fadeDirection;

    // Datas
    RInside & _R;
    Dataset *_obtainedDataset;
    FeatureDefinition *_obtainedFeaturesConf;
    PredictiveModel *_obtainedClassificationModel;
    ModelEvaluation * _obtainedModelEvaluation;

    //Aditional Views
    DatasetView * _datasetView;
    FeatureDefinitionView * _featureView;
    ModelTrainingView * _modelTrainingView;
    ModelEvaluationView * _modelEvaluationView;

    /**
     * @brief replaceVariableWidget
     * @param _widget
     */
    void replaceVariableWidget(QWidget *_widget);
    /**
     * @brief cleanLayout
     * @param variableLayout
     */
    void cleanLayout(QLayout *variableLayout);
    /**
     * @brief initParams
     */
    void initParams();

    /**
     * @brief hideViews
     */
    void hideViews();
};

#endif // MAINWINDOW_H
