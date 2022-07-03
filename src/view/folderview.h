#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QListWidget>

#include "data/storage.h"
#include "helpers/posaction.h"
#include "dialog/folderdialog.h"
#include "dialog/mover.h"

class FolderView : public QListWidget
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    bool isMoved;

public:
    PosAction *actEdit;
    PosAction *actDelete;
    QAction *actNew;
    PosAction *actMove;

public:
    FolderView(QWidget *parent = 0);
    virtual ~FolderView(){}

    void refresh();
    void markEdited();//помечаем все папки как отредактированные (italic)
    void markDeleted();//помечаем все папки как удалённые
    void markRestored();

private slots:
    void contextMenuRequsted(const QPoint &p);
    void acceptStorage(Storage *s);//приём списка внутр моделил из модели
    void acceptCatalog(Catalog* catalog);//приём названия каталога из CatalogsView
    void acceptClear();//приём из представления каталогов

/************************************************/
// Реакция на события из представления
/************************************************/
public slots:
    void newItem();
    void editItem(const QModelIndex &I, QWidget *parent = 0);
    void delItem(const QModelIndex &I, QWidget *parent = 0);
    void moveItem(const QModelIndex &I, QWidget *parent = 0);

signals:
    //Отправка каталога и папки в модель для отображения ссылок из базы
    void sendParents(Catalog*,Folder*);
    void changeMode(bool);

    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};


#endif // FOLDERVIEW_H
