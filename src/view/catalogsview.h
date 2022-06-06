#ifndef CATALOGSVIEW_H
#define CATALOGSVIEW_H

#include <QListWidget>

#include "data/storage.h"
#include "helpers/posaction.h"
#include "dialog/catalogdialog.h"

class CatalogsView : public QListWidget
{
    Q_OBJECT

    Storage *s;

public:
    PosAction *actEdit;
    PosAction *actDelete;
    QAction *actNew;

public:
    CatalogsView(QWidget *parent = 0);
    virtual ~CatalogsView(){}

    void refresh();

private slots:
    void contextMenuRequsted(const QPoint &p);
    void acceptStorage(Storage *s);

/************************************************/
// Реакция на события из представления
/************************************************/
public slots:
    void newItem();
    void editItem(const QModelIndex &I, QWidget *parent = 0);
    void delItem(const QModelIndex &I, QWidget *parent = 0);

    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
    //Отправка каталога для отображения папок из внутренней структуры данных
    void sendCatalog(Catalog*);
    //Очистка списка предыдущих папок, для отображения новых
    void clearFolders();
    void clearLinks();
};

#endif // CATALOGSVIEW_H
