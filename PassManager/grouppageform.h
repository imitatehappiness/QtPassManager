#pragma once

#include "itemblock.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include <QWidget>

namespace Ui {
class GroupPageForm;
}

class GroupPageForm : public QWidget
{
    Q_OBJECT

public:
    explicit GroupPageForm(QWidget *parent = nullptr);
    ~GroupPageForm();

    void setGroupInfo(GroupInfo* groupInfo);
    int getId();
    void updateForm();
private:
    void addAccountBlock(AccountInfo &info);
    void updateStyle();
signals:
    void closePage(PageType type);
    void showNotification(QString message);
    void updateBlock(GroupInfo *info);
    void removeBlock(int id, PageType type);
    void openPage(int id, PageType type);
    void edit(int id);
private slots:
    void searchTextChanged(const QString &searchText);
    void openAccountPage(int id, PageType type);
    void on_pB_close_clicked();
    void on_pB_edit_clicked();
private:
    Ui::GroupPageForm *ui;
    QVBoxLayout* mMainLayout;
    QVector<ItemBlock*> mItemBlocksArray;
    QLineEdit* mSearchLineEdit;
    GroupInfo* mGroupInfo;
};

