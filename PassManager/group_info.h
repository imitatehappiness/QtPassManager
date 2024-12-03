#pragma once
#include <QWidget>

#include "account_info.h"
#include "types.h"

struct GroupInfo
{
    GroupInfo(
            int id,
            QVector<AccountInfo*> accounts,
            const QString& title = "",
            QColor bgColor = QColor(bgCardColorName),
            QColor iconColor = QColor(bgIconGroupColorName),
            bool favorite = false) :
        mId(id),
        mTitle(title),
        mBgColor(bgColor),
        mIconColor(iconColor),
        mFavotite(favorite),
        mAccounts(accounts)
    {
    }

    GroupInfo(const GroupInfo& other):
        mId(other.mId),
        mTitle(other.mTitle),
        mBgColor(other.mBgColor),
        mIconColor(other.mIconColor),
        mFavotite(other.mFavotite),
        mAccounts(other.mAccounts)
    {
    }

    ~GroupInfo()
    {

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

    QVector<AccountInfo*> getAccounts(){
        return this->mAccounts;
    }

    void setAccounts(QVector<AccountInfo*> accounts){
        this->mAccounts = accounts;
    }

    void appendAccount(AccountInfo* mew_account) {

        for (const auto& account : mAccounts) {
            if (account->getId() == mew_account->getId()) {
                return;
            }
        }

        mAccounts.append(mew_account);
    }

    void removeAccount(int id) {
        for (int i = 0; i < mAccounts.size(); ++i) {
            if (mAccounts[i]->getId() == id) {
                mAccounts.remove(i);
                return;
            }
        }
    }

private:
    int mId;
    QString mTitle;
    QColor mBgColor;
    QColor mIconColor;
    bool mFavotite;
    QVector<AccountInfo*> mAccounts;
};
