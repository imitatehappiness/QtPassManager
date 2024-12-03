#include "grouppageform.h"
#include "qmessagebox.h"
#include "ui_grouppageform.h"
#include "types.h"

GroupPageForm::GroupPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupPageForm)
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

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &GroupPageForm::searchTextChanged);
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

    if (ui->frame_4->layout()) {
        delete ui->frame_4->layout();
    }
    ui->frame_4->setLayout(mainLayout);
}

GroupPageForm::~GroupPageForm()
{
    delete ui;
}

void GroupPageForm::setGroupInfo(GroupInfo* groupInfo){
    mGroupInfo = groupInfo;
    updateForm();
}

int GroupPageForm::getId(){
    return mGroupInfo->getId();
}

void GroupPageForm::updateForm(){
    ui->l_title->setText(mGroupInfo->getTitle());
    if (mGroupInfo->getTitle().size() > 0){
        ui->icon->setText(QString(mGroupInfo->getTitle().at(0)).toUpper());
    }

    for (int i = mMainLayout->count() - 1; i >= 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    QVector<AccountInfo*> accounts = mGroupInfo->getAccounts();
    std::sort(accounts.begin(), accounts.end(), [](AccountInfo *a, AccountInfo *b) {
        return a->getFavorite() > b->getFavorite();
    });

    for (auto &info : accounts) {
        addAccountBlock(*info);
    }

    updateStyle();

    mMainLayout->addStretch();
}

void GroupPageForm::addAccountBlock(AccountInfo &info){
    auto *accountInfoPtr = new AccountInfo(info);
    auto *item = new ItemBlock(nullptr, accountInfoPtr);

    mMainLayout->addWidget(item);
    mItemBlocksArray.append(item);

    connect(item, &ItemBlock::openPage, this, &GroupPageForm::openAccountPage);
}

void GroupPageForm::searchTextChanged(const QString &searchText){
    for (int i = mMainLayout->count() - 1; i >= 0; --i) {
        QLayoutItem *child = mMainLayout->takeAt(i);
        if (child) {
            delete child->widget();
            delete child;
        }
    }

    for (auto &info : mGroupInfo->getAccounts()) {
        if (searchText.isEmpty() || info->getTitle().contains(searchText, Qt::CaseInsensitive)) {
            addAccountBlock(*info);
        }
    }

    mMainLayout->addStretch();
}

void GroupPageForm::openAccountPage(int id, PageType type){
    emit openPage(id, type);
}


void GroupPageForm::on_pB_close_clicked(){
    emit closePage(PageType::group);
}

void GroupPageForm::updateStyle() {
    QString style =
        "QFrame{"
           "background-color: #202020; "
           "border-radius: 5px; "
        "}"

        "QFrame#f_block{"
           "background-color: " + mGroupInfo->getBgColor().name() + "; "
           "border-radius: 5px; "
        "}"
        "QLabel{"
            "background-color: " + mGroupInfo->getBgColor().name() + "; "
        "}"
        "QPushButton#pB_close{"
           "background-color: " + mGroupInfo->getBgColor().name() + "; "
           "border-radius: 5px; "
           "border: none ; "
        "}"
        "QLabel#icon{"
           "background-color: " + mGroupInfo->getIconColor().name() + "; "
           "border-radius: 5px; "
        "}";

    setStyleSheet(style);
}

void GroupPageForm::on_pB_edit_clicked(){
    emit edit(mGroupInfo->getId());
}

