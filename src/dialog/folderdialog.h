#ifndef FOLDERDIALOG_H
#define FOLDERDIALOG_H

#include <QDialog>

#include "data/storage.h"

namespace Ui {
class FolderDialog;
}

class FolderDialog : public QDialog
{
    Q_OBJECT

    Folder *f;

public:
    FolderDialog(Folder *folder, QWidget *parent = nullptr);
    virtual ~FolderDialog();

    void load();
    void save();

    Folder *getFolder();

private slots:
    void on_btnOK_clicked();

private:
    Ui::FolderDialog *ui;
};

#endif // FOLDERDIALOG_H
