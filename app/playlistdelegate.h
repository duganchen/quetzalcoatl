#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <QStyledItemDelegate>

class PlaylistDelegate : public QStyledItemDelegate
{
public:
    PlaylistDelegate(QValidator *, QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:
    QValidator *m_validator;
};

#endif // PLAYLISTDELEGATE_H
