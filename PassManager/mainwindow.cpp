#include "mainwindow.h"
#include "popup.h"
#include "qevent.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QPushButton>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QJsonDocument>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStandardPaths>
#include <QDir>

#include "types.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPageType = PageType::main_page;
    this->statusBar()->setSizeGripEnabled(false);

    mStackedWidget = new QStackedWidget(this);
    setCentralWidget(mStackedWidget);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    QWidget *searchAndSelectWidget = new QWidget(this);
    QHBoxLayout *searchAndSelectLayout = new QHBoxLayout(searchAndSelectWidget);
    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setPlaceholderText(tr("Search..."));

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchTextChanged);
    searchAndSelectLayout->addWidget(mSearchLineEdit);

    mTypeSelect = new QComboBox(this);
    mTypeSelect->addItem(tr("All"));
    mTypeSelect->addItem(tr("Accounts"));
    mTypeSelect->addItem(tr("Groups"));

    connect(mTypeSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeSelectChanged(int)));
    searchAndSelectLayout->addWidget(mTypeSelect);

    mainLayout->addWidget(searchAndSelectWidget);

    QWidget *accountListWidget = new QWidget(this);
    accountListWidget->setObjectName("accountList");
    mMainLayout = new QVBoxLayout(accountListWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    ui->scrollArea->setWidget(accountListWidget);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet("background-color: rgb(20, 20, 20); border-radius: 5px;");

    QWidget *scrollContainer = new QWidget(this);
    QVBoxLayout *scrollContainerLayout = new QVBoxLayout(scrollContainer);
    scrollContainerLayout->addWidget(ui->scrollArea);

    mainLayout->addWidget(scrollContainer);

    mainWidget->setLayout(mainLayout);
    mStackedWidget->addWidget(mainWidget);

    mAccountPageForm = new AccountPageForm();
    mGroupPageForm = new GroupPageForm();
    mCreateAccountForm = new CreateAccountForm();
    mCreateGroupForm = new CreateGroupForm();
    mEditGroupPageForm = new EditGroupPageForm();

    connectForms();

    mStackedWidget->addWidget(mAccountPageForm);
    mStackedWidget->addWidget(mCreateAccountForm);
    mStackedWidget->addWidget(mCreateGroupForm);
    mStackedWidget->addWidget(mGroupPageForm);
    mStackedWidget->addWidget(mEditGroupPageForm);

    readAccountsFromXml();
    readGroupsFromXml();
    updateMainForm();

    mStackedWidget->setCurrentIndex(0);

    mPopUp = new PopUp(this);

    initMenuBar();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::initMenuBar() {
    mMenuBar = new QMenuBar(this);
    setMenuBar(mMenuBar);

    QMenu *settingsMenu = mMenuBar->addMenu(tr("&Settings"));

    QAction *createAccountAction = settingsMenu->addAction(tr("&Create new account"));
    createAccountAction->setIcon(QIcon(":/recources/icons/create.png"));
    connect(createAccountAction, &QAction::triggered, this, &MainWindow::createNewAccount);

    QAction *createGroupAction = settingsMenu->addAction(tr("&Create new group"));
    createGroupAction->setIcon(QIcon(":/recources/icons/create.png"));
    connect(createGroupAction, &QAction::triggered, this, &MainWindow::createNewGroup);
}

void MainWindow::updateMainForm(ListType type) {

    QLayoutItem *child;
    while ((child = mMainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    mItemBlocksArray.clear();

    switch (type) {
        case ListType::accounts:
            std::sort(mAccountInfoArray.begin(), mAccountInfoArray.end(), [](AccountInfo *a, AccountInfo *b) {
                return a->getFavorite() > b->getFavorite();
            });

            for (auto &info : mAccountInfoArray) {
                addAccountBlock(*info);
            }
            break;
        case ListType::groups:
            std::sort(mGroupInfoArray.begin(), mGroupInfoArray.end(), [](GroupInfo *a, GroupInfo *b) {
                return a->getFavorite() > b->getFavorite();
            });

            for (auto &info : mGroupInfoArray) {
                addGroupBlock(*info);
            }
            break;
        default:
            std::sort(mGroupInfoArray.begin(), mGroupInfoArray.end(), [](GroupInfo *a, GroupInfo *b) {
                return a->getFavorite() > b->getFavorite();
            });

            for (auto &info : mGroupInfoArray) {
                addGroupBlock(*info);
            }

            std::sort(mAccountInfoArray.begin(), mAccountInfoArray.end(), [](AccountInfo *a, AccountInfo *b) {
                return a->getFavorite() > b->getFavorite();
            });

            for (auto &info : mAccountInfoArray) {
                addAccountBlock(*info);
            }
            break;
    }

    mMainLayout->addStretch();
}

void MainWindow::addGroupBlock(const GroupInfo &info){
    GroupInfo *groupInfoPtr = new GroupInfo(info);
    ItemBlock *item = new ItemBlock(nullptr, groupInfoPtr);

    mMainLayout->addWidget(item);
    mItemBlocksArray.append(item);

    connect(item, &ItemBlock::openPage, this, &MainWindow::openPage);
}

void MainWindow::addAccountBlock(const AccountInfo &info) {
    auto *accountInfoPtr = new AccountInfo(info);
    auto *item = new ItemBlock(nullptr, accountInfoPtr);

    mMainLayout->addWidget(item);
    mItemBlocksArray.append(item);

    connect(item, &ItemBlock::openPage, this, &MainWindow::openPage);
}

void MainWindow::openPage(int id, PageType type) {
    if (type == PageType::account){

        if (mPageType != PageType::group) {
            mPageType = PageType::account;
        }

        for (auto &info : mAccountInfoArray) {
            if (info->getId() == id) {
                mAccountPageForm->updateForm(info);
                break;
            }
        }
        mStackedWidget->setCurrentIndex(1);
    }else{

        QVector<AccountInfo*> accounts;
        for(auto& group : mGroupInfoArray){
            if(id == group->getId()){
                mGroupPageForm->setGroupInfo(group);
                mGroupPageForm->updateForm();
                break;
            }
        }
        mPageType = PageType::group;
        mStackedWidget->setCurrentIndex(4);
    }
}

void MainWindow::closePage(PageType type) {
    if (mPageType == PageType::group && type == PageType::account){
        mPageType = PageType::group;
        mStackedWidget->setCurrentIndex(4);
    }else if (type == PageType::edit_group){
        mPageType = PageType::group;
        mStackedWidget->setCurrentIndex(4);
    }else{
        mPageType = PageType::main_page;
        mStackedWidget->setCurrentIndex(0);
        mSearchLineEdit->clear();
    }

}

void MainWindow::createNewAccount() {
    mStackedWidget->setCurrentIndex(2);
    mCreateAccountForm->clearFields();
}

void MainWindow::createNewGroup(){
    mStackedWidget->setCurrentIndex(3);
    mCreateGroupForm->clearFields();
    mCreateGroupForm->setAccounts(mAccountInfoArray);
}

void MainWindow::createAccount(QString password, QString login, QString title){
    mAccountInfoArray.append(new AccountInfo(mAccountInfoArray.size() + 1, login, password, title));
    updateMainForm();
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
    writeAccountsToXml();
}

void MainWindow::createGroup(QString title, QVector<int> accounts){
    QVector<AccountInfo*> resAccounts;
    for(auto& tempAccount : mAccountInfoArray){
        for (auto& id : accounts){
            if (id == tempAccount->getId()){
                resAccounts.append(tempAccount);
            }
        }
    }

    mGroupInfoArray.append(new GroupInfo(mGroupInfoArray.size() + 1, resAccounts, title));
    updateMainForm();
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
    writeGroupsToXml();
}

void MainWindow::openMainPage(){
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
}

void MainWindow::editGroup(int id){
    for (auto& info : mGroupInfoArray){
        if (info->getId() == id){
            mEditGroupPageForm->updateForm(info);
            mEditGroupPageForm->setAccounts(mAccountInfoArray);
        }
    }

    mStackedWidget->setCurrentIndex(5);
}

void MainWindow::onTypeSelectChanged(int index){
    updateMainForm(ListType(index));
    mSearchLineEdit->setText("");
}

void MainWindow::searchTextChanged(const QString &searchText) {
    for (int i = mMainLayout->count() - 1; i >= 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    for (auto &info : mGroupInfoArray) {
        if (searchText.isEmpty() || info->getTitle().contains(searchText, Qt::CaseInsensitive)) {
            addGroupBlock(*info);
        }
    }

    for (auto &info : mAccountInfoArray) {
        if (searchText.isEmpty() || info->getTitle().contains(searchText, Qt::CaseInsensitive)) {
            addAccountBlock(*info);
        }
    }

    mMainLayout->addStretch();
}

void MainWindow::removeBlock(int id, PageType type) {
    if (type == PageType::account){
        if (mPageType == PageType::account){
            for (int i = 0; i < mAccountInfoArray.size(); ++i) {
                if (mAccountInfoArray[i]->getId() == id) {
                    delete mAccountInfoArray[i];
                    mAccountInfoArray.removeAt(i);
                    break;
                }
            }

            for (auto& group : mGroupInfoArray){
                group->removeAccount(id);
                mGroupPageForm->setGroupInfo(group);
                mGroupPageForm->updateForm();
            }

        }else if (mPageType == PageType::group){

            for (auto& group : mGroupInfoArray){
                group->removeAccount(id);
                mGroupPageForm->setGroupInfo(group);
                mGroupPageForm->updateForm();
            }

        }

        writeGroupsToXml();
        writeAccountsToXml();

        updateMainForm();

    }else if (type == PageType::group){

        for (int i = 0; i < mGroupInfoArray.size(); ++i) {
            if (mGroupInfoArray[i]->getId() == id) {
                delete mGroupInfoArray[i];
                mGroupInfoArray.removeAt(i);
                break;
            }
        }

        updateMainForm();
        writeGroupsToXml();
    }

}
void MainWindow::updateAccountInfo(AccountInfo *info) {

    for (auto &curInfo : mAccountInfoArray) {
        if (curInfo->getId() == info->getId()) {
            *curInfo = *info;
            updateMainForm();
            break;
        }
    }


    for (auto &curInfo : mGroupInfoArray) {
        QVector<AccountInfo*> accounts = curInfo->getAccounts();

        for(int i = 0; i < accounts.size(); i++){
            if (accounts[i]->getId() == info->getId()){
                accounts[i] = info;
                curInfo->setAccounts(accounts);
                mGroupPageForm->setGroupInfo(curInfo);
                mGroupPageForm->updateForm();
                updateMainForm();
                writeGroupsToXml();
                break;
            }
        }
    }
    writeAccountsToXml();

}

void MainWindow::updateGroupInfo(GroupInfo *info){
    for (auto &curInfo : mGroupInfoArray) {
        if (curInfo->getId() == info->getId()) {
            *curInfo = *info;
            mGroupPageForm->setGroupInfo(curInfo);
            mGroupPageForm->updateForm();
            updateMainForm();
            break;
        }
    }

    writeGroupsToXml();
}

void MainWindow::showNotification(QString message) {
    mPopUp->setPopupText(message);
    mPopUp->show();
}

void MainWindow::connectForms() {
    connect(mAccountPageForm, &AccountPageForm::closePage, this, &MainWindow::closePage);
    connect(mAccountPageForm, &AccountPageForm::showNotification, this, &MainWindow::showNotification);
    connect(mAccountPageForm, &AccountPageForm::updateBlock, this, &MainWindow::updateAccountInfo);
    connect(mAccountPageForm, &AccountPageForm::removeBlock, this, &MainWindow::removeBlock);

    connect(mCreateAccountForm, &CreateAccountForm::createAccount, this, &MainWindow::createAccount);
    connect(mCreateAccountForm, &CreateAccountForm::cancel, this, &MainWindow::openMainPage);

    connect(mCreateGroupForm, &CreateGroupForm::cancel, this, &MainWindow::openMainPage);
    connect(mCreateGroupForm, &CreateGroupForm::createGroup, this, &MainWindow::createGroup);

    connect(mGroupPageForm, &GroupPageForm::closePage, this, &MainWindow::closePage);
    connect(mGroupPageForm, &GroupPageForm::removeBlock, this, &MainWindow::removeBlock);
    connect(mGroupPageForm, &GroupPageForm::openPage, this, &MainWindow::openPage);
    connect(mGroupPageForm, &GroupPageForm::edit, this, &MainWindow::editGroup);

    connect(mEditGroupPageForm, &EditGroupPageForm::closePage, this, &MainWindow::closePage);
    connect(mEditGroupPageForm, &EditGroupPageForm::removeBlock, this, &MainWindow::removeBlock);
    connect(mEditGroupPageForm, &EditGroupPageForm::showNotification, this, &MainWindow::showNotification);
    connect(mEditGroupPageForm, &EditGroupPageForm::updateBlock, this, &MainWindow::updateGroupInfo);

}

void MainWindow::writeAccountsToXml(const QString &fileName) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir;
    if (!dir.exists(appDataPath)) {
        dir.mkpath(appDataPath);
    }

    QString filePath = appDataPath + "/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open or create file for writing:" << filePath;
        return;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("Accounts");

    for (auto &info : mAccountInfoArray) {
        writer.writeStartElement("Account");

        writer.writeAttribute("id", QString::number(info->getId()));
        writer.writeAttribute("login", info->getLogin());
        writer.writeAttribute("password", info->getPassword());
        writer.writeAttribute("title", info->getTitle());
        writer.writeAttribute("bGColor", info->getBgColor().name());
        writer.writeAttribute("iconColor", info->getIconColor().name());
        writer.writeAttribute("favorite", QString::number(info->getFavorite()));

        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
    qDebug() << "File written successfully at:" << filePath;
}


void MainWindow::readAccountsFromXml(const QString &fileName) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePath = appDataPath + "/" + fileName;
    qDebug() << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement() && reader.name() == "Accounts") {
        while (reader.readNextStartElement()) {
            if (reader.name() == "Account") {
                auto id = reader.attributes().value("id").toInt();
                auto login = reader.attributes().value("login").toString();
                auto password = reader.attributes().value("password").toString();
                auto title = reader.attributes().value("title").toString();
                auto bgColor = QColor(reader.attributes().value("bGColor").toString());
                auto textColor = QColor(reader.attributes().value("iconColor").toString());
                auto favorite = reader.attributes().value("favorite").toInt();

                mAccountInfoArray.append(new AccountInfo(id, login, password, title, bgColor, textColor, favorite));
            }
            reader.skipCurrentElement();
        }
    }

    file.close();
}

void MainWindow::writeGroupsToXml(const QString &fileName) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir;
    if (!dir.exists(appDataPath)) {
        dir.mkpath(appDataPath);
    }

    QString filePath = appDataPath + "/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open or create file for writing:" << filePath;
        return;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("Groups");

    for (const auto &group : mGroupInfoArray) {
        writer.writeStartElement("Group");

        writer.writeAttribute("id", QString::number(group->getId()));
        writer.writeAttribute("title", group->getTitle());
        writer.writeAttribute("bGColor", group->getBgColor().name());
        writer.writeAttribute("iconColor", group->getIconColor().name());
        writer.writeAttribute("favorite", QString::number(group->getFavorite()));

        for (const auto &account : group->getAccounts()) {
            writer.writeStartElement("Account");

            writer.writeAttribute("id", QString::number(account->getId()));
            writer.writeAttribute("login", account->getLogin());
            writer.writeAttribute("password", account->getPassword());
            writer.writeAttribute("title", account->getTitle());
            writer.writeAttribute("bGColor", account->getBgColor().name());
            writer.writeAttribute("iconColor", account->getIconColor().name());
            writer.writeAttribute("favorite", QString::number(account->getFavorite()));

            writer.writeEndElement();
        }

        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
}

void MainWindow::readGroupsFromXml(const QString &fileName) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePath = appDataPath + "/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << filePath;
        return;
    }

    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement() && reader.name() == "Groups") {
        while (reader.readNextStartElement()) {
            if (reader.name() == "Group") {
                auto id = reader.attributes().value("id").toInt();
                auto title = reader.attributes().value("title").toString();
                auto bgColor = QColor(reader.attributes().value("bGColor").toString());
                auto iconColor = QColor(reader.attributes().value("iconColor").toString());
                auto favorite = reader.attributes().value("favorite").toInt();

                QVector<AccountInfo*> accounts;

                while (reader.readNextStartElement()) {
                    if (reader.name() == "Account") {
                        auto accountId = reader.attributes().value("id").toInt();
                        auto login = reader.attributes().value("login").toString();
                        auto password = reader.attributes().value("password").toString();
                        auto accountTitle = reader.attributes().value("title").toString();
                        auto accountBgColor = QColor(reader.attributes().value("bGColor").toString());
                        auto accountIconColor = QColor(reader.attributes().value("iconColor").toString());
                        auto accountFavorite = reader.attributes().value("favorite").toInt();

                        accounts.append(new AccountInfo(accountId, login, password, accountTitle, accountBgColor, accountIconColor, accountFavorite));

                        reader.skipCurrentElement();
                    } else {
                        reader.skipCurrentElement();
                    }
                }

                mGroupInfoArray.append(new GroupInfo(id, accounts, title, bgColor, iconColor, favorite));
            } else {
                reader.skipCurrentElement();
            }
        }
    }

    file.close();
}
