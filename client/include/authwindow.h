#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

    void SetOtherWindowsList(size_t, QMainWindow **);

private:
    Ui::AuthWindow *ui;
    QMainWindow** otherWindowsList;
    size_t otherWindowsList_size;

    void LoginBtnHandler();
    void ShowPassBtnHandler();
    void SetDisabledOtherWindows(const bool&);
signals:
    void showed();
    void closed();
    void enterPressed();
protected:
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);
    void showEvent(QShowEvent*);
};

#endif // AUTHWINDOW_H
