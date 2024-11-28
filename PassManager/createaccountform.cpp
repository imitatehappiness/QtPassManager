#include "createaccountform.h"
#include "ui_createaccountform.h"

#include <QMessageBox>
#include <QRandomGenerator>

const  QString msgBoxStyle =
    "QMessageBox {"
    "   background-color: rgb(33, 33, 33);"
    "   color: #eee;"
    "}"
    "QPushButton {"
    "   background-color: rgb(24, 24, 24);"
    "   color: #fff;"
    "   border: 1px solid rgb(24, 24, 24);"
    "   border-radius: 5px;"
    "   padding: 5px 10px;"
    "   min-width: 40px;"
    "}"
    "QPushButton:hover {"
    "   background-color: rgb(60, 60, 60);"
    "   border: 1px solid rgb(60, 60, 60);"
    "}"
    "QPushButton:pressed {"
    "   border: 1px solid rgb(20, 20, 20);"
    "}";

CreateAccountForm::CreateAccountForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createAccountForm)
{
    ui->setupUi(this);
}

CreateAccountForm::~CreateAccountForm(){
    delete ui;
}

QString CreateAccountForm::generatePassword(int length) {
    QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    if (ui->cB_useNumber->checkState() == Qt::Checked){
        possibleCharacters += "0123456789";
    }
    if (ui->cB_userCpecialCharacters->checkState() == Qt::Checked){
        possibleCharacters += "!@#$%^&*()-_=+";
    }

    QString password;

    for (int i = 0; i < length; ++i){
        int index = QRandomGenerator::global()->bounded(possibleCharacters.size());
        password.append(possibleCharacters[index]);
    }

    return password;
}

void CreateAccountForm::clearFields(){
    ui->lE_login->clear();
    ui->lE_pass->clear();
    ui->lE_title->clear();
    ui->icon->clear();

    ui->cB_useNumber->setCheckState(Qt::Unchecked);
    ui->cB_userCpecialCharacters->setCheckState(Qt::Unchecked);
}


void CreateAccountForm::on_lE_title_textChanged(const QString &arg1){
    ui->l_title->setText(arg1);
    if (arg1.size() > 0){
        ui->icon->setText(QString(arg1.at(0)).toUpper());
    }
}

void CreateAccountForm::on_lE_login_textChanged(const QString &arg1){
    ui->l_login->setText(arg1);
}

void CreateAccountForm::on_pB_create_clicked(){
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

    emit createAccount(password, login, title);
}

void CreateAccountForm::on_pB_showPass_clicked(){
    bool isActive = ui->pB_showPass->property("active").toBool();

    isActive = !isActive;

    ui->pB_showPass->setProperty("active", isActive);

    if (isActive) {
        ui->pB_showPass->setIcon(QIcon(":/recources/icons/collapse_hide_light.png"));
        ui->lE_pass->setEchoMode(QLineEdit::Normal);
    } else {
        ui->pB_showPass->setIcon(QIcon(":/recources/icons/collapse_show_light.png"));
        ui->lE_pass->setEchoMode(QLineEdit::Password);
    }
}

void CreateAccountForm::on_pB_cancel_clicked(){
    emit cancel();
}

void CreateAccountForm::on_pB_generatePass_clicked(){
    ui->lE_pass->setText(generatePassword(ui->sB_passLength->value()));
}

