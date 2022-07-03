#include "searchview.h"
#include "ui_searchview.h"


SearchView::SearchView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchView)
{
    setWindowTitle(tr("Search"));

    ui->setupUi(this);
    ui->rName->setChecked(true);
}

SearchView::~SearchView()
{
    delete ui;
}


void SearchView::on_edtText_returnPressed()
{
    QString pattern = ui->edtText->text();

    if(ui->rName->isChecked()){
        emit sendPattern(pattern, true);
    } else {
        emit sendPattern(pattern, false);
    }

    emit changeMode(true);
}

