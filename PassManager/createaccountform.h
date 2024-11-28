#pragma once

#include <QWidget>

namespace Ui {
class createAccountForm;
}

class CreateAccountForm : public QWidget
{
    Q_OBJECT
public:
    explicit CreateAccountForm(QWidget *parent = nullptr);
    ~CreateAccountForm();
private:
    QString generatePassword(int length);
signals:
    void createAccount(QString password, QString login, QString title);
    void cancel();
private slots:
    void on_lE_title_textChanged(const QString &arg1);
    void on_lE_login_textChanged(const QString &arg1);
    void on_pB_create_clicked();
    void on_pB_showPass_clicked();
    void on_pB_cancel_clicked();
    void on_pB_generatePass_clicked();
private:
    Ui::createAccountForm *ui;
};

