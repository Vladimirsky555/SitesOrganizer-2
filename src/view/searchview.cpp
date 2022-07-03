#include "searchview.h"
#include "ui_searchview.h"

#include <QDebug>

SearchView::SearchView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchView)
{
    ui->setupUi(this);

    ui->btnOK->setDefaultAction(ui->actionOK);
    connect(ui->actionOK, SIGNAL(triggered()),
            this, SLOT(sendPattern()));
}

SearchView::~SearchView()
{
    delete ui;
}

void SearchView::sendPattern()
{
    QString pattern = ui->edtText->text();
    emit sendPattern(pattern);
    emit changeMode(true);
}

void SearchView::on_edtText_returnPressed()
{
    QString pattern = ui->edtText->text();
    emit sendPattern(pattern);
    emit changeMode(true);
}

