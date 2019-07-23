#include "fileselectline.h"
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

FileSelectLine::FileSelectLine(QWidget *parent)
{
    if(parent)
    {
        this->setParent(parent);
        this->installEventFilter(this);

        //        connect(this,&FileSelectLine::focesIn,[=]()
        //        {
        //            emit this->textChanged(this->text());
        //        });

        //        connect(this,&FileSelectLine::focesOut,[=]
        //        {
        //            emit this->enterKey();
        //        });

    }
    resize(this->minimumSize());
    this->setFocusPolicy(Qt::StrongFocus);
}

FileSelectLine::~FileSelectLine(){}

void FileSelectLine::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Enter
            || event->key() == Qt::Key::Key_Return)
    {
        qDebug()<<"enterKey";
        emit enterKey();
    }
    QLineEdit::keyPressEvent(event);
}

bool FileSelectLine::event(QEvent *event)
{
//    qDebug()<<__PRETTY_FUNCTION__<<QApplication::focusWidget()<<this;
//    this->rect().bottomLeft();
    this->rect().setBottomLeft(this->mapToGlobal(this->rect().bottomLeft()));
//    if(QApplication::focusWidget() == this)
//    {
//        if(parent()) qobject_cast<QWidget*>(parent())->setFocus();
//    }
    QLineEdit::event(event);
}

//void FileSelectLine::focusOutEvent(QFocusEvent *event)
//{
//    void focesOut();
//    QLineEdit::focusOutEvent(event);
//}

//void FileSelectLine::focusInEvent(QFocusEvent *event)
//{
//    void focesIn();
//    QLineEdit::focusInEvent(event);
//}


