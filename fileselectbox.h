#ifndef FILESELECTBOX_H
#define FILESELECTBOX_H

/*******************************************************************************
* SrcFileFormat:UTF-8
*
* Developer:FunNing
*
* Operation: Dynamic input path display file, imitating CMake TableView item
*
* Widgets:
*    QFileDialog
*    QLineEdit
*    QPushButton
*    QTableView
*
* provide Interface:
*    const QLineEdit* lineEdit();       //Get m_pLineEdit   Widget
*    const QPushButton* pushButton();   //Get m_pPushButton Widget
*    const QTableView* tableView();     //Get m_pTableView  Widget
*
*
* Exists Debug: 2 (Unkown from Qt or self Used member)
* 1.      setGeometry: Unable to set geometry 100x30+640+280
*         on QWidgetWindow/'FileSelectBoxClassWindow'.
*         Resulting geometry:  120x30+640+280
*         (frame: 8, 31, 8, 8, custom margin: 0, 0, 0, 0,
*         minimum size: 100x20, maximum size: 16777215x16777215).
*
*   from mathod virtual void FileSelectBox::resizeEvent(QResizeEvent* event);
*
*
* 2.      shell\comdlg32\fileopensave.cpp(14274)\COMDLG32.dll!00007FFF4E58F092:
*         (caller: 00007FFF4E5BFE41) ReturnHr(1) tid(1624) 80004005 未指定的错误
*         CallContext:[\PickerModalLoop]
*
*   from member class QFileDialog* m_pFileDialog;
*
*
* RunTime Test:
*   call lineEidt 500000, No wild pointers and memory leaks were found
*
*******************************************************************************/

#include <QWidget>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>

#include "fileselectline.h"
class QFileDialog;

class selectPopList : public QTableView
{
    Q_OBJECT
public:
    explicit selectPopList(QWidget* parent = nullptr)
    {
        if(parent) this->setParent(parent);
        setStyleSheet("QTableView::item{selection-background-color:rgb(23,165,230)}");
    }
    ~selectPopList(){
        if(timer){delete timer;timer=nullptr;}
    }

signals:
    void enterKeySendCurrString(QString);
    void enterKeySendCurrIndex(QModelIndex);

private:
    QTimer* timer = nullptr;
    void keyPressEvent(QKeyEvent* event)
    {
        switch (event->key()) {
        case Qt::Key::Key_Enter:
            emit enterKeySendCurrString(QString(this->currentIndex().data().toString()));
            emit enterKeySendCurrIndex(this->currentIndex());
            break;
        }
    }
    void mousePressEvent(QMouseEvent* event)
    {
        if(model())
        {
            switch (event->button()) {
            case Qt::LeftButton:
                qDebug()<<__PRETTY_FUNCTION__<<" >> "<<indexAt(this->mapFromGlobal(QCursor::pos()));
                selectRow(indexAt(this->mapFromGlobal(QCursor::pos())).row());
                setCurrentIndex(indexAt(this->mapFromGlobal(QCursor::pos())));
                if(!timer)
                {
                    timer = new QTimer();

                }
                connect(timer,&QTimer::timeout,[=]{
                    qDebug()<<"base timer";
                    emit enterKeySendCurrString(QString(this->currentIndex().data().toString()));
                    emit enterKeySendCurrIndex(this->currentIndex());
                    timer->stop();
                    delete timer;
                    timer = nullptr;
                });
                timer->start(180);
                break;

            }

        }
    }
};

class FileSelectBox : public QWidget
{
    Q_OBJECT
    enum FileSelectType
    {
        SELECT_ALL = 0,
        SELECT_DIRS = 1,
        SELECT_FILES = 2,
    };

public:
    explicit FileSelectBox(QWidget *parent = nullptr);
    ~FileSelectBox();
    const QLineEdit* lineEdit();
    const QPushButton* pushButton();
    const QTableView* tableView();
    void setLineEditText(QString Url);
    void setPushButtonText(QString DisplayTest);

private:
    FileSelectLine* m_pLineEdit = nullptr;
    selectPopList* m_pTableView = nullptr;
    QPushButton* m_pPushButton = nullptr;
    QStandardItemModel* m_pItemModel = nullptr;
    FileSelectType SelectFileType = SELECT_ALL;
    QFileDialog* m_pFileDialog = nullptr;
    int m_iItemWidth,m_iItemHeight = 30;
    QPoint m_MovePos;
    QPoint m_AfterPos;

    virtual void resizeEvent(QResizeEvent* event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void moveEvent(QMoveEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
    virtual void closeEvent(QCloseEvent *event);

    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
private slots:
    void showPopList(QStringList strList);
    void selectFile(QString path);
    void showSelectFileDialog();
    void hidePopList();
    void enterKeyAddText();

signals:

public slots:

};

#endif // FILESELECTBOX_H
