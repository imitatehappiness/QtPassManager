#include "creategroupform.h"
#include "groupaccountform.h"
#include "qmessagebox.h"
#include "ui_creategroupform.h"
#include "types.h"

CreateGroupForm::CreateGroupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroupForm)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 0);

    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    mSearchLineEdit = new QLineEdit(this);
    mSearchLineEdit->setPlaceholderText(tr("Search..."));
    mSearchLineEdit->setStyleSheet("background-color: #eee; color: rgb(24, 24, 24);");

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &CreateGroupForm::searchTextChanged);
    searchLayout->addWidget(mSearchLineEdit);
    searchWidget->setLayout(searchLayout);

    mainLayout->addWidget(searchWidget);

    QWidget *groupListWidget = new QWidget(this);
    groupListWidget->setObjectName("groupList");
    mMainLayout = new QVBoxLayout(groupListWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);

    ui->scrollArea->setWidget(groupListWidget);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet("background-color: rgb(20,20,20); border-radius: 5px;");

    QWidget *scrollContainer = new QWidget(this);
    QVBoxLayout *scrollContainerLayout = new QVBoxLayout(scrollContainer);
    scrollContainerLayout->setContentsMargins(0, 0, 0, 0);
    scrollContainerLayout->addWidget(ui->scrollArea);

    mainLayout->addWidget(scrollContainer);

    if (ui->f_accounts_list->layout()) {
        delete ui->f_accounts_list->layout();
    }
    ui->f_accounts_list->setLayout(mainLayout);

    ui->icon->setStyleSheet("border-radius: 5px; background-color: " + bgIconGroupColorName);
}

CreateGroupForm::~CreateGroupForm(){
    delete ui;
}

void CreateGroupForm::clearFields(){
    ui->l_title->clear();
    ui->lE_title->clear();
    ui->icon->clear();
}

void CreateGroupForm::setAccounts(QVector<AccountInfo*> accounts){
    mAccounts = accounts;

    for (int i = mMainLayout->count() - 1; i >= 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    for (auto& account : mAccounts){
        auto *item = new GroupAccountForm();
        item->setInfo(account);
        mMainLayout->addWidget(item);
    }

    mMainLayout->addStretch();
}

void CreateGroupForm::on_pB_create_clicked(){

    QString title;
    title = ui->lE_title->text();

    if (title.size() == 0)
    {
        QMessageBox msgBox;

        msgBox.setStyleSheet(msgBoxStyle);

        msgBox.setWindowTitle("Information");
        msgBox.setText("Fill in all the fields");
        msgBox.setWindowIcon(QIcon(":/recources/icons/icon.png"));
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok){
            return;
        }
    }

    QVector<int> accounts;

    for (int i = 0; i < mMainLayout->count(); ++i) {
        QLayoutItem *item = mMainLayout->itemAt(i);
        if (item) {
            GroupAccountForm *accountForm = qobject_cast<GroupAccountForm*>(item->widget());
            if (accountForm && accountForm->isChecked()) {
                accounts.append(accountForm->getId());
            }
        }
    }

    emit createGroup(ui->lE_title->text(), accounts);
}

void CreateGroupForm::on_pB_cancel_clicked(){
    emit cancel();
}

void CreateGroupForm::on_lE_title_textChanged(const QString &arg1){
    ui->l_title->setText(arg1);
    if (arg1.size() > 0){
        ui->icon->setText(QString(arg1.at(0)).toUpper());
    }else{
        ui->icon->setText("");
    }
}

void CreateGroupForm::searchTextChanged(const QString &searchText){
    for (int i = mMainLayout->count() - 1; i >= 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    for (auto &account : mAccounts) {
        if (searchText.isEmpty() || account->getTitle().contains(searchText, Qt::CaseInsensitive)) {
            auto *item = new GroupAccountForm();
            item->setInfo(account);
            mMainLayout->addWidget(item);
        }
    }

    mMainLayout->addStretch();
}

