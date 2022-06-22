#include "model.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSqlRecord>

Model::Model(QObject *parent) : QSqlTableModel(parent)
{
    //Устанавливаем стратегию редактирования
    //    setEditStrategy(QSqlTableModel::OnFieldChange);
    setEditStrategy(QSqlTableModel::OnRowChange);

    this->items = new QList<Data*>;
    this->s = new Storage();


    selectAll();//получаем из базы все элементы, заполняя модель Model (QList<Data*> items)
    fillStorage();//заполняем собственную иерархическую модель Storage->Catalog->Folder->Link
    items->clear();//очищаем, ибо будет выведен весь список, а у нас вывод только по клику на папку
    sendStorage();//отправляем список папок в представление FolderView
    //и список категорий в представление CatalogsView
}

Model::~Model()
{
    save_to_db_all();
}

void Model::sendStorage()
{
    emit sendStorage(s);
}


void Model::acceptClear()
{
    beginResetModel();
    items->clear();
    endResetModel();
}

//void Model::acceptSaveToDb()
//{
//    items->clear();
//    Data *D = new Data();

//    for(int i = 0; i < s->getCount(); i++)
//    {
//        Catalog *catalog = s->getCatalogById(i);

//        //Удаляем каталог без папок
//        if(catalog->getCount() == 0)
//            s->deleteCatalog(catalog);

//        for(int j = 0; j < catalog->getCount(); j++)
//        {
//            Folder *folder = catalog->getFolder(j);

//            //Удаляем папку без ссылок
//            if(folder->Count() == 0)
//                catalog->deleteFolder(folder);

//            for(int l = 0; l < folder->Count(); l++)
//            {
//                Link *link = folder->getLinkById(l);

//                D->setId(link->Id());
//                D->setCatalog(catalog->Name());
//                D->setFolder(folder->Name());
//                D->setlinkName(link->linkName());
//                D->setlinkReal(link->linkReal());
//                D->isDeleted = link->isDeleted;

//                if(link->isDeleted)
//                {
//                    deleteItemFromDb(D);
//                    qDebug() << folder->Name() <<
//                                link->Id().toInt();
//                    folder->deleteLink(link);
//                }
//            }
//        }
//    }
//}



/************************************************/
// Методы модели
/************************************************/
void Model::addItem(Data *item)
{
    if(item != NULL){
        items->append(item);
    }
    return;
}


//Если каталог существует - выходим из функции,
//если не существует - создаём его
void Model::addCatalog(Data *item)
{
    Catalog *cat = s->getCatalogByName(item->Catalog());
    if(cat == NULL)
    {
        if(item->Catalog() != ""){
            cat = new Catalog(item->Catalog());
            s->addCatalog(cat);
        }
    }

    return;
}


Data *Model::at(int k)
{
    return this->items->at(k);
}

Folder *Model::getCurrentFolder()
{
    return this->currentFolder;
}

void Model::fillStorage()
{
    QStringList folders;//временный список, чтобы не дублировать папки

    //На 1-м этапе пробегаемся по списку элементов и каждой категории приписываем папки
    for(int i = 0; i < items->count(); i++)
    {
        for(int j = 0; j < s->getCount(); j++)
        {
            QString folder = items->at(i)->Folder();
            Catalog *catalog = s->getCatalogById(j);

            if(items->at(i)->Catalog() == catalog->Name())
            {
                //Создаём папку, если её имени нет во временном списке folders
                if(!folders.contains(folder))
                {
                    Folder *f = new Folder(folder);
                    catalog->addFolder(f);
                }

                folders.append(folder);//уже добавленную папку во временный список
            }
        }
    }

    //На 2-м этапе снова пробегаемся по списку элементов и добавляем в папки остальные данные
    for(int i = 0; i < items->count(); i++)
    {
        for(int j = 0; j < s->getCount(); j++)
        {
            Catalog *catalog = s->getCatalogById(j);
            for(int k = 0; k < catalog->getCount(); k++)
            {
                Folder *folder = catalog->getFolder(k);
                if(items->at(i)->Folder() == folder->Name())
                {
                    Link *link = new Link(items->at(i)->Id(),
                                          items->at(i)->linkName(),
                                          items->at(i)->linkReal(),
                                          items->at(i)->Date());
                    folder->addLink(link);
                }
            }
        }
    }
}




/************************************************/
// Блок работы с базой данных
/************************************************/
void Model::selectAll()
{
    items->clear();

    QSqlQuery qry;
    qry.prepare(
                "SELECT                   \n"
                "   id,                   \n"
                "   catalog,              \n"
                "   folder,               \n"
                "   date,                 \n"
                "   link_name,            \n"
                "   link_real,            \n"
                "   description           \n"
                "   FROM linksDB;         \n"
                );

    if(qry.exec()){
        while(qry.next())
        {
            Data *item = new Data(this, qry);
            addItem(item);
            addCatalog(item);
        }
    } else {
        qCritical() << "Cannot execute query!";
        QSqlError err = qry.lastError();
        qCritical() << err.nativeErrorCode();
        qCritical() << err.databaseText().toUtf8().data();
        qCritical() << err.driverText().toUtf8().data();
        qDebug() << qry.executedQuery();
    }
}

void Model::selectByFolderName(QString folderName)
{        
    for(int i = 0; i < items->count(); i++)
    {
        delete items->at(i);
    }

    items->clear();

    beginResetModel();
    for(int i = 0; i < s->getCount(); i++)
    {
        Catalog *catalog = s->getCatalogById(i);
        for(int j = 0; j < catalog->getCount(); j++)
        {
            Folder *folder = catalog->getFolder(j);
            for(int l = 0; l < folder->Count(); l++)
            {
                Link *link = folder->getLinkById(l);
                if(folder->Name() == folderName)
                {
                    Data *D = new Data();

                    D->setId(link->Id());
                    D->setCatalog(catalog->Name());
                    D->setFolder(folder->Name());
                    D->setlinkName(link->linkName());
                    D->setlinkReal(link->linkReal());
                    D->setDate(link->Date());

                    D->isDeleted = link->isDeleted;
                    D->isEdited = link->isEdited;
                    D->isNew = link->isNew;
                    D->isMoved = link->isMoved;
                    D->isOpened = link->isOpened;

                    addItem(D);
                }
            }
        }
    }

    endResetModel();
}


//Используется при импорте из файла, для записи каждого элемента в базу данных
bool Model::import_to_db_item(Data *item)
{
    QSqlQuery query;
    query.setForwardOnly(true);

    query.prepare("INSERT INTO linksDB (catalog, folder, link_name, link_real)"
                  "VALUES (:CATALOG, :FOLDER, :LINK_NAME, :LINK_REAL)");

    query.bindValue(":CATALOG", item->Catalog());
    query.bindValue(":FOLDER", item->Folder());
    query.bindValue(":LINK_NAME", item->linkName());
    query.bindValue(":LINK_REAL", item->linkReal());

    if(query.exec()) return true;

    qCritical() << query.lastError().databaseText();
    qCritical() << query.lastError().driverText();
    qCritical() << query.lastError().nativeErrorCode();

    return false;
}

void Model::save_to_db_all()
{
    Data *D = new Data();

    for(int i = 0; i < s->getCount(); i++)
    {
        Catalog *catalog = s->getCatalogById(i);
        for(int j = 0; j < catalog->getCount(); j++)
        {
            Folder *folder = catalog->getFolder(j);
            for(int l = 0; l < folder->Count(); l++)
            {
                Link *link = folder->getLinkById(l);

                D->setId(link->Id());
                D->setCatalog(catalog->Name());
                D->setFolder(folder->Name());
                D->setlinkName(link->linkName());
                D->setlinkReal(link->linkReal());
                D->setDate(link->Date());

                if(link->isNew){
                    addItemToDb(D);
                } else {
                    if(link->isDeleted){
                        deleteItemFromDb(D);
                    }

                    if(link->isEdited){
                        editItemInDb(D);
                    }
                }
            }
        }
    }
}

//Добавление элемента в базу
void Model::addItemToDb(Data *item)
{
    import_to_db_item(item);
}

//Редактирование элемента в базе
void Model::editItemInDb(Data *item)
{
    QSqlQuery UPD;
    UPD.setForwardOnly(true);

    UPD.prepare("UPDATE linksDB SET            "
                "   catalog = :CATALOG,      "
                "   folder = :FOLDER,        "
                "   date = :DATE,          "
                "   link_name = :LINK_NAME,     "
                "   link_real = :LINK_REAL,     "
                "   description = :DESCRIPTION  "
                "   WHERE id = :ID; "
                );

    UPD.bindValue(":ID", item->Id().toInt());
    UPD.bindValue(":CATALOG", item->Catalog());
    UPD.bindValue(":FOLDER", item->Folder());
    UPD.bindValue(":DATE", item->Date());
    UPD.bindValue(":LINK_NAME", item->linkName());
    UPD.bindValue(":LINK_REAL", item->linkReal());
    UPD.bindValue(":DESCRIPTION", item->Description());

    if(UPD.exec()) return;

    qCritical() << UPD.lastError().databaseText();
    qCritical() << UPD.lastError().driverText();
    qCritical() << UPD.lastError().nativeErrorCode();
}

//Удаление элемента из базы
void Model::deleteItemFromDb(Data *item)
{
    QSqlQuery DEL;
    DEL.setForwardOnly(true);

    DEL.prepare("DELETE FROM linksDB WHERE id = :ID;");
    DEL.bindValue(":ID", item->Id());

    if(!DEL.exec()){
        qCritical() << DEL.lastError().databaseText();
        qCritical() << DEL.lastError().driverText();
        qCritical() << DEL.lastError().nativeErrorCode();
    }
}

/************************************************/
// Блок работы с файлом данных
/************************************************/
int Model::defineCounts(QString filename)
{
    int countLines = 0;
    QFile file;

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);

    for(int i = 0; i < 10000; i++){
        QByteArray arr = file.readLine();
        if(QString::fromUtf8(arr) != ""){
            countLines++;            
        } else {
            break;
        }
    }

    file.close();
    return countLines + 1;    
}

QStringList Model::importFolders(int countLines, QString filename)
{
    QFile file;
    QStringList folders;

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);

    for(int i = 0; i < countLines; i++){

        QByteArray arr = file.readLine();
        QRegExp rx("\">(.*)</H");

        int pos = 0;
        while((pos = rx.indexIn(QString::fromUtf8(arr), pos)) != -1){
            pos += rx.matchedLength();

            QString value = rx.cap(0).remove(0,2);
            value.chop(3);

            folders.append(value);
        }
    }

    return folders;
    file.close();
}

//Импорт всех ссылок с разбивкой на папки
void Model::import_File_Model(QString filename)
{
    if(filename == "") return;

    QFile file;
    QRegExp rx_name; rx_name.setPattern("\">[^</A>]+");
    QRegExp rx_link; rx_link.setPattern("(http|https)://[^\"]+");

    int countLines = defineCounts(filename);
    emit sendRange(countLines);

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);

    //Сначала получаем список папок
    QStringList folders = importFolders(countLines, filename);

    //Парсим названия сайтов и ссылки
    QString value;
    QString folder;
    int pos;
    QString exept = "";
    for(int i = 0; i < countLines; i++)
    {
        QByteArray arr = file.readLine();

        Data *item = new Data();

        pos = 0;
        while((pos = rx_name.indexIn(QString::fromUtf8(arr), pos)) != -1){
            pos += rx_name.matchedLength();
            value = rx_name.cap(0).remove(0,2);

            for(int j = 0; j < folders.count(); j++)
            {
                if(value == folders.at(j))
                {
                    folder = folders.at(j);
                    exept = value;
                    item->setlinkName(value);
                    item->setFolder(folder);
                    item->setCatalog("import");

                    emit sendCount(i);
                }
            }

            if(value != exept)
            {
                item->setFolder(folder);
                item->setlinkName(value);
                item->setCatalog("import");
                addItem(item);

                emit sendCount(i);
            }
        }

        pos = 0;
        while((pos = rx_link.indexIn(QString::fromUtf8(arr), pos)) != -1){
            pos += rx_link.matchedLength();
            value = rx_link.cap(0);
            item->setlinkReal(value);
            item->setDate(QDateTime::currentDateTime());

            //Добавляем в базу данных, полученный айтем
            import_to_db_item(item);
        }
    }

    file.close();

    items->clear();
    selectAll();
    fillStorage();
    sendStorage();
}

//Слот запускается из конструктора MainWindow
//По другому не работает
void Model::acceptStorage()
{
    emit sendStorage(s);
}

void Model::acceptParents(Catalog *catalog, Folder *folder)
{
    this->currentCatalog = catalog;
    this->currentFolder = folder;

    selectByFolderName(folder->Name());
}


void Model::accept_link_item_selected(QModelIndex index)
{
    this->currentIndex = index;
}


/************************************************/
// События в представлении
/************************************************/
void Model::editItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QVariant linkId = data(index(I.row(), 0), Qt::DisplayRole);
    QString linkName = data(index(I.row(), 3), Qt::DisplayRole).toString();
    beginResetModel();

    for(int i = 0; i < s->getCount(); i++)
    {
        Catalog *catalog = s->getCatalogById(i);
        for(int j = 0; j < catalog->getCount(); j++)
        {
            Folder *folder = catalog->getFolder(j);
            for(int l = 0; l < folder->Count(); l++)
            {
                Link *currentLink = folder->getLinkById(l);
                if(!currentLink->isNew){
                    if(currentLink->Id() == linkId)
                    {
                        LinkDialog ld(currentLink, parent);
                        ld.exec();
                        currentLink->isEdited = true;
                        currentLink->isMoved = false;
                        selectByFolderName(currentFolder->Name());
                        endResetModel();
                    }
                } else {
                    if(currentLink->linkName() == linkName)
                    {
                        LinkDialog ld(currentLink, parent);
                        ld.exec();
                        currentLink->isMoved = false;
                        currentLink->isEdited = true;
                        selectByFolderName(currentFolder->Name());
                        endResetModel();
                    }
                }
            }
        }
    }
}

void Model::newItem()
{
    Link *L = new Link();
    L->isNew = true;
    L->setDate(QDateTime::currentDateTime());
    currentFolder->addLink(L);    

    LinkDialog ld(L);
    ld.exec();
    selectByFolderName(currentFolder->Name());
}

void Model::delItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QVariant linkId = data(index(I.row(), 0), Qt::DisplayRole);
    QString linkName = data(index(I.row(), 3), Qt::DisplayRole).toString();
    beginResetModel();

    for(int i = 0; i < s->getCount(); i++)
    {
        Catalog *catalog = s->getCatalogById(i);
        for(int j = 0; j < catalog->getCount(); j++)
        {
            Folder *folder = catalog->getFolder(j);
            for(int l = 0; l < folder->Count(); l++)
            {
                Link *currentLink = folder->getLinkById(l);
                if(!currentLink->isNew){
                    if(currentLink->Id() == linkId)
                    {
                        if(!currentLink->isDeleted){
                            folder->markDeletedLinkById(l);
                            selectByFolderName(currentFolder->Name());
                            endResetModel();
                            return;
                        } else {
                            folder->markRestoredLinkById(l);
                            selectByFolderName(currentFolder->Name());
                            endResetModel();
                            return;
                        }
                    }
                } else {
                    if(currentLink->linkName() == linkName)
                    {
                        folder->deleteLink(currentLink);
                        selectByFolderName(currentFolder->Name());
                        endResetModel();
                        return;
                    }
                }
            }
        }
    }

    return;
}

void Model::openItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString linkName = data(index(I.row(), 3), Qt::DisplayRole).toString();
    Link *link = currentFolder->getLinkByName(linkName);
    link->isOpened = true;//перемещением мы отредактировали ссылку

    QString linkReal = data(index(I.row(), 4), Qt::DisplayRole).toString();

    if (linkReal == "") return;
    else {
        QDesktopServices::openUrl(QUrl(linkReal));
    }

    beginResetModel();
    selectByFolderName(currentFolder->Name());
    endResetModel();
}

void Model::moveItem(const QModelIndex &I, QWidget *parent)
{
    Q_UNUSED(parent)

    QString linkName = data(index(I.row(), 3), Qt::DisplayRole).toString();
    Link *link = currentFolder->getLinkByName(linkName);
    link->isMoved = true;//перемещением мы отредактировали ссылку

    Mover *m = new Mover(s, currentCatalog, currentFolder, link, true);
    m->exec();

    beginResetModel();
    selectByFolderName(currentFolder->Name());
    endResetModel();
}



/************************************************/
// Блок виртуальных методов модели
/************************************************/
int Model::rowCount(const QModelIndex &parent) const
{
    //Если родитель существует, то кол-во строк 0
    if(!parent.isValid()){
        return items->count();
    } else {
        return 0;
    }
}

int Model::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return 5;
    } else {
        return 0;
    }
}

QVariant Model::data(const QModelIndex &I, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:       return dataDisplay(I);
    case Qt::FontRole:          return dataFont(I);
    case Qt::ForegroundRole:    return dataForeground(I);
    case Qt::TextAlignmentRole: return dataTextAlignment(I);
    case Qt::ToolTipRole:       return dataToolTip(I);
    default: return QVariant();
    }
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Для горизонтальных заголовков оставляем данные по умолчанию
    if(orientation != Qt::Horizontal)
    {
        return QAbstractItemModel::headerData(section,orientation,role);
    }

    switch (role)
    {
    case Qt::DisplayRole:
        switch (section)
        {
        case 0: return tr("Number");
        case 1: return tr("Catalog");
        case 2: return tr("Folder");
        case 3: return tr("Name");
        case 4: return tr("Link");
        default: return QVariant();
        }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignBaseline | Qt::AlignHCenter);
    case Qt::FontRole:
    {
        QFont F("MS Shell Dlg 2", 11, QFont::Bold, false);
        return F;
    }
    default: return QVariant();
    }
}

QVariant Model::dataDisplay(const QModelIndex &I) const
{
    Data *D = items->at(I.row());

    switch (I.column()) {
    case 0: return D->Id();
    case 1: return D->Catalog();
    case 2: return D->Folder();
    case 3: return D->linkName();
    case 4: return D->linkReal();
    }

    return QVariant();
}

QVariant Model::dataForeground(const QModelIndex &I) const
{
    Data *D = items->at(I.row());
    if(!D) return QVariant();

    QColor Result;

    if(!D->isDeleted) Result.setAlphaF(1);//неактивный побледнеет
    if(D->isNew) Result = QColor("green");
    if(D->isEdited) Result = QColor("blue");
    if(D->isMoved) Result = QColor("red");
    if(D->isOpened) Result = QColor("purple");

    return Result;
}

QVariant Model::dataTextAlignment(const QModelIndex &I) const
{
    Q_UNUSED(I)
//    int Result = Qt::AlignVCenter;
//    Result |= Qt::AlignLeft;
//    return Result;
    return QVariant();
}

QVariant Model::dataToolTip(const QModelIndex &I) const
{
    Data *D = items->at(I.row());
    if(!D) return QVariant();

    //Работает в табличном представлении
//    switch (I.column()) {
    //Сработает если дата истечения валидная
//    case 2: {
        if(!D->Date().isValid())return QVariant();
        return QString("Дата создания: %1").arg(D->Date().toString("dd.MM.yyyy"));
        //return D->Date().toLocalTime();
//    }
//    default: return QVariant();
//    }
}

Qt::ItemFlags Model::flags(const QModelIndex &) const
{
    //    Qt::ItemFlags Result = Qt::ItemIsEnabled | Qt::ItemIsSelectable |
    //            Qt::ItemIsEditable;
    //    if(index.column() != 0)
    //        Result = Result | Qt::ItemIsEditable;//редактируем колонки, кроме 0

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable |
            Qt::ItemIsEditable;
}

QVariant Model::dataFont(const QModelIndex &I) const
{
    Data *D = items->at(I.row());
    if(!D) return QVariant();

    QFont F;
    if(D->isDeleted) {
        F.setStrikeOut(true);
    } else {
        F.setStrikeOut(false);
    }

    //    if(D->isEdited) F.setItalic(true);
    return F;
}










