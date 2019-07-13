#ifndef FILELOGMODEL_H
#define FILELOGMODEL_H

#include <QAbstractTableModel>

class CFileLogModel : public QAbstractTableModel
{
public:
    enum Columns
    {
        Status,
        Filename,
        NumberOfElements
    };

    using vFiles = std::vector<std::pair<QString, QString>>;

    CFileLogModel(const vFiles& log, QObject* pParent = nullptr);

    void SetLog(const vFiles& log);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    vFiles m_log;
};

#endif // FILELOGMODEL_H
