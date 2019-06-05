#include "qcb_brushwidget.h"

QCB_BrushWidget::QCB_BrushWidget()
{

}

QCB_BrushWidget::QCB_BrushWidget(QWidget* childShow, QSizeProportion scaled_px)
{
   setShowWidget(childShow);
   scaleProportion = scaled_px;
}

void QCB_BrushWidget::setSacleProportion(QSizeProportion scaledPption)
{
    this->scaleProportion = scaledPption;
}

void QCB_BrushWidget::setSacleProportion(int hor_px, int vec_px)
{
    this->scaleProportion.setWidth(hor_px);
    this->scaleProportion.setHeight(vec_px);
}

QSizeProportion QCB_BrushWidget::getxSacleProportion() const
{
    return this->scaleProportion;
}

void QCB_BrushWidget::setShowWidget(QWidget *childShow)
{
    int maxLength= childShow->size().width()>childShow->size().height()?childShow->size().width():childShow->size().height();
    childShow->resize(maxLength,maxLength);
    childShow->setMinimumSize(10,10);
    this->SrcWidgetSize = childShow->size();
    this->setWidget(childShow);
}

void QCB_BrushWidget::wheelEvent(QWheelEvent *event)
{
    if(m_VectorKeys.count()>0)
    {
        KeyMouseUnion(m_VectorKeys,event->delta());//按键和鼠标组合操作
    }
    else
    {
        MouseWheel(event->delta(),widget());       //鼠标操作
    }
}

void QCB_BrushWidget::keyPressEvent(QKeyEvent *event)
{
    m_VectorKeys.append((Qt::Key)event->key());//组合键添加
    KeyUnion(m_VectorKeys);                    //按键组合操作函数
    qDebug()<<"按键按下";
}

void QCB_BrushWidget::keyReleaseEvent(QKeyEvent *event)
{
    m_VectorKeys.remove(m_VectorKeys.indexOf((Qt::Key)event->key()));//组合键删除
    qDebug()<<"按键松开";
}

void QCB_BrushWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"鼠标点击";
}

void QCB_BrushWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"鼠标送开";
}

void QCB_BrushWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"鼠标双击";
}

void QCB_BrushWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"鼠标移动"<<event->pos();
}

void QCB_BrushWidget::enterEvent(QEvent *event)
{
    qDebug()<<"进入事件";
}

void QCB_BrushWidget::leaveEvent(QEvent *event)
{
    qDebug()<<"离开事件";
}

void QCB_BrushWidget::focusInEvent(QFocusEvent *event)
{
    qDebug()<<"焦点获得";
}

void QCB_BrushWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<"焦点失去";
}

void QCB_BrushWidget::KeyUnion(QVector<Qt::Key> keys)
{
    if(keys.count()>1)//多键值触发
    {
        if(keys.count()==2)
        {
            qDebug()<<"双键值组合";
            if(keys.indexOf(Qt::Key::Key_Control)>=0 && keys.indexOf(Qt::Key::Key_Z)>=0)
            {
                qDebug()<<"恢复原图像";
                widget()->resize(this->SrcWidgetSize);
            }
        }
    }
    else if(keys.count()!=0)//单键值触发
    {
        qDebug()<<"单按键操作"<<keys;
    }
    //提示:执行完毕记得清空,否则可能存在多次触发组合键
}

void QCB_BrushWidget::KeyMouseUnion(QVector<Qt::Key> keys, int Direction)//参数1:组合键; 参数2:滚动方向;
{
    qDebug()<<"按键滚轮联合处理"<<scaleProportion<<widget()->size();
    if(keys.indexOf(Qt::Key::Key_Control)>=0)//Control键按下
    {
        if(widget()!=nullptr)//展示组件不为空
        {
            if(scaleProportion == QSize(0,0))//缩放大小px为空
            {
                    scaleProportion.setWidth(widget()->width()/10);
                    scaleProportion.setHeight(widget()->height()/10);
            }
            if(Direction>0)
            {
                qDebug()<<"放大控件";
                widget()->resize(widget()->size().width() + scaleProportion.width(),
                                 widget()->size().height() + scaleProportion.height());
            }
            else
            {
                qDebug()<<"缩小控件";
                widget()->resize(widget()->size().width() - scaleProportion.width(),
                                 widget()->size().height() - scaleProportion.height());
            }
            update();
        }
    }

}

void QCB_BrushWidget::MouseWheel(int Direction,QWidget* ShowImageWidget)//参数1:组合键; 参数2:滚动方向;
{
    if(ShowImageWidget == widget())//图片显示组件比对
    {
        QLabel* pointer = qobject_cast<QLabel*>(ShowImageWidget);
        if(Direction>0)
        {
            qDebug()<<"上滚轮事件";
            ScrollBarCrtl(0 - pointer->height()/10);//滚动大小计算传入
        }
        else
        {
            qDebug()<<"下滚轮事件";
            ScrollBarCrtl(0 + pointer->height()/10);//滚动大小计算传入
        }
    }
    else
    {
        qDebug()<<"pointer error from this->Widget()";
    }
}

void QCB_BrushWidget::ScrollBarCrtl(int WheelLength)
{
    verticalScrollBar()->setValue(verticalScrollBar()->value()+ WheelLength);
}
