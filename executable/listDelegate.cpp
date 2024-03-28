#include "listDelegate.h"

#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

ListDelegate::ListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Calculate areas
    const QRect totalArea = option.rect;
    const QRect textArea = totalArea.adjusted(0, 0, -totalArea.width() * 0.25, 0);
    const QRect buttonArea = totalArea.adjusted(totalArea.width() * 0.75, 0, 0, 0);

    // Draw text
    const QString entryName = index.data(Qt::UserRole + 1).toString();
    painter->drawText(textArea, Qt::AlignVCenter, entryName);

    // Draw button
    QStyleOptionButton button;
    button.rect = buttonArea;
    button.text = "Delete";
    button.state |= QStyle::State_Enabled;
    const bool mouseOver = option.state & QStyle::State_MouseOver;
    if (mouseOver)
    {
        button.state |= QStyle::State_MouseOver;
    }
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

    painter->restore();
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = option.rect.size();
    size.setHeight(40);
    return size;
}

bool ListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                               const QModelIndex &index)
{
    const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    const QRect deleteButtonRect = option.rect.adjusted(option.rect.width() * 0.75, 0, 0, 0);

    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (deleteButtonRect.contains(mouseEvent->pos()))
        {
            // Button click
            const unsigned int entryId = index.data(Qt::UserRole).toUInt();
            emit deleteClicked(entryId);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
