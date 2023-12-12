// profile_page.h
#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H


#include <QMediaPlayer>
#include <QLabel>
#include <QDialog>

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QDialog {
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();

signals:
    void closeProfile();

private slots:
    void on_Tool_to_change_img_clicked();
    void on_Homebutton_clicked();
    void on_Play_Button_1_Clicked();
    void on_Play_Button_2_Clicked();
    void on_Close_Button_Clicked();



private:
    Ui::ProfilePage *ui;
};

#endif // PROFILE_PAGE_H


