#ifndef DATASETVIEW_H
#define DATASETVIEW_H

#include <QGroupBox>
#include <QThread>

#include <controllers/datasetcontroller.h>

#include <views/viewModels/scopentrytablemodel.h>

namespace Ui {
class DatasetView;
}

class DatasetView : public QGroupBox
{

    Q_OBJECT
//    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int animationTime READ animationTime WRITE setAnimationTime NOTIFY animationTimeChanged)
    Q_PROPERTY(QString defaultSearchPath READ defaultSearchPath WRITE setDefaultSearchPath NOTIFY defaultSearchPathChanged)
public:
    explicit DatasetView(QWidget *parent = 0);
    ~DatasetView();

//    double opacity() const;
//    void setOpacity(double opacity);

    int animationTime() const;
    void setAnimationTime(int animationTime);

    QString defaultSearchPath() const;
    void setDefaultSearchPath(const QString &defaultSearchPath);

    Dataset * dataset() const;
    void setDataset(Dataset *dataset_p);

private:
    Ui::DatasetView *ui;
    //    double _opacity;
    int _animationTime;
    QString _defaultSearchPath;
    DatasetController _controller;
    Dataset * _dataset_p;
    QList <SCOPEntry*> _models;
    SCOPEntryTableModel * _datasetVisualRep;
    QThread _threadController;

    void animatePathIdCheck(int _state);
    void configControllerByUi();

//    void validateDatasetForm();

    /**
     * @brief displayDatasetInformation
     */
    void displayDatasetInformation();
    void removeDataFromTableView();

signals:
//    void opacityChanged();
    void animationTimeChanged();
    void defaultSearchPathChanged();
    void stepCompleted();
    void datasetDestroyed();
    void eventNotified(QString eventName);


private slots:
    void datasetGenerated(Dataset * dataset);
    void viewableDatasetlGenerated(QList<SCOPEntry*> visualRep);
    void on_qcb1_pathId_stateChanged(int _state);
    void onAnimationPathIdFinished();
    void on_qtb1_pathId_clicked();
    void on_qtb2_pathFolder_clicked();
    void on_qpb1_loadDataset_clicked();
    /**
     * @brief taskNotified
     * @param taskName
     */
    void taskNotified(QString taskName);
    /**
     * @brief progressNotified
     * @param progress
     */
    void progressNotified(int progress);


    void on_qpb3_resetDataset_clicked();
    void on_qpb4_finishSelection_clicked();
};

#endif // DATASETVIEW_H
