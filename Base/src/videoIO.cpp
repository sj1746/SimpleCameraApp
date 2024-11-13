/* The MIT license (MIT)
 * Copyright (c) 2024 tsai shie jiun
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "videoIO.h"

videoCam::videoCam(){
	QThread::currentThread()->setObjectName("Camera thread");
	isActive=0;

}
videoCam::~videoCam(){
	isActive=0;
	emit finished();
}

void videoCam::videoProcess(){
	cap.set(CAP_PROP_FRAME_WIDTH,frame_WIDTH);
	cap.set(CAP_PROP_FRAME_HEIGHT,frame_HEIGHT);
	cap.set(CAP_PROP_FPS,30);
	cap.open(0,CAP_V4L2);
	if(!cap.isOpened()){
		//failure to open camera!
		emit alert_user(1);
		emit finished();
	}
	while(isActive){
		cap.read(camFrames);
		if (camFrames.empty()) {
			break;}
		//converting cvMat to pixmap
		pixmapFrame = cvToPixmap(camFrames);
		emit videoFramesUpdate();
	}
	//emit pixmap for qlabel use.
}


void videoCam::videoStop(){
	isActive=0;
}
void videoCam::videoStart(){
	frame_WIDTH = 1920;
	frame_HEIGHT = 1080;
	isActive=1;
}


QImage videoCam::cvToQimage(const cv::Mat &inputMat){
    QImage image( inputMat.data,
                  inputMat.cols, inputMat.rows,
                  static_cast<int>(inputMat.step),
                  QImage::Format_RGB888 );
    return image.rgbSwapped();
}
QPixmap videoCam::cvToPixmap(const cv::Mat &inputMat){
	return QPixmap::fromImage( cvToQimage( inputMat ) );
}
