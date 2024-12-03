#include "editgrouppageform.h"
#include "groupaccountform.h"
#include "qclipboard.h"
#include "qcolordialog.h"
#include "qmessagebox.h"
#include "ui_editgrouppageform.h"

EditGroupPageForm::EditGroupPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditGroupPageForm)
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

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &EditGroupPageForm::searchTextChanged);
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
}


EditGroupPageForm::~EditGroupPageForm()
{
    delete ui;
}

void EditGroupPageForm::updateForm(GroupInfo* info){
    mGroupInfo = info;

    ui->l_title->setText(mGroupInfo->getTitle());

    ui->lE_title->setText(mGroupInfo->getTitle());

    mGroupInfo->getFavorite() ? ui->cB_favorite->setCheckState(Qt::Checked) : ui->cB_favorite->setCheckState(Qt::Unchecked);

    if (mGroupInfo->getTitle().size() > 0){
        ui->icon->setText(QString(mGroupInfo->getTitle().at(0)).toUpper());
    }

    updateStyle();

}

void EditGroupPageForm::updateStyle(){
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
           "border: 1px solid " + mGroupInfo->getBgColor().name() + "; "
        "}"
        "QLabel#icon{"
           "background-color: " + mGroupInfo->getIconColor().name() + "; "
        "}";

    setStyleSheet(style);
}

QColor EditGroupPageForm::getColor(){
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle(tr("Select Color"));

    QColor selectedColor = colorDialog.getColor(Qt::white);

    return selectedColor;
}

void EditGroupPageForm::on_pB_remove_clicked()
{
    QMessageBox msgBox;

    msgBox.setStyleSheet(msgBoxStyle);

    msgBox.setWindowTitle("Confirmation of deletion");
    msgBox.setText("Are you sure you want to remove this group?");
    msgBox.setWindowIcon(QIcon(":/recources/icons/icon.png"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No) {
        return;
    }

    emit removeBlock(mGroupInfo->getId(), PageType::group);
    emit closePage(PageType::group);
    emit showNotification("Removed!");
}


void EditGroupPageForm::on_pB_edit_title_clicked(){
    bool isActive = ui->pB_edit_title->property("active").toBool();
    isActive = !isActive;
    ui->pB_edit_title->setProperty("active", isActive);

    if (isActive) {
        ui->pB_edit_title->setText("Save");
        ui->lE_title->setEnabled(true);
    } else {
        ui->pB_edit_title->setText("Edit");
        ui->lE_title->setEnabled(false);

        mGroupInfo->setTitle(ui->lE_title->text());
        ui->l_title->setText(ui->lE_title->text());

        if (mGroupInfo->getTitle().size() > 0){
            ui->icon->setText(QString(mGroupInfo->getTitle().at(0)).toUpper());
        }
    }


    emit updateBlock(mGroupInfo);
}

void EditGroupPageForm::on_lE_title_textChanged(const QString &arg1){
    ui->l_title->setText(arg1);
    if (arg1.size() > 0){
        ui->icon->setText(QString(arg1.at(0)).toUpper());
    }
}

void EditGroupPageForm::on_pB_copy_title_clicked(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lE_title->text());
}


void EditGroupPageForm::on_cB_favorite_stateChanged(int arg1){
    mGroupInfo->setFavorite(bool(arg1));
    bool(arg1) ?  ui->cB_favorite->setText("Remove from Favorites") : ui->cB_favorite->setText("Add to Favorites");

    emit updateBlock(mGroupInfo);
}


void EditGroupPageForm::on_pB_resetBgColor_clicked(){
    mGroupInfo->setBgColor(QColor(bgCardColorName));
    updateStyle();
    emit updateBlock(mGroupInfo);
}


void EditGroupPageForm::on_pB_resetIconColor_clicked(){
    mGroupInfo->setIconColor(QColor(bgIconGroupColorName));
    updateStyle();
    emit updateBlock(mGroupInfo);
}


void EditGroupPageForm::on_pB_editBgColor_clicked(){
    QColor selectedColor = getColor();
    if (selectedColor.isValid()) {
        mGroupInfo->setBgColor(selectedColor);
        updateStyle();
        emit updateBlock(mGroupInfo);
    }
}


void EditGroupPageForm::on_pB_editIconColor_clicked(){
    QColor selectedColor = getColor();
    if (selectedColor.isValid()) {
        mGroupInfo->setIconColor(selectedColor);
        updateStyle();
        emit updateBlock(mGroupInfo);
    }
}

void EditGroupPageForm::on_pB_close_clicked(){

    if (ui->pB_edit_title->property("active").toBool()){
        QMessageBox msgBox;

        msgBox.setStyleSheet(msgBoxStyle);

        msgBox.setWindowTitle("Confirmation");
        msgBox.setText("Not all parameters are saved");
        msgBox.setWindowIcon(QIcon(":/recources/icons/icon.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        if (msgBox.exec() == QMessageBox::Ok) {
            return;
        }
    }

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

    ui->pB_edit_title->setProperty("active", false);
    ui->pB_edit_title->setText("Edit");
    ui->lE_title->setEnabled(false);

    QVector<AccountInfo* > accounts;

    for (int i = 0; i < mMainLayout->count(); ++i) {
        QLayoutItem *item = mMainLayout->itemAt(i);
        if (item) {
            GroupAccountForm *accountForm = qobject_cast<GroupAccountForm*>(item->widget());
            if (accountForm && accountForm->isChecked()) {
                accounts.append(accountForm->getInfo());
            }
        }
    }

    mAccounts = accounts;
    mGroupInfo->setAccounts(accounts);

    emit updateBlock(mGroupInfo);
    emit closePage(PageType::edit_group);
    emit showNotification("Saved!");

}

void EditGroupPageForm::searchTextChanged(const QString &searchText){
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

void EditGroupPageForm::setAccounts(QVector<AccountInfo*> accounts){
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

        for (auto& groupAccount : mGroupInfo->getAccounts()){
            if(groupAccount->getId() == account->getId()){
                item->setChecked(Qt::Checked);
                break;
            }
        }

        mMainLayout->addWidget(item);

    }

    mMainLayout->addStretch();
}
