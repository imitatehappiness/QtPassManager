#pragma once

#include "account_info.h"
#include "ui_createaccountform.h"
#include <QWidget>

namespace Ui {
class CreateGroupForm;
}

class CreateGroupForm : public QWidget
{
    Q_OBJECT
public:
    explicit CreateGroupForm(QWidget *parent = nullptr);
    ~CreateGroupForm();
    void clearFields();
    void setAccounts(QVector<AccountInfo*> accounts);
signals:
    void createGroup(QString title, QVector<int> accounts);
    void cancel();
private slots:
    void on_pB_create_clicked();
    void on_pB_cancel_clicked();
    void on_lE_title_textChanged(const QString &arg1);
    void searchTextChanged(const QString &searchText);
private:
    Ui::CreateGroupForm *ui;
    QVector<AccountInfo*> mAccounts;
    QLineEdit* mSearchLineEdit;
    QVBoxLayout* mMainLayout;
};
