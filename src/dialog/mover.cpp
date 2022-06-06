#include "mover.h"
#include "ui_mover.h"

Mover::Mover(Storage *s, Catalog *catalog, Folder *folder, Link *link,
             bool flag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mover)
{
    ui->setupUi(this);

    this->s = s;
     this->catalog = catalog;
     this->folder = folder;
     this->link = link;
     this->flag = flag;

     if(flag){
         for(int i = 0; i < catalog->getCount(); i++)
         {
             Folder *f = catalog->getFolder(i);

             if(f != folder){
                ui->lstItems->addItem(f->Name());
             }
         }
     } else {
         for(int i = 0; i < s->getCount(); i++)
         {
             Catalog *cat = s->getCatalogById(i);

             if(cat != catalog){
                 ui->lstItems->addItem(cat->Name());
             }
         }

     }
}

Mover::~Mover()
{
    delete ui;
}

void Mover::on_lstItems_clicked(const QModelIndex &I)
{
    if(flag){
            QString folderName = ui->lstItems->item(I.row())->data(Qt::DisplayRole).toString();
            Folder *to = catalog->getFolderByName(folderName);

            to->addLink(link);//добавляем в новую папку
            folder->deleteLink(link);//удаляем из старой папки ссылку

        } else {

            QString catalogName = ui->lstItems->item(I.row())->data(Qt::DisplayRole).toString();
            Catalog *to = s->getCatalogByName(catalogName);

            to->addFolder(folder);//добавляем в новый каталог папку
            catalog->deleteFolder(folder);//удаляем из старого каталога папку
        }

        emit moved();
        close();
}
