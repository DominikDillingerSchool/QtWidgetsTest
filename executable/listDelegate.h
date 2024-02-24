#pragma once

#include <QStyledItemDelegate>

class ListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

  public:
    ListDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

  signals:
    void deleteClicked(const unsigned int &entryId);
};