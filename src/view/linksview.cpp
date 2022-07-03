#include "linksview.h"

#include <QHeaderView>
#include <QMenu>
#include <QDebug>

LinksView::LinksView(QWidget *parent) :
    QTableView(parent)
{
    //Создаём модель
    M = new Model(this);
    setModel(M);

    this->search = false;

    //Передача индекса в Model
    connect(this, SIGNAL(link_item_selected(QModelIndex)),
            M, SLOT(accept_link_item_selected(QModelIndex)));


    //Включаем режим всплывающего меню для редактирования
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequsted(QPoint)));

    {
        PosAction *A = actEdit = new PosAction(this);
        A->setText(tr("Edit"));
        QPixmap p(":/icons/edit"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                M, SLOT(editItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        QAction *A = actNew = new QAction(this);
        A->setText(tr("Add"));
        QPixmap p(":/icons/add"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(triggered()), M, SLOT(newItem()));
        addAction(A);
    }{
        PosAction *A = actDelete = new PosAction(this);
        A->setText(tr("Delete"));
        QPixmap p(":/icons/delete"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                M, SLOT(delItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        QAction *A = actOpen = new PosAction(this);
        A->setText(tr("Open"));
        QPixmap p(":/icons/open"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                M, SLOT(openItem(QModelIndex,QWidget*)));
        addAction(A);
    }{
        QAction *A = actMove = new PosAction(this);
        A->setText(tr("Move"));
        QPixmap p(":/icons/move"); A->setIcon(QIcon(p));
        A->setFont(QFont ("MS Shell Dlg 2", 10));
        connect(A, SIGNAL(actionOnItem(QModelIndex,QWidget*)),
                M, SLOT(moveItem(QModelIndex,QWidget*)));
        addAction(A);
    }

    {//Выравнивание высоты вертикальных строк под контекст
        QHeaderView *V = verticalHeader();
        V->setSectionResizeMode(QHeaderView::ResizeToContents);
    } {//Выравнивание горизонтальных строк
        QHeaderView *H = horizontalHeader();
        H->setSectionResizeMode(QHeaderView::Interactive);
        //H->setSectionResizeMode(2, QHeaderView::Stretch);//растягиваем заголовок
    }

    setSelectionBehavior(QAbstractItemView::SelectRows);
//    setSelectionMode(QAbstractItemView::SingleSelection);


    //    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    setColumnHidden(0, true);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    //    resizeColumnToContents(0);

    setAlternatingRowColors(false);

    QFont myFont("MS Shell Dlg 2", 11, QFont::Normal, false);
    setFont(myFont);

}

void LinksView::contextMenuRequsted(const QPoint &p)
{
    QMenu M(this);
    //Показывать элемент, если есть что редактировать
    QModelIndex I = indexAt(p);
    if(I.isValid())
    {
        actOpen->I = I;
        actOpen->pWidget = this;//или NULL
        M.addAction(actOpen);
    }

    if(!search) M.addAction(actNew);

    if(I.isValid() && !search)
    {
        actEdit->I = I;
        actEdit->pWidget = this;
        M.addAction(actEdit);

        actDelete->I = I;
        actDelete->pWidget = this;

        Data *D = this->M->at(I.row());//получаем элемент модели
        Folder *f = this->M->getCurrentFolder();//папка внутренней модели

        //Выходим на ссылку внутренней модели, чтобы проверить на удалена или нет
        for(int i = 0; i < f->Count(); i++){
            if(D->Id() == f->getLinkById(i)->Id())
            {
                Link *l = f->getLinkById(i);

                if(l->isDeleted)
                {
                    actDelete->setText(tr("Restore"));
                } else {
                    actDelete->setText(tr("Delete"));
                }
            }
        }

//        if(D->isDeleted)
//        {
//            actDelete->setText("Восстановить");
//        } else {
//            actDelete->setText("Удалить");
//        }                

        M.addAction(actDelete);

        actMove->I = I;
        actMove->pWidget = this;
        M.addAction(actMove);
    }

    M.exec(mapToGlobal(p)); //тоже промахивается
}

void LinksView::acceptSearchMode(bool search)
{
    this->search = search;
}


void LinksView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //Вызываем функцию базового класса
    QTableView::currentChanged(current, previous);

    if(current.isValid())
    {
        emit link_item_selected(current);
        return;
    }

    emit link_item_selected(QModelIndex());
}

