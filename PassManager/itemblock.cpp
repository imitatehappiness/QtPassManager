#include "itemblock.h"
#include "group_info.h"
#include "ui_itemblock.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDebug>

ItemBlock::ItemBlock(QWidget *parent, AccountInfo* accountInfo) :
    QWidget(parent),
    ui(new Ui::ItemBlock),
    mAccountInfo(accountInfo)
{
    ui->setupUi(this);

    mType = PageType::account;
    ui->pB_favorite->hide();

    if (mAccountInfo->getTitle().size() > 0){
        ui->icon->setText(QString(accountInfo->getTitle().at(0)).toUpper());
    }

    ui->l_title->setText(accountInfo->getTitle());
    ui->l_login->setText(accountInfo->getLogin());
    updateStyle();
}

ItemBlock::ItemBlock(QWidget *parent, GroupInfo *groupInfo) :
    QWidget(parent),
    ui(new Ui::ItemBlock),
    mGroupInfo(groupInfo)
{
    ui->setupUi(this);

    mType = PageType::group;
    ui->pB_favorite->hide();

    if (mGroupInfo->getTitle().size() > 0){
        ui->icon->setText(QString(groupInfo->getTitle().at(0)).toUpper());
    }

    ui->l_title->setText(groupInfo->getTitle());
    ui->l_login->setText("");
    updateStyle();
}

ItemBlock::~ItemBlock() {
    delete ui;
}

void ItemBlock::updateStyle() {
    QString style;
    if (mType == PageType::account){
        if (mAccountInfo->getFavorite()){
            ui->pB_favorite->show();
        }

        style =
            "QFrame{"
               "background-color: " + mAccountInfo->getBgColor().name() + "; "
               "border-radius: 5px; "
            "}"
            "QPushButton{"
               "background-color: " + mAccountInfo->getBgColor().name() + "; "
               "border-radius: 5px; "
               "border: 1px solid " + mAccountInfo->getBgColor().name() + "; "
            "}"
            "QLabel#icon{"
               "background-color: " + mAccountInfo->getIconColor().name() + "; "

            "}";
    }else{
        if (mGroupInfo->getFavorite()){
            ui->pB_favorite->show();
        }

        style =
            "QFrame{"
               "background-color: " + mGroupInfo->getBgColor().name() + "; "
               "border-radius: 5px; "
            "}"
            "QPushButton{"
               "background-color: " + mGroupInfo->getBgColor().name() + "; "
               "border-radius: 5px; "
               "border: 1px solid " + mGroupInfo->getBgColor().name() + "; "
            "}"
            "QLabel#icon{"
               "background-color: " + mGroupInfo->getIconColor().name() + "; "

            "}";
    }

    ui->frame->setStyleSheet(style);
}

void ItemBlock::on_pB_open_clicked(){
    if (mType == PageType::account){
        emit openPage(mAccountInfo->getId(), mType);
    }
    else{
        emit openPage(mGroupInfo->getId(), mType);
    }
}

