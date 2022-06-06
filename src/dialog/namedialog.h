#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>

#include "data/storage.h"

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

    Folder *f;
    Catalog *cat;
    bool name;

public:
    NameDialog(bool name, Folder *f, QWidget *parent = nullptr);
    NameDialog(bool name, Catalog *cat, QWidget *parent = nullptr);
    virtual ~NameDialog();

    void load();
    void save();

    Folder *getFolder();
    Catalog *getCatalog();

private slots:
    void on_btnOK_clicked();

private:
    Ui::NameDialog *ui;
};

#endif // NAMEDIALOG_H
