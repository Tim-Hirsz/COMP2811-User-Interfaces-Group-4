#include "video_recorder_page.h"
#include <QDebug>
#include <QUrl>

VideoRecorderPage::VideoRecorderPage(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #29465B, stop:1 #36454F);");
    topLayout = new QVBoxLayout(this);
    setWindowTitle("Video Recorder");
    //setMinimumSize(400, 340);
    setMaximumSize(600, 700);

    //create home button
    homeButton = new QPushButton("Home", this);
    homeButton->setFixedSize(100, 60);
    topLayout->addWidget(homeButton);

    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this);
    camera->setViewfinder(viewfinder);

    mediaRecorder = new QMediaRecorder(camera);
    camera->setCaptureMode(QCamera::CaptureVideo);

    camera->start();

    if (!camera->isAvailable()) {
        qDebug() << "Camera not available";
    } else {
        qDebug() << "Camera is available";
    }

    topLayout->addWidget(viewfinder);

    qDebug() << "Viewfinder added to layout";

    recordButton = new QPushButton("Record", this);
    stopButton = new QPushButton("Stop", this);

    connect(recordButton, &QPushButton::clicked, this, &VideoRecorderPage::startRecording);
    connect(stopButton, &QPushButton::clicked, this, &VideoRecorderPage::stopRecording);
    connect(homeButton, &QPushButton::clicked, this, &VideoRecorderPage::closeVideoPage);

    topLayout->addWidget(recordButton);
    topLayout->addWidget(stopButton);
}

void VideoRecorderPage::startRecording() {
    qDebug() << "Recording Start";
    QString fileName = "video.mp4"; // Change this to your desired file name
    QString videoFilePath = "C:/Users/lucky/Pictures/video.mp4";
    mediaRecorder->setOutputLocation(QUrl::fromLocalFile(videoFilePath));
    // mediaRecorder->setVideoSettings(camera->supportedViewfinderSettings().at(0)); // You might want to set appropriate video settings
    mediaRecorder->isAvailable();
    mediaRecorder->record();
    qDebug() << "Recording state: " << mediaRecorder->state();
}

void VideoRecorderPage::stopRecording() {
    mediaRecorder->stop();
    qDebug() << "Recording state: " << mediaRecorder->state();
}

void VideoRecorderPage::closeVideoPage() {
    emit closingVideo();
}

