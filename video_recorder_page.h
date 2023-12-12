#ifndef VIDEO_RECORDER_PAGE_H
#define VIDEO_RECORDER_PAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QCameraViewfinder>
#include <QMediaRecorder>

class VideoRecorderPage : public QWidget {
    Q_OBJECT

public:
    VideoRecorderPage(QWidget *parent = nullptr);

signals:
    void closingVideo();

private slots:
    void startRecording();
    void stopRecording();
    void closeVideoPage();

private:
    QVBoxLayout *topLayout;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QMediaRecorder *mediaRecorder;
    QPushButton *recordButton;
    QPushButton *stopButton;
    QPushButton *homeButton;
};

#endif // VIDEO_RECORDER_PAGE_H
