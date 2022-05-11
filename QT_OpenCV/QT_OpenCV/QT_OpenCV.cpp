#include "QT_OpenCV.h"
#include"ui_QT_OpenCV.h"
#include"check.h"
#include<qmessagebox.h>
#include<qdebug.h>
#pragma execution_character_set("utf-8")
int c = 0;
QT_OpenCV::QT_OpenCV(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("串口识别控制 ");
	connect(&serial, &QSerialPort::readyRead, this, &QT_OpenCV::serialPort_readyRead);
	ui.cbb_com->clear();
	//运行开始查找可用串口
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui.cbb_com->addItem(info.portName());  
		qDebug() << "串口搜索完成";
		ui.tb_rx_buf->append("串口搜索完成");
	}
	ui.cbb_baud->insertItem(0, "9600");
	ui.cbb_baud->insertItem(1, "115200");
	ui.cbb_baud->insertItem(2, "384000");
	ui.cbb_baud->insertItem(3, "自定义");

	//初始化按钮失能
	ui.pushButton_5->setDisabled(true);    //控制按钮不可用
	ui.pushButton_7->setDisabled(true);
	ui.pushButton_8->setDisabled(true);
	ui.btn_send->setDisabled(true);
	ui.cb_send_enter->setDisabled(true);
	ui.te_tx_buf->setDisabled(true);
	//设置显示框最大行数为10行
	ui.tb_rx_buf->document()->setMaximumBlockCount(10);
}

//打开串口按钮
void QT_OpenCV::on_btn_uart_Ctrl_clicked()
{
	if (ui.btn_uart_Ctrl->text() == "打开串口")   //初始状态，配置串口参数
	{
		serial.setPortName(ui.cbb_com->currentText());     //设置串口号、
		serial.setBaudRate(ui.cbb_baud->currentText().toInt());    //设置波特率
		serial.setDataBits(QSerialPort::Data8);     //设置串口数据位8
		serial.setParity(QSerialPort::NoParity);    //无校验位
		serial.setStopBits(QSerialPort::OneStop);   //1位停止位
		serial.setFlowControl(QSerialPort::NoFlowControl);
		//打开串口
		if (!serial.open(QIODevice::ReadWrite))
		{
			QMessageBox::critical(NULL, "提示", "串口打开失败");
			return;
		}
		ui.tb_rx_buf->append("串口打开成功");
		ui.btn_uart_Ctrl->setText("关闭串口");

		ui.pushButton_5->setEnabled(true);    //控制按钮可用
		ui.pushButton_7->setEnabled(true);
		ui.pushButton_8->setEnabled(true);
		ui.btn_send->setEnabled(true);
		ui.te_tx_buf->setEnabled(true);
		ui.cb_send_enter->setEnabled(true);
	}
	else
	{
		//关闭串口
		serial.close();
		ui.btn_uart_Ctrl->setText("打开串口");
		ui.tb_rx_buf->append("串口关闭成功");

		ui.pushButton_5->setDisabled(true);    //控制按钮不可用
		ui.pushButton_7->setDisabled(true);
		ui.pushButton_8->setDisabled(true);
		ui.btn_send->setDisabled(true);
		ui.cb_send_enter->setDisabled(true);
		ui.te_tx_buf->setDisabled(true);
	}
}

void QT_OpenCV::on_pushButton_5_clicked()
{
	QPalette colr;
	if (ui.pushButton_5->text() == "启动")
	{
		colr.setColor(QPalette::WindowText, Qt::blue);
		ui.label_5->setPalette(colr);
		ui.pushButton_5->setText("停止");
		serial.write("A1\n");     //把数据通过串口发送出去
		ui.tb_rx_buf->append("发送指令A1，系统启动成功");
	}
	else
	{
		colr.setColor(QPalette::WindowText, Qt::black);
		ui.label_5->setPalette(colr);        //设置标签颜色红色
		ui.pushButton_5->setText("启动");
		serial.write("A2\n");     //串口发送A2
		ui.tb_rx_buf->append("发送指令A2,系统停止");
	}
}


void QT_OpenCV::on_pushButton_7_clicked()
{
	if (ui.pushButton_7->text() == "正转")
	{
		serial.write("A3\n");     //把数据通过串口发送出去
		ui.tb_rx_buf->append("发送指令A3,步进电机正转");
	}
}

void QT_OpenCV::on_pushButton_8_clicked()
{
	if (ui.pushButton_8->text() == "反转")
	{
		serial.write("A4\n");     //把数据通过串口发送出去
		ui.tb_rx_buf->append("发送指令A4,步进电机反转");
	}
}

//串口数据接收并显示
void QT_OpenCV::serialPort_readyRead()
{
	QByteArray rx_buf = serial.readAll();   //读取串口接收的数据
	/*
	if(rx_buf.endsWith("\r\n")) //判断接收最后是否是回车换行，即接收完成标志
	{

	}
	*/
	QString rx_buf_tmp = QString::fromLocal8Bit(rx_buf);    //转换为中文格式
	/*
	QT默认的编码是unicode，不能显示中文的
	windows默认使用（GBK/GB2312/GB18030）
	使用fromLocal8Bit()函数，实现了从本地字符集GB到Unicode的转换，用于处理汉语显示乱码等问题
	*/
	qDebug() << rx_buf_tmp;         //控制台输出

	ui.tb_rx_buf->append(rx_buf_tmp);

	rx_buf_tmp.clear();
	rx_buf.clear();
}

void QT_OpenCV::on_btn_serch_clicked()
{
	ui.cbb_com->clear();
	//运行开始查找可用串口
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui.cbb_com->addItem(info.portName());  //串口号下拉菜单，增加一个条目，为串口号COM4
		ui.tb_rx_buf->append("串口搜索完成");
	}
}

//自定义波特率
void QT_OpenCV::on_cbb_baud_currentIndexChanged(const QString &arg1)
{
	if (ui.cbb_baud->currentIndex() == 3)
	{
		ui.cbb_baud->setItemText(3, ""); //调成自定义波特率时，内容设置为空，准备接收输入
		ui.cbb_baud->setEditable(true);
	}
	else
	{
		ui.cbb_baud->setItemText(3, "自定义"); //调成自定义波特率时，内容设置为空，准备接收输入
		ui.cbb_baud->setEditable(false);
	}
	serial.setBaudRate(ui.cbb_baud->currentText().toInt());    //即使打开串口后，仍然可以设置波特率
}

//清除接收窗口按钮
void QT_OpenCV::on_btn_clear_clicked()
{
	ui.tb_rx_buf->clear();
}

//发送按钮被按下
void QT_OpenCV::on_btn_send_clicked()
{
	//获取多行输入框的数据并转换为UTF8格式
	QByteArray tx_buf = ui.te_tx_buf->toPlainText().toUtf8();

	if (send_enter_flag == true)
		tx_buf += "\n";

	serial.write(tx_buf);     //把数据通过串口发送出去
	ui.tb_rx_buf->append("指令发送成功");
	ui.te_tx_buf->clear();
}
//是否发送新行
void QT_OpenCV::on_cb_send_enter_clicked()
{
	if (ui.cb_send_enter->isChecked())
	{
		send_enter_flag = true;
		ui.tb_rx_buf->append("发送新行");
	}
	else
	{
		send_enter_flag = false;
		ui.tb_rx_buf->append("不发送新行");
	}
}

void QT_OpenCV::on_OpenCameraBtn_clicked()  //打开摄像头
{
	VideoCapture capture(0);
	ui.tb_rx_buf->append("障碍物识别");
	while (true)
	{
		capture.read(img);
		newpoints = findcolor(img);
		//drawcri(newpoints,mycolorvalues);
		imshow("video", img);
		waitKey(1);
		if (a==1)
		{
			break;
		}
		if (b == 1)
		{
			ui.tb_rx_buf->setText("前方有障碍物");
		}
	}
}
void QT_OpenCV::on_pushButton_clicked()
{
	Mat imag;
	VideoCapture capture(0);
	ui.tb_rx_buf->append("人脸识别");
	CascadeClassifier facecascade;
	facecascade.load("haarcascade_frontalface_default.xml");

	if (facecascade.empty())
	{
		cout << "Xml file not loaded" << endl;
	}
	vector<Rect> faces;
	while (true)
	{
		capture.read(imag);
		facecascade.detectMultiScale(imag, faces, 1.1, 10);

		for (int i = 0; i < faces.size(); i++)
		{
			c = 1;
			rectangle(imag, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 5);
		}
		imshow("Image",imag);
		waitKey(1);
		if (a == 1)
		{
			break;
		}	
		if (c == 1)
	    {
		ui.tb_rx_buf->setText("前方有人");
	    }
	}
}
void QT_OpenCV::on_CloseCameraBtn_clicked()  //关闭摄像头
{
	ui.tb_rx_buf->append("关闭摄像头");
	a = 1;
	/*VideoCapture capture;*/
	VideoCapture capture;
	capture.release();
}

void QT_OpenCV::on_HSV_clicked()  //HSV选取
{
	    ui.tb_rx_buf->append("HSV选取");
		VideoCapture cap(0);
		Mat img;
		Mat imgHSV, mask, imgcolor;

		int hmin = 0, smin = 0, vmin = 0;
		int hmax = 179, smax = 255, vmax = 255;

		namedWindow("Trackbars", (640, 300));
		createTrackbar("色调最小值", "Trackbars", &hmin, 179);
		createTrackbar("色调最大值", "Trackbars", &hmax, 179);
		createTrackbar("饱和度最小值", "Trackbars", &smin, 255);
		createTrackbar("饱和度最大值", "Trackbars", &smax, 255);
		createTrackbar("明度最小值", "Trackbars", &vmin, 255);
		createTrackbar("明度最大值", "Trackbars", &vmax, 255);

		while (true)
		{
			cap.read(img);
			cvtColor(img, imgHSV, COLOR_BGR2HSV);
			Scalar lower(hmin, smin, vmin);
			Scalar upper(hmax, smax, vmax);
			inRange(imgHSV, lower, upper, mask);
			cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
			imshow("img", img);
			//imshow("imghsv", imgHSV);
			imshow("imgmask", mask);
			waitKey(1);
		}
}

void QT_OpenCV::on_actionRedo_triggered()
{
	ui.tb_rx_buf->redo();
}

void QT_OpenCV::on_actionUndo_triggered()
{
	ui.tb_rx_buf->undo();
}