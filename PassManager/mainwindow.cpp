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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->statusBar()->setSizeGripEnabled(false);
    initMenuBar();

    mStackedWidget = new QStackedWidget(this);
    setCentralWidget(mStackedWidget);

    QWidget *accountListWidget = new QWidget(this);
    accountListWidget->setObjectName("accountList");

    mMainLayout = new QVBoxLayout(accountListWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setPlaceholderText(tr("Search..."));
    mSearchLineEdit->setStyleSheet("background-color: #eee; color: rgb(24, 24, 24)");

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchTextChanged);
    mMainLayout->addWidget(mSearchLineEdit);

    ui->scrollArea->setWidget(accountListWidget);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet("background-color: rgb(20, 20, 20); border-radius: 5px;");

    QWidget *scrollContainer = new QWidget(this);
    QVBoxLayout *scrollContainerLayout = new QVBoxLayout(scrollContainer);
    scrollContainerLayout->addWidget(ui->scrollArea);
    mStackedWidget->addWidget(scrollContainer);

    mAccountPageForm = new AccountPageForm();
    mCreateAccountForm = new CreateAccountForm();

    connectForms();

    mStackedWidget->addWidget(mAccountPageForm);
    mStackedWidget->addWidget(mCreateAccountForm);

    readAccountsFromXml();
    updateAccountsForm();

    mStackedWidget->setCurrentIndex(0);

    mPopUp = new PopUp(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::initMenuBar() {
    QMenuBar *menuBar = new QMenuBar();
    setMenuBar(menuBar);

    QMenu *settingsMenu = menuBar->addMenu(tr("&Settings"));
    QAction *createAction = settingsMenu->addAction(tr("&Create new account"));
    createAction->setIcon(QIcon(":/recources/icons/create.png"));
    connect(createAction, &QAction::triggered, this, &MainWindow::createNewAccount);
}

void MainWindow::updateAccountsForm() {
    QLayoutItem *child;
    while ((child = mMainLayout->takeAt(1)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    mItemBlocksArray.clear();

    std::sort(mAccountInfoArray.begin(), mAccountInfoArray.end(), [](AccountInfo *a, AccountInfo *b) {
        return a->getFavorite() > b->getFavorite();
    });

    for (auto &info : mAccountInfoArray) {
        addVerticalBlock(*info);
    }

    mMainLayout->addStretch();
}

void MainWindow::addVerticalBlock(const AccountInfo &accountInfo) {
    auto *accountInfoPtr = new AccountInfo(accountInfo);  // Создаем копию объекта
    auto *item = new ItemBlock(nullptr, accountInfoPtr);

    mMainLayout->addWidget(item);
    mItemBlocksArray.append(item);

    connect(item, &ItemBlock::openAccountPage, this, &MainWindow::openAccountPage);
}

void MainWindow::openAccountPage(int id) {
    for (auto &info : mAccountInfoArray) {
        if (info->getId() == id) {
            mAccountPageForm->updateForm(info);
            break;
        }
    }
    mStackedWidget->setCurrentIndex(1);
}

void MainWindow::closeAccountPage() {
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
}

void MainWindow::createNewAccount() {
    mStackedWidget->setCurrentIndex(2);
}

void MainWindow::createAccount(QString password, QString login, QString title){
    mAccountInfoArray.append(new AccountInfo(mAccountInfoArray.size() + 1, login, password, title));
    updateAccountsForm();
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
    writeAccountsToXml();
}

void MainWindow::openMainPage(){
    mStackedWidget->setCurrentIndex(0);
    mSearchLineEdit->clear();
}

void MainWindow::searchTextChanged(const QString &searchText) {
    for (int i = mMainLayout->count() - 1; i > 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    for (auto &info : mAccountInfoArray) {
        if (searchText.isEmpty() || info->getTitle().contains(searchText, Qt::CaseInsensitive)) {
            addVerticalBlock(*info);
        }
    }

    mMainLayout->addStretch();
}

void MainWindow::removeBlock(int id) {
    for (int i = 0; i < mAccountInfoArray.size(); ++i) {
        if (mAccountInfoArray[i]->getId() == id) {
            delete mAccountInfoArray[i];
            mAccountInfoArray.removeAt(i);
            updateAccountsForm();
            break;
        }
    }

    writeAccountsToXml();
}
void MainWindow::updateBlock(AccountInfo *info) {
    for (auto &curInfo : mAccountInfoArray) {
        if (curInfo->getId() == info->getId()) {
            *curInfo = *info;
            updateAccountsForm();
            break;
        }
    }

    writeAccountsToXml();
}

void MainWindow::showNotification(QString message) {
    mPopUp->setPopupText(message);
    mPopUp->show();
}

void MainWindow::connectForms() {
    connect(mAccountPageForm, &AccountPageForm::closeAccountPage, this, &MainWindow::closeAccountPage);
    connect(mAccountPageForm, &AccountPageForm::showNotification, this, &MainWindow::showNotification);
    connect(mAccountPageForm, &AccountPageForm::updateBlock, this, &MainWindow::updateBlock);
    connect(mAccountPageForm, &AccountPageForm::removeBlock, this, &MainWindow::removeBlock);
    connect(mCreateAccountForm, &CreateAccountForm::createAccount, this, &MainWindow::createAccount);
    connect(mCreateAccountForm, &CreateAccountForm::cancel, this, &MainWindow::openMainPage);
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

    for (const auto &info : mAccountInfoArray) {
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
