#ifndef FILESELECTLINE_H
#define FILESELECTLINE_H

#include <QLineEdit>

class FileSelectLine:public QLineEdit
{
    Q_OBJECT
public:
    explicit FileSelectLine(QWidget* parent =nullptr);
    ~FileSelectLine();

signals:
    void focusIn();
    void focusOut();

private:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

};


#endif // FILESELECTLINE_H
