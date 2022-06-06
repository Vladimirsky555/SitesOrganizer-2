#include "linkdialog.h"
#include "ui_linkdialog.h"

LinkDialog::LinkDialog(Link *L, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinkDialog)
{
    ui->setupUi(this);

    setWindowTitle("Редактирование элемента");
    this->L = L;
    load();
}

LinkDialog::~LinkDialog()
{
    delete ui;
}

void LinkDialog::load()
{
    ui->edtName->setText(L->linkName());
    ui->edtLink->setText(L->linkReal());
    ui->edtDescription->setText(L->Description());
}

void LinkDialog::save()
{
    L->setlinkName(ui->edtName->text().simplified());
    L->setlinkReal(ui->edtLink->text().simplified());
    L->setDescription(ui->edtDescription->toPlainText().simplified());
}

void LinkDialog::on_btnCancel_clicked()
{
    close();
}


void LinkDialog::on_btnAdd_clicked()
{
    save();
    close();
}

