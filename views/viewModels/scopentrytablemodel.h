#ifndef SCOPENTRYTABLEMODEL_H
#define SCOPENTRYTABLEMODEL_H

#include <QAbstractTableModel>

#include <models/scopentry.h>



class SCOPEntryTableModel : public QAbstractTableModel
{
    QList<SCOPEntry *> m_data;
public:
    SCOPEntryTableModel(QObject * parent = 0);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;

    void setData(const QList<SCOPEntry *> &data);

    void clearData();

protected:
    /**
     * @brief fileSizeHuman
     * @param fileSize
     * @return
     */
    QString fileSizeHuman(const unsigned int fileSize) const;
};

#endif // SCOPENTRYTABLEMODEL_H
