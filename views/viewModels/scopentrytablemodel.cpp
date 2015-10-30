#include "scopentrytablemodel.h"

SCOPEntryTableModel::SCOPEntryTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int SCOPEntryTableModel::rowCount(const QModelIndex &) const { return m_data.count(); }

int SCOPEntryTableModel::columnCount(const QModelIndex &) const { return 9; }

QVariant SCOPEntryTableModel::data(const QModelIndex &index, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    const SCOPEntry * entry= m_data[index.row()];
    QString humanFileSize = this->fileSizeHuman(entry->fileSize());
    switch (index.column()) {
    case 0: return entry->sid();
        break;
    case 1: return entry->sun();
        break;
    case 2: return entry->astralVersion();
        break;
    case 3: return entry->scss();
        break;
    case 4: return entry->sourcePdb();
        break;
    case 5: return entry->sourcePdbRevDat();
        break;
    case 6: return entry->region();
        break;
    case 7: return entry->sourceFile();
        break;
    case 8: return entry->numResidues();
        break;
    case 9: return humanFileSize;
        break;
    default:
        return QVariant();
    };
}

QVariant SCOPEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case 0: return "SCOP-sid:";
        break;
    case 1: return "SCOP-sun:";
        break;
    case 2: return "ASTRAL-version:";
        break;
    case 3: return "SCOP-sccs:";
        break;
    case 4: return "Source-PDB:";
        break;
    case 5: return "Source-PDB-REVDAT:";
        break;
    case 6: return "Region:";
        break;
    case 7: return "Archivo Origen:";
        break;
    case 8: return "# Residous:";
        break;
    case 9: return "Tamaño:";
        break;
    default:
        return QString::number(section);
    }
}

void SCOPEntryTableModel::setData(const QList<SCOPEntry *> &data){
    m_data = data;
}

void SCOPEntryTableModel::clearData(){
    qDeleteAll(m_data);
    m_data.clear();
}

QString SCOPEntryTableModel::fileSizeHuman(const unsigned int fileSize) const{
    float num = fileSize;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}
