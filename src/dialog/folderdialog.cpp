#include "folderdialog.h"
#include "ui_folderdialog.h"

FolderDialog::FolderDialog(Folder *folder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FolderDialog)
{
    ui->setupUi(this);
    this->f = folder;

    load();
}

FolderDialog::~FolderDialog()
{
    delete ui;
}

void FolderDialog::load()
{
    ui->edtName->setText(f->Name());
}

void FolderDialog::save()
{
    f->setName(ui->edtName->text().simplified());
}

Folder *FolderDialog::getFolder()
{
    return this->f;
}

void FolderDialog::on_btnOK_clicked()
{
    save();
    close();
}

