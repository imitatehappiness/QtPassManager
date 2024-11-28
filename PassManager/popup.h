#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopUp : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;
public:
    explicit PopUp(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void setPopupText(const QString& text);
    void show();
private slots:
    void hideAnimation();
    void hide();
private:
    QLabel mLabel;
    QGridLayout mLayout;
    QPropertyAnimation mAnimation;
    float mPopupOpacity;
    QTimer *mTimer;
};

#endif // POPUP_H
