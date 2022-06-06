#ifndef LINKDIALOG_H
#define LINKDIALOG_H

#include "data/storage.h"

#include <QDialog>

namespace Ui {
class LinkDialog;
}

class LinkDialog : public QDialog
{
    Q_OBJECT

    Link *L;

public:
    LinkDialog(Link *L, QWidget *parent = nullptr);
    virtual ~LinkDialog();

    void load();
    void save();

private slots:
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();

private:
    Ui::LinkDialog *ui;
};

#endif // LINKDIALOG_H
