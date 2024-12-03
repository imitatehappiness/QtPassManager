#pragma once

#include <QWidget>

enum PageType{
    main_page,
    account,
    group,
    edit_group
};

enum ListType{
    all,
    accounts,
    groups
};

const QString msgBoxStyle =
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

const QString bgCardColorName = "#202020";
const QString bgIconGroupColorName = "#648d1d";
const QString bgIconAccountColorName = "#4F2D8A";

