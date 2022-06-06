#ifndef MOVER_H
#define MOVER_H

#include <QDialog>

#include "data/storage.h"

namespace Ui {
class Mover;
}

class Mover : public QDialog
{
    Q_OBJECT    

    Storage *s;
    Catalog *catalog;
    Folder *folder;
    Link *link;
    bool flag;//true - ссылки, false - папки

public:
    Mover(Storage *s, Catalog *catalog, Folder *folder, Link *link,
          bool flag, QWidget *parent = nullptr);
    ~Mover();

private slots:
    void on_lstItems_clicked(const QModelIndex &I);

signals:
    void moved();

private:
    Ui::Mover *ui;
};

#endif // MOVER_H
