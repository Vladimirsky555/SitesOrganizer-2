#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QDockWidget>
#include <QStyle>
#include <QDesktopWidget>
#include <QToolBar>
#include <QComboBox>


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

    //Несколько тестовых припаркованных окон. Пункт меню для окон
    //QIcon Ic(":/images/move.png"); //menuBar()->addMenu(Ic, tr("Windows"));
    QMenu *mWindows = menuBar()->addMenu(tr("Windows"));

    //Представление для ссылок
    LinksView *lw = new LinksView(this);
    setCentralWidget(lw);

    //Припаркованное окно представления для каталогов

//    QComboBox *cb = new QComboBox(this);
//    QDockWidget *cbD = new QDockWidget(this);
//    cbD->setWindowTitle(tr("Lan Combobox"));
//    cbD->setWidget(cb);
//    addDockWidget(Qt::LeftDockWidgetArea, cbD);
//    mWindows->addAction(cbD->toggleViewAction());

//    cb->addItems(QStringList() << "rus" << "eng");

//    connect(cb, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
//            [=](const QString &str)
//    {
//        translator.load(":lang/so_" + str + ".");//Загружаем перевод
//        qApp->installTranslator(&translator);//Устанавливаем перевод в приложение
//    });

//    //Сделаем первоначальную инициализацию для окна приложения
//    translator.load(":lang/so_rus.qm");
//    qApp->installTranslator(&translator);

    SearchView *sv = new SearchView(this);
    QDockWidget *Dsv = new QDockWidget(this);
    Dsv->setWindowTitle(tr("Search"));
    Dsv->setWidget(sv);
    addDockWidget(Qt::LeftDockWidgetArea, Dsv);
    mWindows->addAction(Dsv->toggleViewAction());

    //Припаркованное окно представления для каталогов
    CatalogsView *cv = new CatalogsView(this);
    //cv->setStyleSheet("background: #FAF0E6");
    QDockWidget *D2 = new QDockWidget(this);
    D2->setWindowTitle(tr("Catalogs"));
    D2->setWidget(cv);
    addDockWidget(Qt::LeftDockWidgetArea, D2);
    mWindows->addAction(D2->toggleViewAction());

    //Припаркованное окно представления для папок
    FolderView *fv = new FolderView(this);
    //fv->setStyleSheet("background: #FAF0E6");
    QDockWidget *D1 = new QDockWidget(this);
    D1->setWindowTitle(tr("Folders"));
    D1->setWidget(fv);
    addDockWidget(Qt::LeftDockWidgetArea, D1);
    mWindows->addAction(D1->toggleViewAction());



//    FolderView *fv = new FolderView(this);
//    //fv->setStyleSheet("background: #FAF0E6");
//    QDockWidget *D1 = new QDockWidget(this);
//    D1->setWindowTitle(tr("Folders"));
//    D1->setWidget(fv);
//    addDockWidget(Qt::LeftDockWidgetArea, D1);
//    mWindows->addAction(D1->toggleViewAction());

    //Припаркованное окно прогрессбара
    bar = new QProgressBar(this);
    QDockWidget *D3 = new QDockWidget(this);
    D3->setWindowTitle(tr("Progress bar"));
    D3->setWidget(bar);
    addDockWidget(Qt::BottomDockWidgetArea, D3);
    mWindows->addAction(D3->toggleViewAction());

    //Создание панели инструментов с событиями и пустой
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("ToolBar"));
    tb->addAction(ui->actionFileToModel);
    tb->addAction(ui->actionTranslateToEng);
    tb->addAction(ui->actionTranslateToRus);
    addToolBar(Qt::TopToolBarArea, tb);
    mWindows->addAction(tb->toggleViewAction());

    //    QToolBar *tbEmpty = new QToolBar(this);
    //    tbEmpty->setWindowTitle(tr("Empty"));
    //    addToolBar(Qt::LeftToolBarArea, tbEmpty);
    //    mWindows->addAction(tbEmpty->toggleViewAction());

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

    //Передача ключевого слова для поиска в модель из виджета
    connect(sv, SIGNAL(sendPattern(QString)),
            lw->model(), SLOT(acceptPattern(QString)));

    //Информирование о режиме поиска для представления вывода ссылок
    connect(sv, SIGNAL(changeMode(bool)),
            lw, SLOT(acceptSearchMode(bool)));

    connect(fv, SIGNAL(changeMode(bool)),
            lw, SLOT(acceptSearchMode(bool)));

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



void MainWindow::on_actionTranslateToEng_triggered()
{
    //    QTranslator *pTranslator = APP->getTranslator();
    //    pTranslator->load(":lan/eng");
    //    APP->installTranslator(pTranslator);
}


void MainWindow::on_actionTranslateToRus_triggered()
{
    //    QTranslator *pTranslator = APP->getTranslator();
    //    pTranslator->load(":lan/rus");
    //    APP->installTranslator(pTranslator);
}

void MainWindow::changeEvent(QEvent *event)
{
    //В случае получения события о смене языка
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);//переведём окно заново
    }
}

