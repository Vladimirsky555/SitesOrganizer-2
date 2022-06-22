#include "catalogsview.h"

#include <QDebug>
#include <QMenu>

CatalogsView::CatalogsView(QWidget *parent) :
    QListWidget(parent)
{
    QFont myFont("MS Shell Dlg 2", 11, QFont::Medium, false);
    setFont(myFont);

    //Включаем режим всплывающего меню для редактирования
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequsted(QPoint)));

    {
        QAction *A = actNew = new QAction(this);
        A->setText(tr("Add"));
        QPixmap p(":/icons/add"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(triggered()), this, SLOT(newItem()));
        addAction(A);
    }{
        PosAction *A = actEdit = new PosAction(this);
        A->setText(tr("Edit"));
        QPixmap p(":/icons/edit"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                this, SLOT(editItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        PosAction *A = actDelete = new PosAction(this);
        A->setText(tr("Delete"));
        QPixmap p(":/icons/delete"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                this, SLOT(delItem(QModelIndex,QWidget*)));
        addAction(A);
    }
}

void CatalogsView::refresh()
{
    for(int i = 0; i < s->getCount(); i++)
    {
        addItem(s->getCatalogById(i)->Name());
        item(i)->setIcon(QIcon(":/icons/list"));
    }
}

void CatalogsView::contextMenuRequsted(const QPoint &p)
{
    QMenu M(this);
    QModelIndex I = indexAt(p);

    M.addAction(actNew);

    if(I.isValid())
    {
        actEdit->I = I;
        actEdit->pWidget = this;
        M.addAction(actEdit);

        actDelete->I = I;
        actDelete->pWidget = this;
        if(item(I.row())->font().strikeOut())
        {
            actDelete->setText(tr("Restore"));
        } else {
            actDelete->setText(tr("Delete"));
        }
        M.addAction(actDelete);
    }

    M.exec(mapToGlobal(p));
}

void CatalogsView::acceptStorage(Storage *s)
{
    this->s = s;
    refresh();
}

void CatalogsView::newItem()
{
    Catalog *catalog = new Catalog();
    catalog->isNew = true;
    s->addCatalog(catalog);

    CatalogDialog cd(catalog);
    cd.exec();

    QListWidgetItem *it = new QListWidgetItem(catalog->Name());
    insertItem(currentRow()+1, it);
}

void CatalogsView::editItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString catalogName = item(I.row())->text();
    Catalog *catalog = s->getCatalogByName(catalogName);
    catalog->isEdited = true;

    //Меняем название папки во внутренней модели
    CatalogDialog cd(catalog);
    cd.exec();

    //Удаляем элемент, создаём новый и вставляем в представление вывода папок
    takeItem(I.row());
    QListWidgetItem *item = new QListWidgetItem(cd.getCatalog()->Name());

    QFont font;
    font.setItalic(true);
    item->setFont(font);

    insertItem(I.row(), item);

    //Помечаем ссылки папок и папки каталога как отредактированные,
    //если они не новые
    for(int i = 0; i < catalog->getCount(); i++)
    {
        Folder *folder = catalog->getFolder(i);

        if(!folder->isEdited){
            folder->isEdited = true;
            for(int j = 0; j < folder->Count(); j++)
            {
                Link *link = folder->getLinkById(j);
                link->isEdited = true;
            }
        }
    }

    emit clearFolders();
    emit sendCatalog(catalog);
}

void CatalogsView::delItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString catalogName = item(I.row())->text();
    Catalog *catalog = s->getCatalogByName(catalogName);
    catalog->isDeleted = true;

    QFont font;
    if(!item(I.row())->font().strikeOut())
    {
        font.setStrikeOut(true);
        item(I.row())->setFont(font);

        for(int i = 0; i < catalog->getCount(); i++)
        {
            Folder *folder = catalog->getFolder(i);
            if(folder->isNew)
            {
                catalog->deleteFolder(folder);
            } else {

                catalog->markDeletedFolder(folder);
                for(int j = 0; j < folder->Count(); j++)
                {
                    folder->markDeletedLinkById(j);
                }
            }
        }

    } else {

        catalog->isDeleted = false;
        font.setStrikeOut(false);
        item(I.row())->setFont(font);

        for(int i = 0; i < catalog->getCount(); i++)
        {
            Folder *folder = catalog->getFolder(i);
            catalog->markRestoredFolder(folder);

            for(int j = 0; j < folder->Count(); j++)
            {
                folder->markRestoredLinkById(j);
            }
        }
    }

    //Папка удаляется после того как были удалены ссылки папки
    if(catalog->isNew)
    {
        s->deleteCatalog(catalog);
        takeItem(I.row());
    }

    emit clearFolders();
    emit clearLinks();
    emit sendCatalog(catalog);
}

void CatalogsView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //Вызываем функцию базового класса
    QListWidget::currentChanged(current, previous);

    item(current.row())->setIcon(QIcon(":/icons/current"));

    if(previous.isValid())
    item(previous.row())->setIcon(QIcon(":/icons/list"));

    if(current.isValid())
    {
        Catalog *catalog = s->getCatalogByName(item(current.row())->text());
        emit clearFolders();
        emit clearLinks();
        emit sendCatalog(catalog);
        return;
    }
}
