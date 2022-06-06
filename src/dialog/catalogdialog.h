#ifndef CATALOGDIALOG_H
#define CATALOGDIALOG_H

#include <QDialog>

#include "data/storage.h"

namespace Ui {
class CatalogDialog;
}

class CatalogDialog : public QDialog
{
    Q_OBJECT

    Catalog *catalog;

public:
    CatalogDialog(Catalog *catalog, QWidget *parent = nullptr);
    virtual ~CatalogDialog();

    void load();
    void save();

    Catalog *getCatalog();

private slots:
    void on_btnOK_clicked();

private:
    Ui::CatalogDialog *ui;
};

#endif // CATALOGDIALOG_H
