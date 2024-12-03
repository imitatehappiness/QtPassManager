#include "accountpageform.h"
#include "popup.h"
#include "qdesktopwidget.h"
#include "ui_accountpageform.h"

#include <QClipboard>
#include <QMessageBox>
#include <QColorDialog>

AccountPageForm::AccountPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountPageForm)
{
    ui->setupUi(this);

}

AccountPageForm::~AccountPageForm(){
    delete ui;
}

void AccountPageForm::updateForm(AccountInfo *accountInfo){
    mAccountInfo = new AccountInfo(*accountInfo);

    ui->l_login->setText(mAccountInfo->getLogin());
    ui->l_title->setText(mAccountInfo->getTitle());

    ui->lE_pass->setText(mAccountInfo->getPassword());
    ui->lE_login->setText(mAccountInfo->getLogin());
    ui->lE_title->setText(mAccountInfo->getTitle());

    mAccountInfo->getFavorite() ? ui->cB_favorite->setCheckState(Qt::Checked) : ui->cB_favorite->setCheckState(Qt::Unchecked);

    if (mAccountInfo->getTitle().size() > 0){
        ui->icon->setText(QString(mAccountInfo->getTitle().at(0)).toUpper());
    }

    updateStyle();
}

void AccountPageForm::on_pB_close_clicked(){

    if (ui->pB_edit_pass->property("active").toBool() || ui->pB_edit_login->property("active").toBool() || ui->pB_edit_title->property("active").toBool())
    {
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

    QString password, login, title;
    password = ui->lE_pass->text();
    login = ui->lE_login->text();
    title = ui->lE_title->text();

    if (password.size() == 0 || login.size() == 0 || title.size() == 0)
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

    ui->pB_edit_pass->setProperty("active", false);
    ui->pB_edit_login->setProperty("active", false);
    ui->pB_edit_title->setProperty("active", false);

    ui->pB_edit_pass->setText("Edit");
    ui->pB_edit_login->setText("Edit");
    ui->pB_edit_title->setText("Edit");

    ui->lE_pass->setEnabled(false);
    ui->lE_login->setEnabled(false);
    ui->lE_title->setEnabled(false);

    emit closePage(PageType::account);
    emit showNotification("Saved!");
}

void AccountPageForm::on_pB_showPass_clicked(){
    if (!ui->pB_edit_pass->property("active").toBool())
    {
        bool isActive = ui->pB_showPass->property("active").toBool();

        isActive = !isActive;

        ui->pB_showPass->setProperty("active", isActive);

        if (isActive) {
            ui->pB_showPass->setIcon(QIcon(":/recources/icons/collapse_hide_light.png"));
            ui->lE_pass->setEchoMode(QLineEdit::Normal);
        } else {
            ui->pB_showPass->setIcon(QIcon(":/recources/icons/collapse_show_light.png"));
            ui->lE_pass->setEchoMode(QLineEdit::Password);

            emit updateBlock(mAccountInfo);
        }
    }
}

void AccountPageForm::on_pB_edit_pass_clicked() {
    bool isActive = ui->pB_edit_pass->property("active").toBool();
    isActive = !isActive;
    ui->pB_edit_pass->setProperty("active", isActive);

    if (isActive) {
        ui->pB_edit_pass->setText("Save");
        ui->lE_pass->setEchoMode(QLineEdit::Normal);
        ui->lE_pass->setEnabled(true);
    } else {
        ui->pB_edit_pass->setText("Edit");
        ui->lE_pass->setEchoMode(QLineEdit::Password);
        ui->lE_pass->setEnabled(false);

        mAccountInfo->setPassword(ui->lE_pass->text());

        emit updateBlock(mAccountInfo);
    }
}

void AccountPageForm::on_pB_edit_login_clicked() {
    bool isActive = ui->pB_edit_login->property("active").toBool();
    isActive = !isActive;
    ui->pB_edit_login->setProperty("active", isActive);

    if (isActive) {
        ui->pB_edit_login->setText("Save");
        ui->lE_login->setEnabled(true);
    } else {
        ui->pB_edit_login->setText("Edit");
        ui->lE_login->setEnabled(false);

        mAccountInfo->setLogin(ui->lE_login->text());
        ui->l_login->setText(ui->lE_login->text());

        emit updateBlock(mAccountInfo);
    }
}

void AccountPageForm::on_pB_edit_title_clicked() {
    bool isActive = ui->pB_edit_title->property("active").toBool();
    isActive = !isActive;
    ui->pB_edit_title->setProperty("active", isActive);

    if (isActive) {
        ui->pB_edit_title->setText("Save");
        ui->lE_title->setEnabled(true);
    } else {
        ui->pB_edit_title->setText("Edit");
        ui->lE_title->setEnabled(false);

        mAccountInfo->setTitle(ui->lE_title->text());
        ui->l_title->setText(ui->lE_title->text());

        if (mAccountInfo->getTitle().size() > 0){
            ui->icon->setText(QString(mAccountInfo->getTitle().at(0)).toUpper());
        }


        emit updateBlock(mAccountInfo);
    }
}

void AccountPageForm::on_pB_copy_title_clicked(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->l_title->text());
}

void AccountPageForm::on_pB_copy_log_clicked(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->l_login->text());
}

void AccountPageForm::on_pB_copy_pass_clicked(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lE_pass->text());
}

void AccountPageForm::on_cB_favorite_stateChanged(int arg1){
    mAccountInfo->setFavorite(bool(arg1));
    bool(arg1) ?  ui->cB_favorite->setText("Remove from Favorites") : ui->cB_favorite->setText("Add to Favorites");

    emit updateBlock(mAccountInfo);
}


void AccountPageForm::on_pB_remove_clicked(){
    QMessageBox msgBox;

    msgBox.setStyleSheet(msgBoxStyle);

    msgBox.setWindowTitle("Confirmation of deletion");
    msgBox.setText("Are you sure you want to remove this item?");
    msgBox.setWindowIcon(QIcon(":/recources/icons/icon.png"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No) {
        return;
    }

    emit removeBlock(mAccountInfo->getId(), PageType::account);
    emit closePage(PageType::account);
    emit showNotification("Removed!");
}


void AccountPageForm::on_lE_title_textChanged(const QString &arg1){
    ui->l_title->setText(arg1);
    if (arg1.size() > 0){
        ui->icon->setText(QString(arg1.at(0)).toUpper());
    }
}

void AccountPageForm::on_lE_login_textChanged(const QString &arg1){
    ui->l_login->setText(arg1);
}

void AccountPageForm::updateStyle() {
    QString style =
        "QFrame{"
           "background-color: #202020; "
           "border-radius: 5px; "
        "}"

        "QFrame#f_block{"
           "background-color: " + mAccountInfo->getBgColor().name() + "; "
           "border-radius: 5px; "
        "}"
        "QLabel{"
            "background-color: " + mAccountInfo->getBgColor().name() + "; "
        "}"
        "QPushButton#pB_close{"
           "background-color: " + mAccountInfo->getBgColor().name() + "; "
           "border-radius: 5px; "
           "border: 1px solid " + mAccountInfo->getBgColor().name() + "; "
        "}"
        "QLabel#icon{"
           "background-color: " + mAccountInfo->getIconColor().name() + "; "
           "border-radius: 5px; "
        "}";

    setStyleSheet(style);
}

QColor AccountPageForm::getColor(){
    QColorDialog colorDialog(this);
    colorDialog.setWindowTitle(tr("Select Color"));

    QColor selectedColor = colorDialog.getColor(Qt::white);

    return selectedColor;
}

void AccountPageForm::on_pB_editBgColor_clicked(){
    QColor selectedColor = getColor();
    if (selectedColor.isValid()) {
        mAccountInfo->setBgColor(selectedColor);
        updateStyle();
        emit updateBlock(mAccountInfo);
    }
}


void AccountPageForm::on_pB_resetBgColor_clicked(){
    mAccountInfo->setBgColor(QColor(bgCardColorName));
    updateStyle();
    emit updateBlock(mAccountInfo);
}


void AccountPageForm::on_pB_resetIconColor_clicked(){
    mAccountInfo->setIconColor(QColor(bgIconAccountColorName));
    updateStyle();
    emit updateBlock(mAccountInfo);
}

void AccountPageForm::on_pB_editIconColor_clicked(){
    QColor selectedColor = getColor();
    if (selectedColor.isValid()) {
        mAccountInfo->setIconColor(selectedColor);
        updateStyle();
        emit updateBlock(mAccountInfo);
    }
}

