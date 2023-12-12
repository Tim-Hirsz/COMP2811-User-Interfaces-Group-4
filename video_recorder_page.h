#ifndef VIDEO_RECORDER_PAGE_H
#define VIDEO_RECORDER_PAGE_H
#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QTimer>

class VideoRecorderPage : public QWidget {
    Q_OBJECT

public:
    VideoRecorderPage(QWidget *parent = nullptr);
    QPushButton *homeButton;
    //QWidget *mainWindow;
    void close_page();
    //QWidget* mainWindow;



private slots:
    void startRecording();
    void stopRecording();
    //void close_page();

private:
    QVBoxLayout *topLayout;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QMediaRecorder *mediaRecorder;
    QPushButton *recordButton;
    QPushButton *stopButton;
    //QPushButton *homeButton;
};

#endif // VIDEO_RECORDER_PAGE_H
