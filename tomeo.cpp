#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QLabel>
#include <QScrollArea>
#include "profile_page.h"
#include "video_recorder_page.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn(std::string loc) {
    std::vector<TheButtonInfo> out = std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc));
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files
        QString f = it.next();

        if (f.contains("."))
#if defined(_WIN32)
            if (f.contains(".wmv")) { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV")) { // mac/linux
#endif
                QString thumb = f.left(f.length() - 4) + ".png";
                if (QFile(thumb).exists()) { // if a png thumbnail exists
                    QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon *ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl *url = new QUrl(QUrl::fromLocalFile(f));    // convert the file location to a generic url
                        out.push_back(TheButtonInfo(url, ico));           // add to the output list
                    } else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
                } else
                    qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
            }
    }

    return out;
}

// Placeholder structure for video information
struct VideoInfo {
    QString username;
    QString videoPath;
    // Add more fields as needed
};


int main(int argc, char *argv[]) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn(std::string(argv[1]));

    if (videos.size() == 0) {
        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // Main window
    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("TOMEO");

    // Set background color
    mainWindow->setStyleSheet("background-color: #36454F;");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWindow);

    // Top bar with picture and app name
    QWidget *topBarWidget = new QWidget();
    QHBoxLayout *topBarLayout = new QHBoxLayout(topBarWidget);
    topBarWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #29465B, stop:1 #36454F);");

    // Create the profile page
    ProfilePage profilePage;

    // Create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // Profile picture (clickable to navigate to profile)
    QPushButton *profilePicButton = new QPushButton("Profile");
    top->addWidget(profilePicButton);
    profilePicButton->setFixedSize(100, 100);
    profilePicButton->setStyleSheet("QPushButton {"
                                    "    border-radius: 50px;"
                                    "    background-color: #6D7B8D;"
                                    "}");

    // Connect the button's clicked signal to show the profile page
    QObject::connect(profilePicButton, &QPushButton::clicked, [&profilePage]() {
        profilePage.exec();
    });

    // App name label
    QLabel *appNameLabel = new QLabel("ForReal.");
    appNameLabel->setStyleSheet("font-size: 25px; font-weight: bold; color: white");

    // Add stretch to center the app name label
    topBarLayout->addWidget(profilePicButton);
    topBarLayout->addWidget(appNameLabel, 0, Qt::AlignCenter); // Center the app name label

    // Add top bar to the main layout
    mainLayout->addWidget(topBarWidget);

    // Scroll area for videos
    QScrollArea *scrollArea = new QScrollArea();
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // Example video widgets
    for (const auto &videoInfo : videos) {
        QWidget *videoWidget = new QWidget();
        QVBoxLayout *videoLayout = new QVBoxLayout(videoWidget);

        // QHBoxLayout for profile picture and username
        QHBoxLayout *userLayout = new QHBoxLayout();

        // Example profile picture (replace with your profile picture logic)
        QLabel *profilePicLabel = new QLabel("");
        profilePicLabel->setFixedSize(50, 50);
        profilePicLabel->setStyleSheet("QLabel {"
                                       "    border-radius: 25px;"
                                       "    background-color: #6D7B8D;"
                                       "}");

        // QLabel to display a placeholder username
        QLabel *usernameLabel = new QLabel("Username");  // Replace "Username" with your desired placeholder username
        usernameLabel->setStyleSheet("font-size: 14px; color: white;");

        userLayout->addWidget(profilePicLabel);
        userLayout->addWidget(usernameLabel);

        // Example video content
        QVideoWidget *videoDisplay = new QVideoWidget();
        ThePlayer *player = new ThePlayer;
        player->setVideoOutput(videoDisplay);
        player->jumpTo(new TheButtonInfo(videoInfo)); // Assuming ThePlayer has a jumpTo function
        player->play();

        // Set size policy and size hint for the video display
        videoDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        videoDisplay->setMinimumSize(400, 355); // Adjust these values according to your preference


        // Like, Share, Comment buttons
        QHBoxLayout *buttonsLayout = new QHBoxLayout();

        QPushButton *likeButton = new QPushButton(QIcon("C:/Users/lucky/Downloads/COMP2811-User-Interfaces-Group-4-main (1)/icons/heart_icon.png"), "");
        // likeButton->setStyleSheet("outline: none;");  // Remove focus border
        QPushButton *shareButton = new QPushButton(QIcon("C:/Users/lucky/Downloads/COMP2811-User-Interfaces-Group-4-main (1)/icons/share_icon.png"), "");
        // shareButton->setFlat(true);  // Set flat property to remove the button outline
        QPushButton *commentButton = new QPushButton(QIcon("C:/Users/lucky/Downloads/COMP2811-User-Interfaces-Group-4-main (1)/icons/comment_icon.png"), "");
        // commentButton->setFlat(true);  // Set flat property to remove the button outline

        // Add the buttons to the layout
        buttonsLayout->addWidget(likeButton);
        buttonsLayout->addWidget(shareButton);
        buttonsLayout->addWidget(commentButton);

        // Add layouts and widgets to videoLayout
        videoLayout->addLayout(userLayout);
        videoLayout->addWidget(videoDisplay);
        videoLayout->addLayout(buttonsLayout);  // Add buttonsLayout to videoLayout


        // Add video widget to the scroll layout
        scrollLayout->addWidget(videoWidget);
    }

    // Set up scroll area
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);

    // Add scroll area to the main layout
    mainLayout->addWidget(scrollArea);

    // Star bottom bar for camera button
    // Bottom bar with navigation button
    QWidget *bottomBarWidget = new QWidget();
    QHBoxLayout *bottomBarLayout = new QHBoxLayout(bottomBarWidget);
    bottomBarWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:1 #36454F);");

    // Fixed red circle button for navigation
    QPushButton *navigateButton = new QPushButton();
    navigateButton->setFixedSize(50, 50);
    navigateButton->setStyleSheet("QPushButton {"
                                  "    border-radius: 25px;"
                                  "    background-color: #ff4500;"
                                  "}");

    // Set up Video Recorder Page
    VideoRecorderPage videoRecorderPage;

    // Connect the button's clicked signal to show the profile page
    QObject::connect(navigateButton, &QPushButton::clicked, [&mainWindow, &videoRecorderPage]() {
        videoRecorderPage.show();

    });




//    QObject::connect(videoRecorderPage, &videoRecorderPage-close_page, this, &mainWindow, &videoRecorderPage){
//        mainWindow->show();
//        // Do something when 'widgetToClose' is closed
//        // This lambda function will be executed when the widget is destroyed
//    });



//    QObject::connect(videoRecorderPage.homeButton, &QPushButton::clicked, [&mainWindow, &videoRecorderPage]() {
//        mainWindow.show();
//        videoRecorderPage.close();
//    });

    bottomBarLayout->addWidget(navigateButton, 0, Qt::AlignCenter);

    // Add bottom bar to the main layout
    mainLayout->addWidget(bottomBarWidget);

    // showtime!


    mainWindow->show();

    // wait for the app to terminate
    return app.exec();
}




