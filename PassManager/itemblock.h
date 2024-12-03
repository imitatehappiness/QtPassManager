#pragma once

#include <QWidget>

#include "account_info.h"
#include "group_info.h"
#include "types.h"

namespace Ui {
class ItemBlock;
}

class ItemBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ItemBlock(QWidget *parent = nullptr);
    ItemBlock(QWidget *parent = nullptr, AccountInfo* accountInfo = nullptr);
    ItemBlock(QWidget *parent = nullptr, GroupInfo* groupInfo = nullptr);
    ~ItemBlock();
public slots:
    void updateStyle();
private slots:
    void on_pB_open_clicked();
signals:
    void openPage(int id, PageType type);
private:
    Ui::ItemBlock *ui;
    AccountInfo* mAccountInfo;
    GroupInfo* mGroupInfo;
    PageType mType;
};

