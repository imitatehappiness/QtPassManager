#pragma once

#include <QWidget>

#include "account_info.h"

namespace Ui {
class ItemBlock;
}

class ItemBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ItemBlock(QWidget *parent = nullptr);
    ItemBlock(QWidget *parent = nullptr, AccountInfo* accountInfo = nullptr);
    ~ItemBlock();
public slots:
    void updateStyle();
private slots:
    void on_pB_open_clicked();
signals:
    void openAccountPage(int id);
private:
    Ui::ItemBlock *ui;
    AccountInfo* mAccountInfo;
};

