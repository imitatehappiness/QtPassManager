#include "itemblock.h"
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

    ui->pB_favorite->hide();

    if (mAccountInfo->getTitle().size() > 0){
        ui->icon->setText(QString(accountInfo->getTitle().at(0)).toUpper());
    }

    ui->l_title->setText(accountInfo->getTitle());
    ui->l_login->setText(accountInfo->getLogin());
    updateStyle();
}

ItemBlock::~ItemBlock() {
    delete ui;
    delete mAccountInfo;
}

void ItemBlock::updateStyle() {

    if (mAccountInfo->getFavorite())
    {
        ui->pB_favorite->show();
    }

    QString style =
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

    ui->frame->setStyleSheet(style);
}

void ItemBlock::on_pB_open_clicked(){
    emit openAccountPage(mAccountInfo->getId());
}

