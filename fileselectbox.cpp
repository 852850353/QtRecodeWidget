#include "fileselectbox.h"
#include <QStandardItem>
#include <QDir>
#include <QResizeEvent>
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include <QApplication>

FileSelectBox::FileSelectBox(QWidget *parent)
{
    if(parent) setParent(parent);
    initWidget();
}

FileSelectBox::FileSelectBox(FileSelectBox::FileSelectType type, QWidget *parent)
{
    if(type != FileSelectType::SELECT_ALL) m_eSelectFileType = type;
    if(parent) setParent(parent);
    initWidget();
}


FileSelectBox::~FileSelectBox()
{
    if(m_pLineEdit)   delete m_pLineEdit;
    if(m_pPushButton) delete m_pPushButton;
    if(m_pItemModel)  delete m_pItemModel;
    if(m_pTableView)  delete m_pTableView;

    if(m_pLineEdit && m_pPushButton && m_pItemModel && m_pTableView)
    {
        m_pLineEdit= nullptr;
        m_pPushButton= nullptr;
        m_pItemModel= nullptr;
        m_pTableView= nullptr;
    }
}

const QLineEdit *FileSelectBox::lineEdit()
{
    return m_pLineEdit;
}

const QPushButton *FileSelectBox::pushButton()
{
    return m_pPushButton;
}

const QTableView *FileSelectBox::tableView()
{
    return m_pTableView;
}

const QDialog *FileSelectBox::fileDialog()
{
    return m_pFileDialog;
}

void FileSelectBox::setLineEditText(QString Url)
{
    m_pLineEdit->setText(Url);
}

void FileSelectBox::setPushButtonText(QString DisplayTest)
{
    m_pPushButton->setText(DisplayTest);
}

void FileSelectBox::resizeEvent(QResizeEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    int buttonFixWidth = 20;

    this->setMinimumSize(100,buttonFixWidth);

    m_pLineEdit->setGeometry(0,
                             0,
                             event->size().width()-buttonFixWidth,
                             event->size().height());

    m_pPushButton->setGeometry(event->size().width()-buttonFixWidth,
                               0,
                               buttonFixWidth,
                               event->size().height());

    if(m_pTableView->model()!=nullptr && m_pTableView->model()->rowCount()>0)
    {
        m_pTableView->resize(this->size().width(),m_pTableView->model()->rowCount()*m_iItemHeight);
    }
    else{m_pTableView->close();}
    m_pTableView->move(m_MovePos);
}

void FileSelectBox::keyPressEvent(QKeyEvent *event)
{
    if(m_pTableView->model())
    {
        switch (event->key())
        {
        case Qt::Key::Key_Down:

            if(m_pTableView->currentIndex().row() < m_pTableView->model()->rowCount()-1)
            {
                m_pTableView->setCurrentIndex(m_pTableView->model()->index(m_pTableView->currentIndex().row()+1,0));
            }
            break;
        case Qt::Key::Key_Up:
            if(m_pTableView->currentIndex().row() > 0)
            {
                m_pLineEdit->setFocus();
                m_pTableView->setCurrentIndex(m_pTableView->model()->index(m_pTableView->currentIndex().row()-1,0));
            }
            break;
        }
    }
}

void FileSelectBox::moveEvent(QMoveEvent *event)
{
    m_MovePos = this->mapToGlobal(this->rect().bottomLeft());
    m_pTableView->move(m_MovePos);
}

void FileSelectBox::paintEvent(QPaintEvent *event)
{
    m_MovePos = this->mapToGlobal(this->rect().bottomLeft());
    //    m_MovePos = this->mapToGlobal(this->rect().bottomLeft());
    if(m_pLineEdit == QApplication::focusWidget()
            && m_pTableView
            && m_pTableView->model()
            && m_pTableView->model()->rowCount())
    {
        if(m_AfterPos != m_MovePos)
        {
            if(!m_AfterPos.isNull())
            {
                this->setFocus();
            }
            qDebug()<<"m_MovePos"<<m_MovePos;
            m_pTableView->move(m_MovePos);
            m_AfterPos = m_MovePos;
        }
    }else
    {
        m_pTableView->close();
    }
}



void FileSelectBox::showEvent(QShowEvent *event)
{
    m_pTableView->move(m_MovePos);
    if(m_pTableView) m_pTableView->show();
}

void FileSelectBox::hideEvent(QHideEvent *event)
{
    if(m_pTableView) m_pTableView->hide();
}

void FileSelectBox::closeEvent(QCloseEvent *event)
{
    if(m_pTableView) m_pTableView->close();
}

void FileSelectBox::focusInEvent(QFocusEvent *event)
{
    if(m_pTableView)
    {
        m_pTableView->move(m_MovePos);
        m_pTableView->show();
    }
    QWidget::focusInEvent(event);
}

void FileSelectBox::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__;
    if(m_pTableView) m_pTableView->hide();
    QWidget::focusOutEvent(event);
}

void FileSelectBox::initWidget()
{
    m_pLineEdit = new FileSelectLine(this);
    m_pLineEdit->setText("test");
    m_pPushButton = new QPushButton(this);
    m_pTableView = new selectPopList();
    m_pFileDialog = new QFileDialog();

    if(m_pLineEdit && m_pPushButton && m_pTableView)
    {
        m_pPushButton->setText("\n…");
        m_pPushButton->setStyleSheet("background:rgba(255,255,255,255)");
        m_pTableView->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
        m_pTableView->horizontalHeader()->hide();
        m_pTableView->verticalHeader()->hide();
        m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
        m_pTableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个

        connect(m_pPushButton,&QPushButton::clicked,this,&FileSelectBox::showSelectFileDialog);

        connect(m_pLineEdit,&QLineEdit::textChanged,this,&FileSelectBox::selectFile);

        connect(m_pLineEdit,&FileSelectLine::enterKey,[=]
        {
            if(m_pTableView->model()&& m_pTableView->model() &&m_pTableView->currentIndex().row() > -1)
            {
                m_pLineEdit->setText(m_pTableView->currentIndex().data().toString());
            }
        });

        connect(m_pFileDialog,&QFileDialog::fileSelected,[=](QString File){
            m_pLineEdit->setText(File);
        });

        connect(m_pFileDialog,&QFileDialog::urlSelected,[=](QUrl Path){
            m_pLineEdit->setText(Path.toString());
        });
        connect(m_pTableView,&selectPopList::enterKeySendCurrString,[=](QString filePath){
            m_pLineEdit->setText(filePath);
            m_pTableView->hide();
        });
    }
}

void FileSelectBox::showSelectFileDialog()
{
    m_pTableView->close();
    QStringList pathText = lineEdit()->text().split("\\");
    pathText.removeAt(pathText.size()-1);
    QString path;
    for(auto str : pathText)
    {
        path.append(str);
    }
    m_pFileDialog->history().clear();
    m_pFileDialog->setDirectoryUrl(path);
    if(path.isEmpty())
    {
        m_pFileDialog->setDirectory(QDir::currentPath());
    }
    else
    {
        m_pFileDialog->setDirectory(path);
    }
    m_pFileDialog->setWindowModality(Qt::ApplicationModal);
    m_pFileDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    switch (m_eSelectFileType) {
    case SELECT_DIRS:
        m_pFileDialog->setWindowTitle("Open Directory Only onec");
        m_pFileDialog->setFileMode(QFileDialog::DirectoryOnly);
        break;
    case SELECT_FILES:
        m_pFileDialog->setWindowTitle("Open File Only onec");
        m_pFileDialog->setFileMode(QFileDialog::FileMode::ExistingFile);
        break;
    case SELECT_ALL:
        m_pFileDialog->setWindowTitle("Open Directory or File Only onec");
        m_pFileDialog->setFileMode(QFileDialog::FileMode::AnyFile);
        break;
    }
    m_pFileDialog->show();
}

void FileSelectBox::selectFile(QString path)
{
    QDir dir(path);
    if(dir.exists() && path.size() > 0 && path.at(path.size()-1) == "\\")
    {
        QStringList fileNameList;
        switch (m_eSelectFileType) {
        case SELECT_DIRS:
            fileNameList = dir.entryList(QDir::Dirs);
            break;
        case SELECT_FILES:
            fileNameList = dir.entryList(QDir::Files);
            break;
        case SELECT_ALL:
            fileNameList = dir.entryList();
            break;
        }
        QStringList filePathList;
        for (auto var: fileNameList)
        {
            if(var != "." && var != "..")
            {
                filePathList << QString(path).append(var);
            }
        }
        showPopList(filePathList);
    }
    else
    {
        hidePopList();
    }

};

void FileSelectBox::hidePopList()
{
    if(m_pItemModel)
    {
        m_pTableView->hide();
        m_pTableView->clearSpans();
        delete m_pItemModel;
        m_pItemModel = nullptr;
    }
}

void FileSelectBox::enterKeyAddText()
{
    if(m_pTableView->model() && m_pItemModel && m_pTableView->currentIndex().row() > -1)
    {
        m_pLineEdit->setText(m_pTableView->currentIndex().data().toString());
    }
}

void FileSelectBox::showPopList(QStringList strList)
{
    if (strList.size()>0)
    {
        if(m_pItemModel)
        {
            delete m_pItemModel;
            m_pItemModel = nullptr;
        }

        m_pItemModel = new QStandardItemModel();
        QList<QStandardItem*> itemList;
        for(QString StrItem : strList)
        {
            itemList<< new QStandardItem(StrItem);
        }
        m_pItemModel->appendColumn(itemList);
        m_pTableView->setModel(m_pItemModel);
        if(m_pTableView->model()->rowCount()>0)
        {
            m_pTableView->resize(this->size().width(),m_pTableView->model()->rowCount()*m_iItemHeight);
        }

        m_pTableView->move(m_MovePos);
        m_pTableView->show();
    }
}
