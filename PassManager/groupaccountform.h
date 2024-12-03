#pragma once

#include "account_info.h"
#include <QWidget>

namespace Ui {
class GroupAccountForm;
}

class GroupAccountForm : public QWidget
{
    Q_OBJECT

public:
    explicit GroupAccountForm(QWidget *parent = nullptr);
    ~GroupAccountForm();

    void setInfo(AccountInfo* info);
    AccountInfo* getInfo();
    int getId();
    bool isChecked();
    void setChecked(Qt::CheckState state);
private:
    Ui::GroupAccountForm *ui;
    AccountInfo* mAccountInfo;
};

