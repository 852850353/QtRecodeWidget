#include "qcb_imagescrollitem.h"
#include <QDebug>

QCB_ImageScrollItem::QCB_ImageScrollItem(QWidget *parent) : QWidget(parent)
{
    if(LeftButton==nullptr) LeftButton = new QPushButton("◀",this);     //左侧按钮
    if(RightButton==nullptr) RightButton = new QPushButton("▶",this);    //右侧按钮
    if(UpButton==nullptr) UpButton = new QPushButton("▲",this);       //上测按钮
    if(DownButton==nullptr) DownButton = new QPushButton("▼",this);     //下测按钮
    if(CentralWidget==nullptr) CentralWidget = new QWidget(this);
    CentralWidget->setStyleSheet("background-color:rgb(0,196,196)");
    resize(800,100);
    setMinimumSize(800,100);
    AreaChangeTimer.setInterval(10);
    AreaChangeTimer.start();
    connect(&AreaChangeTimer,SIGNAL(timeout()),this,SLOT(hideArea()));  //绑定隐藏面积信号槽
}

void QCB_ImageScrollItem::setShowImageSize(QSize miniImgSize)
{

}

void QCB_ImageScrollItem::enterEvent(QEvent *event)
{

    ScaleState = true;
    AreaChange();
}

void QCB_ImageScrollItem::leaveEvent(QEvent *event)
{
    ScaleState = false;
    AreaChange();
}

void QCB_ImageScrollItem::resizeEvent(QResizeEvent *event)
{
        AreaCut();
}

void QCB_ImageScrollItem::buttonConversion(QSize w_size)
{

}

void QCB_ImageScrollItem::AreaCut()
{
    QSize widgetArea = this->size();
    int RLbuttonWidth = 25; //左右侧各占1/20
    int RLbuttonHeight = widgetArea.height();   //高度和控件保持一致
    int UDbuttonWidth = widgetArea.width() - RLbuttonWidth *2;//上下侧减去左右侧按钮宽度
    int UDbuttonHeight = 25;//上下侧占控件1/5

    CentralWidgetArea = QRect(RLbuttonWidth,
                              UDbuttonHeight,
                              widgetArea.width()-RLbuttonWidth*2,
                              widgetArea.height()-UDbuttonHeight*2);//设置中间控件的面积和位置

    ScaleCentralWidgetArea = QRect(QPoint(0,0),size());

    ScaleLeftArea.setRect(0,0,RLbuttonWidth,RLbuttonHeight);
    LeftArea.setRect(0,0,RLbuttonWidth,RLbuttonHeight);

    ScaleRightArea.setRect(RLbuttonWidth+CentralWidgetArea.width(),0,RLbuttonWidth,RLbuttonHeight);
    RightArea.setRect(RLbuttonWidth+CentralWidgetArea.width(),0,RLbuttonWidth,RLbuttonHeight);
    //去除左侧宽度和中间件宽度

    ScaleUpArea.setRect(RLbuttonWidth,0,UDbuttonWidth,UDbuttonHeight);
    UpArea.setRect(RLbuttonWidth,0,UDbuttonWidth,UDbuttonHeight);
    //去除左按钮宽度

    ScaleDownArea.setRect(RLbuttonWidth,UDbuttonHeight+CentralWidgetArea.height(),UDbuttonWidth,UDbuttonHeight);
    DownArea.setRect(RLbuttonWidth,UDbuttonHeight+CentralWidgetArea.height(),UDbuttonWidth,UDbuttonHeight);
    //去除左按钮宽度; 去除上按钮高度和中间件高度

    LeftButton->resize(ScaleLeftArea.size());     //左侧按钮
    RightButton->resize(ScaleRightArea.size());    //右侧按钮
    UpButton->resize(ScaleUpArea.size());       //上侧按钮
    DownButton->resize(ScaleDownArea.size());     //下侧按钮
    CentralWidget->resize(ScaleCentralWidgetArea.size());
}

void QCB_ImageScrollItem::setWidgetGeometry(QRect left,QRect right,QRect up,QRect Down)
{


}

void QCB_ImageScrollItem::AreaChange()
{
    if(ScaleState)//启用状态
    {
        if(disconnect(&AreaChangeTimer,SIGNAL(timeout()),this,SLOT(hideArea())))
        {
            connect(&AreaChangeTimer,SIGNAL(timeout()),this,SLOT(showArea()),Qt::UniqueConnection);  //绑定显示面积信号槽
        }
    }
    else
    {
        if(disconnect(&AreaChangeTimer,SIGNAL(timeout()),this,SLOT(showArea())))
        {
            connect(&AreaChangeTimer,SIGNAL(timeout()),this,SLOT(hideArea()),Qt::UniqueConnection);  //绑定隐藏面积信号槽
        }
    }
}

void QCB_ImageScrollItem::showArea()
{
    if(ScaleLeftArea.size().width() < LeftArea.size().width())
    {
        ScaleLeftArea.setWidth(ScaleLeftArea.size().width()+1);//宽度累计
    }

    if(ScaleRightArea.x() >= RightArea.topLeft().x())
    {
        ScaleRightArea.setRect(ScaleRightArea.x()-1,ScaleRightArea.y(),ScaleRightArea.size().width()+1,ScaleRightArea.size().height());
    }

    if(ScaleUpArea.size().height() < UpArea.size().height())
    {
        ScaleUpArea.setHeight(ScaleUpArea.height()+1);
    }

    if(ScaleDownArea.y() >= DownArea.topLeft().y())
    {
        ScaleDownArea.setRect(ScaleDownArea.x(),ScaleDownArea.y()-1,ScaleDownArea.size().width(),ScaleDownArea.size().height()+1);
    }

    ScaleCentralWidgetArea.setRect(ScaleLeftArea.width(),ScaleUpArea.height(),
                                   size().width()- ScaleLeftArea.width()-ScaleRightArea.width(),
                                   size().height()- ScaleUpArea.height()-ScaleDownArea.height());

    LeftButton->setGeometry(ScaleLeftArea);      //左侧按钮
    RightButton->setGeometry(ScaleRightArea);    //右侧按钮
    UpButton->setGeometry(ScaleUpArea);          //上侧按钮
    DownButton->setGeometry(ScaleDownArea);      //下侧按钮
    CentralWidget->setGeometry(ScaleCentralWidgetArea);
}

void QCB_ImageScrollItem::hideArea()
{
    if(ScaleLeftArea.size().width() > 0)
    {
        ScaleLeftArea.setWidth(ScaleLeftArea.size().width()-1);//宽度累计
    }

    if(ScaleRightArea.x() <= RightArea.topRight().x())
    {
        ScaleRightArea.setRect(ScaleRightArea.x()+1,ScaleRightArea.y(),ScaleRightArea.size().width()-1,ScaleRightArea.size().height());
    }


    if(ScaleUpArea.size().height() > 0)
    {
        ScaleUpArea.setHeight(ScaleUpArea.height()-1);
    }

    if(ScaleDownArea.y() <= DownArea.bottomLeft().y())
    {
        ScaleDownArea.setRect(ScaleDownArea.x(),ScaleDownArea.y()+1,ScaleDownArea.size().width(),ScaleDownArea.size().height()-1);
    }

    ScaleCentralWidgetArea.setRect(ScaleLeftArea.width(),ScaleUpArea.height(),
                                   size().width()- ScaleLeftArea.width()-ScaleRightArea.width(),
                                   size().height()- ScaleUpArea.height()-ScaleDownArea.height());

    LeftButton->setGeometry(ScaleLeftArea);     //左侧按钮
    RightButton->setGeometry(ScaleRightArea);    //右侧按钮
    UpButton->setGeometry(ScaleUpArea);       //上侧按钮
    DownButton->setGeometry(ScaleDownArea);     //下侧按钮
    CentralWidget->setGeometry(ScaleCentralWidgetArea);
}



