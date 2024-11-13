/* The MIT license (MIT)
 * Copyright (c) 2024 tsai shie jiun
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef VIDEOCAMIO_H
#define VIDEOCAMIO_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QThread>

using namespace cv;
using namespace std;

class videoCam : public QObject{
	Q_OBJECT
public:
	videoCam();
	~videoCam();
	QPixmap pixmap()const{
		return pixmapFrame;
	}
	void videoStop();
	void videoStart();

public slots:
	void videoProcess();

signals:
	void videoFramesUpdate();
	void finished();
	void alert_user(int alert_issue);
private:
	cv::Mat camFrames;
	VideoCapture cap;
	int frame_HEIGHT, frame_WIDTH;
	bool isActive;
	void simple_colour_balance(cv::Mat& in, cv::Mat& out, float percent);
	QPixmap pixmapFrame;
	QPixmap cvToPixmap(const cv::Mat &inputMat);
	QImage cvToQimage(const cv::Mat &inputMat);


};
#endif
