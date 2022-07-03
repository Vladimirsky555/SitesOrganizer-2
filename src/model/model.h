#ifndef MODEL_H
#define MODEL_H

#include <QSqlTableModel>
#include <QFile>

#include "data/data.h"
#include "data/storage.h"
#include "dialog/linkdialog.h"
#include "dialog/mover.h"


class Model : public QSqlTableModel
{
    Q_OBJECT

    QList<Data*> *items;//Список указателей на элементы
    Storage *s;

    QModelIndex currentIndex;
    Catalog *currentCatalog;
    Folder *currentFolder;

    bool search;//режим поиска

public:
    Model(QObject *parent = nullptr);
    virtual ~Model();

    /************************************************/
    // Вспомогательные функции
    /************************************************/
    void addItem(Data *item);
    void addCatalog(Data *item);
    void searchByPattern(QString pattern, bool mode);

    Data *at(int k);
    Folder *getCurrentFolder();

    void fillStorage();
    void sendStorage();//чтобы не в конструкторе

    /************************************************/
    // Выборка из базы данных
    /************************************************/
    void selectAll();
    void selectByFolderName(QString folderName);

    /************************************************/
    // Блок работ с базой данных
    /************************************************/
    bool import_to_db_item(Data *item);
    void save_to_db_all();

    void addItemToDb(Data *item);
    void editItemInDb(Data *item);
    void deleteItemFromDb(Data *item);

    /************************************************/
    // Блок запуска импорта данных из файла
    /************************************************/
private:
    int defineCounts(QString filename);
    QStringList importFolders(int countLines, QString filename);
    bool checkRegExp(QRegExp rx);

private slots:
    void import_File_Model(QString filename);
    void acceptStorage();//перенаправление сигнала от MainWindow для отображения папок при запуске
    void acceptParents(Catalog* catalog, Folder *folder);//Приём папки для отображения в представлении ссылок
    void accept_link_item_selected(QModelIndex index);//Приём индекса из LinkView
    void acceptClear();//очищение списка ссылок по клику на каталог
    void acceptPattern(QString pattern, bool mode);
//    void acceptSaveToDb();

    /************************************************/
    // Реакция на события из представления
    /************************************************/
public slots:
    void newItem();
    void editItem(const QModelIndex &I, QWidget *parent = 0);
    void delItem(const QModelIndex &I, QWidget *parent = 0);
    void openItem(const QModelIndex &I, QWidget *parent);
    void moveItem(const QModelIndex &I, QWidget *parent);

signals:
    void sendFolders(QList<QString>*);
    void sendStorage(Storage*);
    void sendCount(int);//в каунтер для прогресс-бара
    void sendRange(int);

    /************************************************/
    // QAbstractItemModel interface
    /************************************************/
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &I, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    /************************************************/
    // Вспомогательные функции для работы с моделью
    /************************************************/
protected:
    virtual QVariant dataFont(const QModelIndex &I) const;
    virtual QVariant dataDisplay(const QModelIndex &I) const;
    virtual QVariant dataForeground(const QModelIndex &I) const;
    virtual QVariant dataTextAlignment(const QModelIndex &I) const;
    virtual QVariant dataToolTip(const QModelIndex &I) const;

};

#endif // MODEL_H
