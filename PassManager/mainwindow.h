#pragma once

#include "qlineedit.h"
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QStackedWidget>
#include <QComboBox>

#include <itemblock.h>
#include <accountpageform.h>
#include <createaccountform.h>
#include <creategroupform.h>
#include <grouppageform.h>
#include <editgrouppageform.h>
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
    void updateMainForm(ListType type = ListType::all);
    void initMenuBar();
    void addAccountBlock(const AccountInfo& info);
    void addGroupBlock(const GroupInfo& info);
    void movePopUp();
    void connectForms();
    void writeAccountsToXml(const QString &filePath = "accounts.xml");
    void readAccountsFromXml(const QString &filePath = "accounts.xml");
    void writeGroupsToXml(const QString &filePath = "groups.xml");
    void readGroupsFromXml(const QString &filePath = "groups.xml");
private slots:
    void openPage(int id, PageType type);
    void closePage(PageType type);
    void showNotification(QString message);
    void searchTextChanged(const QString &arg1);
    void updateAccountInfo(AccountInfo* info);
    void updateGroupInfo(GroupInfo* info);
    void removeBlock(int id, PageType type);
    void createNewAccount();
    void createNewGroup();
    void createAccount(QString password, QString login, QString title);
    void createGroup(QString title, QVector<int> accounts);
    void openMainPage();
    void editGroup(int id);
    void onTypeSelectChanged(int index);
private:
    Ui::MainWindow *ui;
    QMenuBar *mMenuBar;
    QVBoxLayout* mMainLayout;
    QVector<ItemBlock*> mItemBlocksArray;

    QVector<AccountInfo*> mAccountInfoArray;
    QVector<GroupInfo*> mGroupInfoArray;

    AccountPageForm* mAccountPageForm;
    GroupPageForm* mGroupPageForm;
    QStackedWidget* mStackedWidget;
    PopUp *mPopUp;
    QLineEdit* mSearchLineEdit;
    CreateAccountForm* mCreateAccountForm;
    CreateGroupForm* mCreateGroupForm;
    EditGroupPageForm* mEditGroupPageForm;
    PageType mPageType;

    QComboBox *mTypeSelect;
};
