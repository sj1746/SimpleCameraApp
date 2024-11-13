/* The MIT license (MIT)
 * Copyright (c) 2024 tsai shie jiun
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent) : QWidget(parent){
	mainWinLayout = new QGridLayout();
	cameraDisplay = new QLabel();
	program_runtime_info = new QLabel();
	mainWinLayout->addWidget(cameraDisplay,0,0);
	mainWinLayout->addWidget(program_runtime_info,1,0);
	
#ifdef USE_IMGSAVE
	user_def_layer_seconds = 60000;
	setting_stackedWidget = new QStackedWidget();
	control_frame = new QFrame();
	control_frame_layout = new QGridLayout();
	setting_frame = new QFrame();
	setting_frame_layout = new QGridLayout();
	layer_time_setting = new QLineEdit;
	layer_time_setting->setText("0000");
	layer_time_setting->setReadOnly(true);
	layer_time_setting->setMaxLength(4);
	layer_checkbox_setting = new QCheckBox("Print Layer Time (s)");

	capture_button = new QPushButton();
	capture_button->setFixedSize(QSize(42,42));
	settings_button = new QPushButton();
	settings_button->setFixedSize(QSize(42,42));
	close_setting_button = new QPushButton();
	close_setting_button->setFixedSize(QSize(42,42));
	capture_button->setIcon(QIcon(":/pngs/art_assets/photocam40.png"));
	capture_button->setIconSize(QSize(40,40));
	settings_button->setIcon(QIcon(":/pngs/art_assets/settings40.png"));
	settings_button->setIconSize(QSize(40,40));
	close_setting_button->setIcon(QIcon(":/pngs/art_assets/settings40.png"));
	close_setting_button->setIconSize(QSize(40,40));
	capture_button->setStyleSheet("QPushButton{background-color: transparent; border:0px solid}"
								"QPushButton:pressed{background-color: transparent; border:0px solid #000000;}");
	settings_button->setStyleSheet("QPushButton{background-color: transparent; border:0px solid}"
								"QPushButton:pressed{background-color: transparent; border:0px solid #000000;}");
	close_setting_button->setStyleSheet("QPushButton{background-color: transparent; border:0px solid}"
							"QPushButton:pressed{background-color: transparent; border:0px solid #000000;}");
	setting_stackedWidget->addWidget(settings_button);
	setting_stackedWidget->addWidget(close_setting_button);

	setting_frame_layout->addWidget(layer_checkbox_setting,0,0);
	setting_frame_layout->addWidget(layer_time_setting,0,1);
	setting_frame_layout->setColumnStretch(2,2);
	setting_frame->setLayout(setting_frame_layout);
	setting_frame->hide();

	control_frame_layout->addWidget(capture_button,0,1);
	control_frame_layout->addWidget(setting_stackedWidget,0,2);
	control_frame_layout->addWidget(setting_frame,0,3);
	control_frame_layout->setColumnStretch(0,2);
	control_frame_layout->setColumnStretch(3,2);
	control_frame->setLayout(control_frame_layout);

	connect(settings_button, &QPushButton::clicked, setting_frame,&QFrame::show);
	connect(settings_button,&QPushButton::clicked,this, [=](){setting_stackedWidget->setCurrentIndex(1);
	});
	connect(close_setting_button, &QPushButton::clicked, setting_frame,&QFrame::hide);
	connect(close_setting_button,&QPushButton::clicked,this, [=](){setting_stackedWidget->setCurrentIndex(0);
	});
	connect(close_setting_button, &QPushButton::clicked, this,[=](){
		autoStartCam();
	});

	connect(layer_checkbox_setting, &QCheckBox::stateChanged,this,[=](){
		if(layer_checkbox_setting->isChecked()){
			layer_time_setting->setReadOnly(false);
		}else{
			layer_time_setting->setReadOnly(true);
		}
	});
	connect(layer_time_setting, &QLineEdit::textChanged,this,[=](){
		layer_time_setting->text().toInt(&isInt);
		if(isInt){
			user_def_layer_seconds=1000 * layer_time_setting->text().toInt();
		}
	});
	mainWinLayout->setRowMinimumHeight(0,720);
	mainWinLayout->addWidget(control_frame,2,0);
	pictureDirExists();


#endif
	this->setLayout(mainWinLayout);
	QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &mainWindow::updateInfo);
	timer->start(1000);
	this->setFixedSize(900,760);
};

void mainWindow::updateInfo(){
	auto timenow = format("%D %T SG Time\n", date::make_zoned("Asia/Singapore",floor<seconds>(system_clock::now())));
	program_runtime_info->setText(QString::fromStdString(timenow));
}

void mainWindow::autoStartCam(){
	//QT Thread handling.
	camThread = new QThread;
	videoWorker = new videoCam();
	videoWorker->moveToThread(camThread);
	connect(camThread,&QThread::started,videoWorker,&videoCam::videoProcess);
	connect(videoWorker,&videoCam::finished,camThread,&QThread::quit);
	connect(camThread,&QThread::finished,camThread,&QThread::deleteLater);
	connect(videoWorker,&videoCam::finished,videoWorker,&videoCam::deleteLater);
	videoWorker->videoStart();
	camThread->start();
	connect(videoWorker,&videoCam::alert_user,this,&mainWindow::camera_closure);
	
#ifdef USE_IMGSAVE
	captureThread = new QThread;
	captureWorker = new image_save();
	captureWorker->moveToThread(captureThread);
	// alternative finish, if camera failed to be detected. -->
	connect(videoWorker,&videoCam::finished,captureThread,&QThread::quit);
	//<--
	connect(captureWorker,&image_save::finished,captureThread,&QThread::quit);
	connect(captureThread,&QThread::finished,captureThread,&QThread::deleteLater);
	connect(captureWorker,&image_save::finished,captureWorker,&image_save::deleteLater);
	camThread->start();
	connect(capture_button,&QPushButton::clicked,this,[&](){
		captureWorker->capture();
	});

	//automatic capture
	QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
		captureWorker->capture();
	});
	timer->start(user_def_layer_seconds);

#endif
	//update QLabel and capture thread.
	connect(videoWorker,&videoCam::videoFramesUpdate,this,[&](){
		cameraDisplay->setPixmap(videoWorker->pixmap().scaled(1280,720,Qt::KeepAspectRatio,Qt::SmoothTransformation));
		captureWorker->imgPixmap = videoWorker->pixmap().copy();
	});


}

void mainWindow::camera_closure(int alertValue){
	if(alertValue){
		//alert user
		QMessageBox msgBox;
		msgBox.setText("App failed to start.");
		msgBox.setInformativeText("Unable to detect a connected camera.");
		msgBox.setStandardButtons(QMessageBox::Ok); 
		msgBox.exec();
	}
}
