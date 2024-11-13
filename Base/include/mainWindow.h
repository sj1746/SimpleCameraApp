/* The MIT license (MIT)
 * Copyright (c) 2024 tsai shie jiun
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QStackedWidget>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QString>
#include <QStyleOption>
#include <QCheckBox>
#include <QLineEdit>
#include <chrono>
#include "videoIO.h"
#include "date.h"
#include "tz.h"
#ifdef USE_IMGSAVE
	#include <boost/filesystem.hpp>
	#include "image_save.h"
	#include "cstdlib"
	using namespace boost::filesystem;
#endif

using namespace std::chrono; 

class mainWindow : public QWidget{
	Q_OBJECT
public: 
    explicit mainWindow(QWidget *parent = nullptr);

public slots:
	void camera_closure(int alertValue);
	void updateInfo();
private:
	QGridLayout *mainWinLayout;
	QLabel *cameraDisplay;
	QThread *camThread;
	videoCam *videoWorker;
	QLabel *program_runtime_info;
	void autoStartCam();

#ifdef USE_IMGSAVE
	int user_def_layer_seconds;
	bool isInt;
	QThread *captureThread;
	image_save *captureWorker;
	QLineEdit *layer_time_setting;
	QCheckBox *layer_checkbox_setting;
	QLabel *layer_setting_label;
	QFrame *control_frame;
	QFrame *setting_frame;
	QPushButton *capture_button;
	QPushButton *settings_button;
	QPushButton *close_setting_button;
	QStackedWidget *setting_stackedWidget;
	QGridLayout *control_frame_layout;
	QGridLayout *setting_frame_layout;
	void pictureDirExists();
#endif

};

#ifdef USE_IMGSAVE

inline void mainWindow::pictureDirExists(){
	path img_dir_path = std::string(std::getenv("HOME"))+"/img_dir";
	if(!exists(img_dir_path)){
		create_directory(img_dir_path);
	}
};
#endif

#endif // MAINWINDOW_H
