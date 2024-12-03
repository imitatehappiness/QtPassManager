#pragma once

#include "account_info.h"
#include "types.h"
#include <QWidget>

class PopUp;

namespace Ui {
class AccountPageForm;
}

class AccountPageForm : public QWidget
{
    Q_OBJECT

public:
    explicit AccountPageForm(QWidget *parent = nullptr);
    ~AccountPageForm();
    void updateForm(AccountInfo* accountInfo);
private:
    void updateStyle();
    QColor getColor();
private slots:
    void on_pB_close_clicked();
    void on_pB_showPass_clicked();
    void on_pB_edit_pass_clicked();
    void on_pB_edit_login_clicked();
    void on_pB_edit_title_clicked();
    void on_pB_copy_title_clicked();
    void on_pB_copy_log_clicked();
    void on_pB_copy_pass_clicked();
    void on_cB_favorite_stateChanged(int arg1);
    void on_pB_remove_clicked();
    void on_lE_title_textChanged(const QString &arg1);
    void on_lE_login_textChanged(const QString &arg1);
    void on_pB_editBgColor_clicked();
    void on_pB_resetBgColor_clicked();
    void on_pB_resetIconColor_clicked();
    void on_pB_editIconColor_clicked();
signals:
    void closePage(PageType type);
    void showNotification(QString message);
    void updateBlock(AccountInfo *info);
    void removeBlock(int id, PageType type);
private:
    Ui::AccountPageForm *ui;
    AccountInfo* mAccountInfo;
};

