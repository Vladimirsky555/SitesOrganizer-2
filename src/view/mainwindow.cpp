#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QDockWidget>

#include <QStyle>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Органайзер сайтов");

//    QDesktopWidget desktop;
//    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
//    QPoint center = rect.center(); //координаты центра экрана
//    int x = center.x() - (this->width()/2);  // учитываем половину ширины окна
//    int y = center.y() - (this->height()/2); // .. половину высоты
//    center.setX(x);
//    center.setY(y);
//   this->move(center);

    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );

    //Представление для ссылок
    LinksView *lw = new LinksView(this);
    setCentralWidget(lw);

    //Представление для папок
    CatalogsView *cv = new CatalogsView(this);
    QDockWidget *D2 = new QDockWidget(this);
    D2->setWindowTitle("Каталоги");
    D2->setWidget(cv);
    addDockWidget(Qt::LeftDockWidgetArea, D2);

    FolderView *fv = new FolderView(this);
    QDockWidget *D1 = new QDockWidget(this);
    D1->setWindowTitle("Папки");
    D1->setWidget(fv);
    addDockWidget(Qt::LeftDockWidgetArea, D1);

    bar = new QProgressBar(this);
    QDockWidget *D3 = new QDockWidget(this);
    D3->setWindowTitle("Progress bar");
    D3->setWidget(bar);
    addDockWidget(Qt::BottomDockWidgetArea, D3);

    //Организация прогресс-бара
    counter = new Counter();
    //От каунтера в прогресс-бар
    connect(counter, SIGNAL(sendValue(int)),
            bar, SLOT(setValue(int)));

    //Из модели в каунтер
    connect(lw->model(), SIGNAL(sendCount(int)),
            counter, SLOT(acceptCount(int)));

    //определение размерности прогрес-бара
    connect(lw->model(), SIGNAL(sendRange(int)),
            this, SLOT(acceptRange(int)));


    //Так как мы создали класс, унаследовав его от QTableView (а не от QWidget),
    //то нам доступен метод model() от созданного в MainWindow экземпляра
    //класса LinkView
    connect(this, SIGNAL(startImport_File_Model(QString)),
            lw->model(), SLOT(import_File_Model(QString)));


    //Передача указателя Storage* в CatalogsView и FoldersView при запуске программы
    connect(this, SIGNAL(sendStorageSignal()),
            lw->model(), SLOT(acceptStorage()));

    connect(lw->model(), SIGNAL(sendStorage(Storage*)),
            fv, SLOT(acceptStorage(Storage*)));

    connect(lw->model(), SIGNAL(sendStorage(Storage*)),
            cv, SLOT(acceptStorage(Storage*)));


    //Передача указателей на каталок и папку из FoldersView в Model
    connect(fv, SIGNAL(sendParents(Catalog*,Folder*)),
            lw->model(), SLOT(acceptParents(Catalog*,Folder*)));

    //Очищение списка папок из представления каталогов,
    //при создании нового каталога
    connect(cv, SIGNAL(clearFolders()),
            fv, SLOT(acceptClear()));

    //Очищение списка ссылок при клике на каталог
    connect(cv, SIGNAL(clearLinks()),
            lw->model(), SLOT(acceptClear()));

    //Передача названия каталога из CatalogsView в FoldersView
    //для отображения папок каталога
    connect(cv, SIGNAL(sendCatalog(Catalog*)),
            fv, SLOT(acceptCatalog(Catalog*)));

    //Сохранение в базу
//    connect(this, SIGNAL(saveToDb()),
//            lw->model(), SLOT(acceptSaveToDb()));



    //По другому не получилось отобразить папки в представлении FoldersView
    //Пришлось создать сигнал из MainWindow, чтобы он запустил сигнал из модели,
    //который уже отправил список папок в представление FoldersView
    sendStorage();
}


void MainWindow::sendStorage()
{
    emit sendStorageSignal();
}


/************************************************/
// Блок запуска импорта данных из файла
/************************************************/
void MainWindow::on_actionFileToModel_triggered()
{
    QFileDialog dialog;
    QString filename = dialog.getOpenFileName();

    emit startImport_File_Model(filename);
}

void MainWindow::acceptRange(int range)
{
    bar->setRange(0, range - 5);
}





