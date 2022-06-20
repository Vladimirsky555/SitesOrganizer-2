#include "folderview.h"

#include <QDebug>
#include <QMenu>

FolderView::FolderView(QWidget *parent) :
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
        QPixmap p(":/images/add.png"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(triggered()), this, SLOT(newItem()));
        addAction(A);
    }{
        PosAction *A = actEdit = new PosAction(this);
        A->setText(tr("Edit"));
        QPixmap p(":/images/edit.png"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                this, SLOT(editItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        PosAction *A = actDelete = new PosAction(this);
        A->setText(tr("Delete"));
        QPixmap p(":/images/delete.png"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                this, SLOT(delItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        PosAction *A = actMove = new PosAction(this);
        A->setText(tr("Move"));
        QPixmap p(":/images/move.png"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                this, SLOT(moveItem(QModelIndex,QWidget*)));
        addAction(A);
    }
}

void FolderView::refresh()
{
    int currentCount = currentCatalog->getCount();
    if(currentCount == 0) return;

    for(int j = 0; j < currentCount; j++)
    {
        Folder *folder = currentCatalog->getFolder(j);
        addItem(folder->Name());
    }    

    //Мы делаем папки невидимыми, поэтому каждый раз
    //приходится пробегаться и устанавливать иконку
    for(int i = 0; i < count(); i++)
    {
        item(i)->setIcon(QIcon(":/images/list.png"));
    }

    if(currentCatalog->isEdited)
    {
        markEdited();
    }

    if(currentCatalog->isDeleted)
    {
        markDeleted();
    } else {
        markRestored();
    }
}

void FolderView::markEdited()
{
    QFont font;
    font.setItalic(true);

    for(int i = 0; i < count(); i++)
    {
        item(i)->setFont(font);
    }
}

void FolderView::markDeleted()
{
    QFont font;
    font.setStrikeOut(true);//удаление

    for(int i = 0; i < count(); i++)
    {
        item(i)->setFont(font);
//        qDebug() << item(i)->data(Qt::DisplayRole).toString();
    }
}

void FolderView::markRestored()
{
    QFont font;
    font.setStrikeOut(false);//восстановление

    for(int i = 0; i < count(); i++)
    {
        item(i)->setFont(font);
//        qDebug() << item(i)->data(Qt::DisplayRole).toString();
    }
}

void FolderView::acceptStorage(Storage *s)
{
    this->s = s;
}

void FolderView::acceptCatalog(Catalog *catalog)
{
    this->currentCatalog = catalog;

    refresh();
}

void FolderView::acceptClear()
{
    for(int i = 0; i < count(); i++)
    {
        item(i)->setHidden(true);
    }    
}



void FolderView::newItem()
{
    Folder *folder = new Folder();
    folder->isNew = true;
    currentCatalog->addFolder(folder);

    FolderDialog fd(folder);
    fd.exec();

    QListWidgetItem *item = new QListWidgetItem(folder->Name());
    insertItem(currentRow()+1, item);    

    emit sendParents(currentCatalog, folder);
}

void FolderView::editItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString folderName = item(I.row())->text();
    Folder *folder = currentCatalog->getFolderByName(folderName);

    //Меняем название папки во внутренней модели
    FolderDialog nd(folder);
    nd.exec();

    //Удаляем элемент, создаём новый и вставляем в представление вывода папок
    takeItem(I.row());
    QListWidgetItem *item = new QListWidgetItem(nd.getFolder()->Name());

    QFont font;
    font.setItalic(true);
    item->setFont(font);

    insertItem(I.row(), item);   

    //Помечаем ссылки папки как отредактированные, если они не новые
    if(!folder->isNew)
    {
        for(int i = 0; i < folder->Count(); i++)
        {
            Link *link = folder->getLinkById(i);
            link->isEdited = true;
            link->isMoved = false;
        }
    }


    //Отправляю папку для отображения в модели списка ссылок, удалённых или
    //восстановленных
    emit sendParents(currentCatalog, folder);
}

void FolderView::delItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString folderName = item(I.row())->text();
    Folder *folder = currentCatalog->getFolderByName(folderName);
    folder->isDeleted = true;

    QFont font;
    if(!item(I.row())->font().strikeOut())
    {
        font.setStrikeOut(true);
        item(I.row())->setFont(font);

        //Помечаем ссылки как удалённые
        for(int i = 0; i < folder->Count(); i++)
        {
            Link *link = folder->getLinkById(i);
            if(link->isNew){
                folder->deleteLinkById(i);
            } else {
                folder->markDeletedLinkById(i);
            }
        }
    } else {

        folder->isDeleted = false;
        font.setStrikeOut(false);
        item(I.row())->setFont(font);

        //Помечаем ссылки как нормальные
        for(int i = 0; i < folder->Count(); i++)
        {
            folder->markRestoredLinkById(i);
        }
    }

    //Если папка новая, то удаляется после того как были удалены ссылки папки
    if(folder->isNew)
    {
        currentCatalog->deleteFolder(folder);
        item(I.row())->setHidden(true);
    }

    //Отправляю папку для отображения в модели списка ссылок, удалённых или
    //восстановленных
    emit sendParents(currentCatalog, folder);
}

void FolderView::moveItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString folderName = item(I.row())->text();
    Folder *folder = currentCatalog->getFolderByName(folderName);

    //Помечаем папку и ссылки в ней как отредактированные, чтобы их можно
    //было занести в базу
    folder->isEdited = true;


    for(int i = 0; i < folder->Count(); i++)
    {
        Link *l = folder->getLinkById(i);
        l->isEdited = true;
        l->isMoved = true;
    }

    //Запускаем мувер
    Mover *mover = new Mover(s, currentCatalog, folder, NULL, false);
    mover->exec();

    //Скрываем перемещённую папку
    if(isMoved)
    item(I.row())->setHidden(true);

    emit sendParents(currentCatalog, folder);
}

void FolderView::contextMenuRequsted(const QPoint &p)
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
            actDelete->setText("Восстановить");
        } else {
            actDelete->setText("Удалить");
        }

        M.addAction(actDelete);

        actMove->I = I;
        actMove->pWidget = this;
        M.addAction(actMove);
    }

    M.exec(mapToGlobal(p));
}



void FolderView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //Вызываем функцию базового класса
    QListWidget::currentChanged(current, previous);

    item(current.row())->setIcon(QIcon(":/images/current.png"));

    if(previous.isValid())
    item(previous.row())->setIcon(QIcon(":/images/list.png"));

    QString folder = item(currentRow())->text();

    for(int i = 0; i < s->getCount(); i++)
    {
        Catalog *catalog = s->getCatalogById(i);
        for(int j = 0; j < catalog->getCount(); j++)
        {
            Folder *f = catalog->getFolder(j);
            if(f->Name() == folder)
            {
                if(current.isValid()){
                    emit sendParents(currentCatalog, f);
                    return;
                }
            }
        }
    }
}
