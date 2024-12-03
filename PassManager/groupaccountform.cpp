#include "groupaccountform.h"
#include "ui_groupaccountform.h"

GroupAccountForm::GroupAccountForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupAccountForm)
{
    ui->setupUi(this);
}

GroupAccountForm::~GroupAccountForm(){
    delete ui;
}

void GroupAccountForm::setInfo(AccountInfo* info){
    mAccountInfo = info;

    ui->l_title->setText(mAccountInfo->getTitle());
    ui->l_login->setText(mAccountInfo->getLogin());
    if (mAccountInfo->getTitle().size() > 0){
        ui->icon->setText(QString(mAccountInfo->getTitle().at(0)).toUpper());
    }
}

AccountInfo *GroupAccountForm::getInfo(){
    return mAccountInfo;
}


int GroupAccountForm::getId(){
    return mAccountInfo->getId();
}

bool GroupAccountForm::isChecked(){
    return ui->checkBox->isChecked();
}

void GroupAccountForm::setChecked(Qt::CheckState state){
    ui->checkBox->setCheckState(state);
}
