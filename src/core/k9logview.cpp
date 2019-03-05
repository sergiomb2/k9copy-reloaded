// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014

#include "k9logview.h"
#include "k9common.h"
#include "ui_k9logview.h"
#include <QDebug>
#if QT_VERSION >= 0x050000
// Qt 5 Code
#else
// Qt 4 Code
#endif

k9LogView::k9LogView(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::k9LogView)
{
    m_ui->setupUi(this);

    m_started=true;
    QTimer::singleShot(1000,this,SLOT(timerDone()));

}

k9LogView::~k9LogView()
{
    delete m_ui;
}

void k9LogView::closeEvent ( QCloseEvent *  ) {
    m_started=false;
}


void k9LogView::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void k9LogView::timerDone() {
 k9logEntry *logItem;
 while (k9log::count()>0 && m_started) {
     logItem=k9log::dequeue();
     if (logItem) {
        QIcon *icon;
        switch(logItem->level) {
            case k9logLevel::ERROR :
#if QT_VERSION >= 0x050000
	       icon=new QIcon("dialog-error");
	       icon->pixmap(24);
#else
               icon=new QIcon(SmallIcon("dialog-error"));
#endif
               break;
            case k9logLevel::WARNING :
#if QT_VERSION >= 0x050000
               icon=new QIcon("dialog-warning");
               icon->pixmap(24);
#else
               icon=new QIcon(SmallIcon("dialog-warning"));
#endif
               break;
            default:
#if QT_VERSION >= 0x050000
               icon=new QIcon("dialog-information");
               icon->pixmap(24);
#else
               icon=new QIcon(SmallIcon("dialog-information"));
#endif
               break;
        }
        m_ui->lwLogs->addItem(new QListWidgetItem(*icon,logItem->text,0));
        m_ui->lwLogs->scrollToBottom();
        delete icon;
     }
 }
 if (m_started)
    QTimer::singleShot(1000,this,SLOT(timerDone()));

}

QListWidget *k9LogView::list() {
    return m_ui->lwLogs;
}
