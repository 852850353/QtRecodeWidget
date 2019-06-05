#ifndef QCB_BRUSHWIDGET_H
#define QCB_BRUSHWIDGET_H

#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include <QEvent>
#include <QVector>
#include <QLabel>
#include <QDebug>

typedef QSize QSizeProportion;
class QCB_BrushWidget:public QScrollArea
{
    Q_OBJECT
public:
    QCB_BrushWidget();
    QCB_BrushWidget(QWidget* childShow,QSizeProportion scaled_px=QSize(0,0));

    void setSacleProportion(QSizeProportion scaledPption); //px
    void setSacleProportion(int hor_px,int vec_px);//px
    QSizeProportion getxSacleProportion() const;
    void setShowWidget(QWidget *childShow);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    virtual void KeyUnion(QVector<Qt::Key> keys);
    virtual void KeyMouseUnion(QVector<Qt::Key> keys, int Direction);
    virtual void MouseWheel(int Direction, QWidget *ShowImageWidget);
    virtual void ScrollBarCrtl(int WheelLength);

private:
    QSize SrcWidgetSize = QSize(0,0);
    QSizeProportion scaleProportion = QSize(0,0);    //缩放大小,默认不开启缩放
    QPoint S_MousePoint = QPoint(-1,-1);      //鼠标坐标
    QVector<Qt::Key> m_VectorKeys;           //多值存储容器
    QVector<Qt::MouseButton> MouseButton;   //多值存储容器
};

#endif // QCB_BRUSHWIDGET_H
