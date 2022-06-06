#include "namedialog.h"
#include "ui_namedialog.h"

#include <QDebug>

NameDialog::NameDialog(bool name, Folder *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);

    this->name = name;
    this->f = f;
    load();
}

NameDialog::NameDialog(bool name, Catalog *cat, QWidget *parent)  :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);

    this->name = name;
    this->cat = cat;
    load();
}

NameDialog::~NameDialog()
{
    delete ui;
}


void NameDialog::load()
{
    if(name){
        ui->edtName->setText(f->Name());
        return;
    }

    ui->edtName->setText(cat->Name());
}

void NameDialog::save()
{
    if(name){
        f->setName(ui->edtName->text().simplified());
    } else {
        cat->setName(ui->edtName->text().simplified());
    }
}

Folder *NameDialog::getFolder()
{
    return f;
}

Catalog *NameDialog::getCatalog()
{
    return cat;
}


void NameDialog::on_btnOK_clicked()
{
    save();
    close();
}

