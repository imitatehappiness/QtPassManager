#pragma once

#include "group_info.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "types.h"
#include <QWidget>

namespace Ui {
class EditGroupPageForm;
}

class EditGroupPageForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditGroupPageForm(QWidget *parent = nullptr);
    ~EditGroupPageForm();
    void updateForm(GroupInfo* info);
    void setAccounts(QVector<AccountInfo*> accounts);
private:
    void updateStyle();
    QColor getColor();
signals:
    void closePage(PageType type);
    void showNotification(QString message);
    void removeBlock(int id, PageType type);
    void updateBlock(GroupInfo *info);

private slots:
    void on_pB_remove_clicked();
    void on_pB_edit_title_clicked();
    void on_lE_title_textChanged(const QString &arg1);
    void on_pB_copy_title_clicked();
    void on_cB_favorite_stateChanged(int arg1);
    void on_pB_resetBgColor_clicked();
    void on_pB_resetIconColor_clicked();
    void on_pB_editBgColor_clicked();
    void on_pB_editIconColor_clicked();
    void on_pB_close_clicked();
    void searchTextChanged(const QString &searchText);

private:
    Ui::EditGroupPageForm *ui;
    GroupInfo* mGroupInfo;
    QVector<AccountInfo*> mAccounts;
    QLineEdit* mSearchLineEdit;
    QVBoxLayout* mMainLayout;
};

