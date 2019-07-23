#include "fileselectline.h"
#include <QKeyEvent>
#include <QDebug>

FileSelectLine::FileSelectLine(QWidget *parent)
{
    if(parent)
    {
        this->setParent(parent);
        this->installEventFilter(this);
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


