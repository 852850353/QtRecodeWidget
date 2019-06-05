#ifndef QCB_IMAGESCROLLITEM_H
#define QCB_IMAGESCROLLITEM_H

#include <QWidget>
#include <QPushButton>
#include <QRect>
#include <QTimer>

class QCB_ImageScrollItem : public QWidget
{
    Q_OBJECT

    enum ButtonType
    {
        Button_UP = 0,
        Button_Down,
        Button_Lefe,
        Button_right,
    };

public:
    explicit QCB_ImageScrollItem(QWidget *parent = nullptr);
    void setShowImageSize(QSize ImgSize);               //设置缩略图大小

private:
    QVector<QImage> Img_Item;   //ItemImage表 用于打开所有的表
    unsigned int Itemline;      //显示行数
    QPushButton* LeftButton = nullptr;     //左侧按钮
    QPushButton* RightButton = nullptr;    //右侧按钮
    QPushButton* UpButton = nullptr;       //上测按钮
    QPushButton* DownButton = nullptr;     //下测按钮
    QWidget* CentralWidget = nullptr;      //中间按钮

    QRect CentralWidgetArea;    //中间件的起始坐标
    QRect LeftArea;     //左侧面积
    QRect RightArea;    //右侧面积
    QRect UpArea;       //上侧面积
    QRect DownArea;     //下侧面积

    void buttonConversion(QSize w_size);    //按钮转置
    void AreaCut();//面积分割初始化操作
    void AreaChange();//动态更改面积

    QRect ScaleLeftArea = QRect(0,0,0,0);     //缩放左侧面积
    QRect ScaleRightArea = QRect(0,0,0,0);    //缩放右侧面积
    QRect ScaleUpArea = QRect(0,0,0,0);       //缩放上侧面积
    QRect ScaleDownArea = QRect(0,0,0,0);          //缩放下侧面积
    QRect ScaleCentralWidgetArea = QRect(0,0,0,0); //中间件的缩放坐标
    bool ScaleState = false;      //状态控制,默认没有控件
    QTimer AreaChangeTimer;       //面积更改定时器
    void setWidgetGeometry(QRect left, QRect right, QRect up, QRect Down);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void ClickedEvent(ButtonType);

public slots:
    void showArea();
    void hideArea();
};

#endif // QCB_IMAGESCROLLITEM_H
