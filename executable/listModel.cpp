#include "listModel.h"

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent), m_entries(), m_idCounter(0)
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return m_entries.count();
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[EntryIdRole] = "entryId";
    roles[NameRole] = "name";
    return roles;
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Entry &entry = m_entries[index.row()];
    switch (role)
    {
    case EntryIdRole:
        return entry.entryId;
    case NameRole:
        return entry.name;
    default:
        return QVariant();
    }
}

void ListModel::appendEntry(const QString &name)
{
    const QString trimmedText = name.trimmed();
    if (trimmedText.isEmpty())
    {
        return;
    }

    beginInsertRows(QModelIndex(), m_entries.count(), m_entries.count());
    m_entries.append({m_idCounter, trimmedText});
    m_idCounter++;
    endInsertRows();
}

void ListModel::deleteEntry(const unsigned int &idToDelete)
{
    for (auto index = 0; index < m_entries.count(); index++)
    {
        if (m_entries[index].entryId == idToDelete)
        {
            beginRemoveRows(QModelIndex(), index, index);
            m_entries.removeAt(index);
            endRemoveRows();
            index--;
        }
    }
}
