/* The MIT license (MIT)
 * Copyright (c) 2024 tsai shie jiun
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "image_save.h"

image_save::image_save(){
	QThread::currentThread()->setObjectName("Camera thread");
}
image_save::~image_save(){
	emit finished();
}

void image_save::capture(){
	QMutex mutex;
	mutex.lock();
	//save image
	imgPixmap.scaled(640,720,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	auto timenow = format("%D %T", make_zoned("Asia/Singapore",floor<seconds>(system_clock::now())));
	QString filename= QString::fromStdString(timenow).replace(":","") + ".png";
	filename = filename.replace("/","");
	filename = filename.replace(" ","");
	filename = QString::fromStdString(std::string(std::getenv("HOME")))+ "/img_dir/" + filename;
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	imgPixmap.save(&file,"PNG");
	mutex.unlock();
}
