#pragma once
#include <QWidget>

struct AccountInfo
{
    AccountInfo(
            int id,
            const QString& login,
            const QString& password,
            const QString& title = "",
            QColor bgColor = QColor("#202020"),
            QColor iconColor = QColor("#4F2D8A"),
            bool favorite = false) :
        mId(id),
        mLogin(login),
        mPassword(password),
        mTitle(title),
        mBgColor(bgColor),
        mIconColor(iconColor),
        mFavotite(favorite)
    {
    }

    AccountInfo(const AccountInfo& other):
        mId(other.mId),
        mLogin(other.mLogin),
        mPassword(other.mPassword),
        mTitle(other.mTitle),
        mBgColor(other.mBgColor),
        mIconColor(other.mIconColor),
        mFavotite(other.mFavotite)
    {
    }

    void setLogin(const QString& login){
        this->mLogin = login;
    }

    void setPassword(const QString& password){
        this->mPassword = password;
    }

    void setTitle(const QString& title){
        this->mTitle = title;
    }

    void setBgColor(const QColor& color){
        this->mBgColor = color;
    }

    void setFavorite(bool favorite){
        this->mFavotite = favorite;
    }

    void setIconColor(const QColor& color){
        this->mIconColor = color;
    }

    int getId(){
        return this->mId;
    }

    QString getLogin(){
        return this->mLogin;
    }

    QString getPassword(){
        return this->mPassword;
    }

    QString getTitle(){
        return this->mTitle;
    }

    QColor getBgColor(){
        return this->mBgColor;
    }

    QColor getIconColor(){
        return this->mIconColor;
    }

    bool getFavorite(){
        return this->mFavotite;
    }

private:
    int mId;
    QString mLogin;
    QString mPassword;
    QString mTitle;
    QColor mBgColor;
    QColor mIconColor;
    bool mFavotite;
};
