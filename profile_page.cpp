// profile_page.cpp
#include "profile_page.h"
#include "ui_profile_page.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

ProfilePage::ProfilePage( QWidget *parent) : QDialog(parent), ui(new Ui::ProfilePage) {

    ui->setupUi(this);



    // Play 버튼 클릭 시 연결할 슬롯 함수를 설정합니다.
    connect(ui->Play_Button_1, &QPushButton::clicked, this, &ProfilePage::on_Play_Button_1_Clicked);
    connect(ui->Play_Button_2, &QPushButton::clicked, this, &ProfilePage::on_Play_Button_2_Clicked);

    // connector to close button
    connect(ui -> Close_Button, &QPushButton::clicked, this, &ProfilePage::on_Close_Button_Clicked);
    connect(ui -> Homebutton, &QPushButton::clicked, this, &ProfilePage::on_Homebutton_clicked);

    // connector to change image for user
    connect(ui->Tool_to_change_img, SIGNAL(clicked()), this, SLOT(on_Tool_to_change_img_clicked()));
    connect(ui->Tool_to_change_img_2, SIGNAL(clicked()), this, SLOT(on_Tool_to_change_img_clicked()));
}

ProfilePage::~ProfilePage() {
    delete ui;
}

void ProfilePage::on_Tool_to_change_img_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", QDir::homePath(), "Images (*.png *.jpg *.jpeg)");

    if (!imagePath.isEmpty())
    {
        QPixmap newPixmap(imagePath);
        ui->Profile_img_appearance->setPixmap(newPixmap.scaled(ui->Profile_img_appearance->size()*1.7, Qt::KeepAspectRatio));
        ui->Profile_img_appearance->raise();
        //ui->Tool_to_change_img->lower();

    }

    else
    {
        QMessageBox::information(this, "Image not selected", "No image selected or canceled");
    }

    // 선택된 이미지를 QLabel (Profile_img_appearance)에 표시하도록 설정

}

// Home 버튼 클릭 시 호출되는 슬롯 함수 구현
void ProfilePage::on_Homebutton_clicked() {
    // 초기 홈 페이지로 돌아가는 로직을 구현
    // 여기에 필요한 코드를 추가하세요.
    // 예를 들면, hide()를 호출하여 현재 페이지를 숨기고, 홈 페이지를 보여줄 수 있습니다.
    //close(); // 현재 페이지를 숨김
    // 다음에는 홈 페이지를 보여주는 코드를 추가하세요.
    emit closeProfile();
}

void ProfilePage::on_Close_Button_Clicked() {

    //tool to close ProfilePage
    close();
    //emit closeProfile();

}


void ProfilePage::on_Play_Button_1_Clicked()
{
    QString videoPath = QFileDialog::getOpenFileName(this, "Open Video", QDir::homePath(), "Videos (*.mp4 *.avi *.mkv)");

    if (!videoPath.isEmpty()) {
        QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
        QVideoWidget *videoWidget = new QVideoWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(ui->video_1);
        layout->addWidget(videoWidget);

        mediaPlayer->setVideoOutput(videoWidget);
        mediaPlayer->setMedia(QUrl::fromLocalFile(videoPath));
        mediaPlayer->play();

    }
}



void ProfilePage::on_Play_Button_2_Clicked()
{
    QString videoPath = QFileDialog::getOpenFileName(this, "Open Video", QDir::homePath(), "Videos (*.mp4 *.avi *.mkv)");

    if (!videoPath.isEmpty()) {
        QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
        QVideoWidget *videoWidget = new QVideoWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(ui->video_2);
        layout->addWidget(videoWidget);

        mediaPlayer->setVideoOutput(videoWidget);
        mediaPlayer->setMedia(QUrl::fromLocalFile(videoPath));
        mediaPlayer->play();
    }
}







