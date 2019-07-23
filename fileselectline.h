#ifndef FILESELECTLINE_H
#define FILESELECTLINE_H

#include <QLineEdit>
#include <QKeyEvent>

class FileSelectLine:public QLineEdit
{
    Q_OBJECT
public:
    explicit FileSelectLine(QWidget* parent =nullptr);
    ~FileSelectLine();

signals:
    void enterKey();
//    void focesOut();
//    void focesIn();
protected:
    void keyPressEvent(QKeyEvent *event);
//    void focusOutEvent(QFocusEvent *event);
//    void focusInEvent(QFocusEvent *event);
    bool event(QEvent *event);
//    void (QMouseEvent *event);
};

#endif // FILESELECTLINE_H
