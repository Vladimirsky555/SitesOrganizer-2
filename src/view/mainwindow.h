#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressBar>
#include <QTranslator>

#include "ui_mainwindow.h"
#include "data/data.h"
#include "linksview.h"
#include "folderview.h"
#include "catalogsview.h"
#include "helpers/counter.h"
#include "searchview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Counter *counter;
    QProgressBar *bar;

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow(){delete ui;}

    //Метод для запуска сигнала для отображения папок в FoldersView
    void sendStorage();

private slots:
    void on_actionFileToModel_triggered();
    void acceptRange(int range);

signals:
    void startImport_File_Model(QString);
    void sendStorageSignal();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void changeEvent(QEvent *event) override;
};
#endif // MAINWINDOW_H
