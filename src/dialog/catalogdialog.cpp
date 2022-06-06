#include "catalogdialog.h"
#include "ui_catalogdialog.h"

CatalogDialog::CatalogDialog(Catalog *catalog, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatalogDialog)
{
    ui->setupUi(this);
    this->catalog = catalog;

    load();
}

CatalogDialog::~CatalogDialog()
{
    delete ui;
}

void CatalogDialog::load()
{
    ui->edtName->setText(catalog->Name());
}

void CatalogDialog::save()
{
    catalog->setName(ui->edtName->text().simplified());
}

Catalog *CatalogDialog::getCatalog()
{
    return this->catalog;
}

void CatalogDialog::on_btnOK_clicked()
{
    save();
    close();
}

