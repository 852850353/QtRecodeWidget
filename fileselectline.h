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

protected:
    void keyPressEvent(QKeyEvent *event);
};


#endif // FILESELECTLINE_H
