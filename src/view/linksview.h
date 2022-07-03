#ifndef LINKSVIEW_H
#define LINKSVIEW_H

#include <QTableView>
#include <QFile>

#include "data/data.h"
#include "data/storage.h"
#include "model/model.h"
#include "helpers/posaction.h"

class LinksView : public QTableView
{
    Q_OBJECT

    Model *M;
    QModelIndex currentIndex;
    Folder *currentFolder;
    bool search;

public:
    PosAction *actEdit;
    PosAction *actDelete;
    PosAction *actOpen;
    QAction *actNew;
    PosAction *actMove;

public:
    LinksView(QWidget *parent = 0);
    virtual ~LinksView(){}

private slots:
    void contextMenuRequsted(const QPoint &p);
    void acceptSearchMode(bool search);

    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
    void link_item_selected(QModelIndex id);

};

#endif // LINKSVIEW_H
