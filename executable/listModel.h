#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>

class ListModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit ListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    enum
    {
        EntryIdRole = Qt::UserRole,
        NameRole
    };
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void appendEntry(const QString &name);
    Q_INVOKABLE void deleteEntry(const unsigned int &idToDelete);
    Q_INVOKABLE void clearModel();

  private:
    struct Entry
    {
        unsigned int entryId;
        QString name;
    };

    QList<Entry> m_entries;
    unsigned int m_idCounter;
};
