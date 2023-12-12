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
#include "user_login_page.h"
#include <QThread>

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

    // Create the login page
    User_Login_Page loginPage;

    //create signup page
    User_SignUp_Page signUpPage;

    // Main window
    QWidget mainWindow;
    mainWindow.setWindowTitle("TOMEO");

    // Set background color
    mainWindow.setStyleSheet("background-color: #36454F;");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&mainWindow);

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
    QObject::connect(profilePicButton, &QPushButton::clicked, [&profilePage, &mainWindow]() {
        mainWindow.close();
        profilePage.exec();
    });

    QObject::connect(&profilePage, &ProfilePage::closeProfile, [&mainWindow, &profilePage]() {
        profilePage.close();
        mainWindow.show();
    });



    // App name label
    QLabel *appNameLabel = new QLabel("ForReel");
    appNameLabel->setStyleSheet("font-size: 50px; font-weight: bold; color: white");

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


        TheButton *likeButton = new TheButton(nullptr);
        likeButton->setIcon(QIcon(":/new/prefix1/icons/heart_icon.png"));
        likeButton->setCheckable(true);
        likeButton->setFixedSize(QSize(40, 40));  // Set a fixed size for the like button
        likeButton->setIconSize(QSize(25, 25));  // Adjust the size according to your preference
        likeButton->setStyleSheet("background-color: #808080; border: none; border-radius: 20px;");

        // Connect the new signal to update the button icon
        QObject::connect(likeButton, &TheButton::likeStateChanged, [likeButton](bool liked) {
            if (liked) {
                likeButton->setIcon(QIcon(":/new/prefix1/icons/redheart_icon.png"));
            } else {
                likeButton->setIcon(QIcon(":/new/prefix1/icons/heart_icon.png"));
            }
        });


        // Share button
        QPushButton *shareButton = new QPushButton(QIcon(":/new/prefix1/icons/share_icon.png"), "");
        shareButton->setFixedSize(QSize(40, 40));  // Set a fixed size for the like button
        shareButton->setIconSize(QSize(25, 25));  // Adjust the size according to your preference
        shareButton->setStyleSheet("background-color: #808080; border: none; border-radius: 20px;");


        // Comment button
        QPushButton *commentButton = new QPushButton(QIcon(":/new/prefix1/icons/comment_icon.png"), "");
        commentButton->setFixedSize(QSize(40, 40));  // Set a fixed size for the like button
        commentButton->setIconSize(QSize(25, 25));  // Adjust the size according to your preference
        commentButton->setStyleSheet("background-color: #808080; border: none; border-radius: 20px;");


        // Sound button
        QPushButton *soundButton = new QPushButton(QIcon(":/new/prefix1/icons/muted_icon.png"), "");
        soundButton->setFixedSize(QSize(40, 40));  // Set a fixed size for the like button
        soundButton->setIconSize(QSize(25, 25));  // Adjust the size according to your preference
        soundButton->setStyleSheet("background-color: #808080; border: none; border-radius: 20px;");


        // Connect the click event of the soundButton to toggle sound and change the icon
        QObject::connect(soundButton, &QPushButton::clicked, [player, soundButton]() {
            player->toggleSound(); // Add a function to toggle sound in ThePlayer class

            // Toggle the icon based on the sound state
            if (player->isMuted()) {
                soundButton->setIcon(QIcon(":/new/prefix1/icons/muted_icon.png"));
            } else {
                soundButton->setIcon(QIcon(":/new/prefix1/icons/sound_icon.png"));
            }
        });

        // Share button
        QPushButton *noButton = new QPushButton(QIcon(""), "");
        noButton->setIconSize(QSize(30, 30));  // Adjust the size according to your preference
        noButton->setStyleSheet("background-color: transparent; border: none;");


        // Add buttons to the custom widget
        buttonsLayout->addWidget(likeButton);
        buttonsLayout->addWidget(shareButton);
        buttonsLayout->addWidget(commentButton);
        buttonsLayout->addWidget(noButton);
        buttonsLayout->addWidget(soundButton);

        // Add layouts and widgets to videoLayout
        videoLayout->addLayout(userLayout);
        videoLayout->addWidget(videoDisplay);
        videoLayout->addLayout(buttonsLayout);  // Add buttonsLayout to videoLayout


        // Add video widget to the scroll layout
        scrollLayout->addWidget(videoWidget);


        // Add a line separator
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(2);  // Adjust the line thickness here
        scrollLayout->addWidget(line);
    }

    // Set up scroll area
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



//        QPushButton *likeButton = new QPushButton(QIcon(":/new/prefix1/icons/heart_icon.png"), "");
//        // likeButton->setStyleSheet("outline: none;");  // Remove focus border
//        QPushButton *shareButton = new QPushButton(QIcon(":/new/prefix1/icons/share_icon.png"), "");
//        // shareButton->setFlat(true);  // Set flat property to remove the button outline
//        QPushButton *commentButton = new QPushButton(QIcon(":/new/prefix1/icons/comment_icon.png"), "");
//        // commentButton->setFlat(true);  // Set flat property to remove the button outline

//        // Add the buttons to the layout
//        buttonsLayout->addWidget(likeButton);
//        buttonsLayout->addWidget(shareButton);
//        buttonsLayout->addWidget(commentButton);

//        // Add layouts and widgets to videoLayout
//        videoLayout->addLayout(userLayout);
//        videoLayout->addWidget(videoDisplay);
//        videoLayout->addLayout(buttonsLayout);  // Add buttonsLayout to videoLayout


//        // Add video widget to the scroll layout
//        scrollLayout->addWidget(videoWidget);
//    }

//    // Set up scroll area
//    scrollArea->setWidgetResizable(true);
//    scrollArea->setWidget(scrollWidget);





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
    QObject::connect(navigateButton, &QPushButton::clicked, [&videoRecorderPage, &mainWindow]() {
        videoRecorderPage.show();
        mainWindow.close();
    });

    bottomBarLayout->addWidget(navigateButton, 0, Qt::AlignCenter);

    // Add bottom bar to the main layout
    mainLayout->addWidget(bottomBarWidget);

    QObject::connect(&loginPage, &User_Login_Page::login_Successful, [&mainWindow, &loginPage]() {
        mainWindow.show();
        loginPage.close();
    });

    QObject::connect(&videoRecorderPage, &VideoRecorderPage::closingVideo, [&mainWindow, &videoRecorderPage]() {
        mainWindow.show();
        videoRecorderPage.close();
    });

    QObject::connect(&loginPage, &User_Login_Page::create_an_account, [&loginPage, &signUpPage]() {
        loginPage.close();
        signUpPage.show();
    });

    QObject::connect(&signUpPage, &User_SignUp_Page::createAccountSuccess, [&signUpPage, &mainWindow]() {
        mainWindow.show();
        signUpPage.close();
    });

    QObject::connect(&signUpPage, &User_SignUp_Page::returnToLoginPage, [&signUpPage, &loginPage]() {
        loginPage.show();
        signUpPage.close();
    });



    // Show the login page
    loginPage.show();

    // wait for the app to terminate
    return app.exec();
}




