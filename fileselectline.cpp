#include "fileselectline.h"
#include <QKeyEvent>

FileSelectLine::FileSelectLine(QWidget *parent)
{
    if(parent)
    {
        this->setParent(parent);
    }
    resize(this->minimumSize());
    this->setFocusPolicy(Qt::StrongFocus);
}

FileSelectLine::~FileSelectLine(){}

void FileSelectLine::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    emit focusIn();
}

void FileSelectLine::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit focusOut();
}
