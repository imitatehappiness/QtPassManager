#pragma once

#include "qlineedit.h"
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QStackedWidget>

#include <itemblock.h>
#include <accountpageform.h>
#include <createaccountform.h>

class PopUp;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void updateAccountsForm();
    void initMenuBar();
    void addVerticalBlock(const AccountInfo& accountInfo);
    void movePopUp();
    void connectForms();
    void writeAccountsToXml(const QString &filePath = "accounts.xml");
    void readAccountsFromXml(const QString &filePath = "accounts.xml");
private slots:
    void openAccountPage(int id);
    void closeAccountPage();
    void showNotification(QString message);
    void searchTextChanged(const QString &arg1);
    void updateBlock(AccountInfo* info);
    void removeBlock(int id);
    void createNewAccount();
    void createAccount(QString password, QString login, QString title);
    void openMainPage();
private:
    Ui::MainWindow *ui;
    QMenuBar *mMenuBar;
    QVBoxLayout* mMainLayout;
    QVector<ItemBlock*> mItemBlocksArray;
    QVector<AccountInfo*> mAccountInfoArray;
    AccountPageForm* mAccountPageForm;
    QStackedWidget* mStackedWidget;
    PopUp *mPopUp;
    QLineEdit* mSearchLineEdit;
    CreateAccountForm* mCreateAccountForm;
};
