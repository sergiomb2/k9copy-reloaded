//
// C++ Implementation:
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "k9progress.h"
#include <QProgressBar>
#include <QLabel>
#include <QApplication>
#include <QEventLoop>
#include <QImage>
#include <QPainter>
#include <QMovie>
#include <QLayout>
#if QT_VERSION >= 0x050000
#include <KGuiItem>
#include <KStandardGuiItem>
#endif


k9Progress::k9Progress(QWidget* parent)
        : QDialog(parent) {
    Ui_Progress.setupUi(this);
    setModal(true);
    m_process=new k9Process(this,0);
    m_wimage=new k9DrawImage(Ui_Progress.image);
    QGridLayout *l=new QGridLayout(Ui_Progress.image);
    l->addWidget(m_wimage,0,0);

    #if QT_VERSION >= 0x050000
    // Qt5 code
    KGuiItem::assign(Ui_Progress.bCancel, KStandardGuiItem::stop() );
    QString aright=QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kmyapp/aright.png");
    QString adown=QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kmyapp/adown.png");
    #else
    // Qt4 code
    Ui_Progress.bCancel->setGuiItem(KStandardGuiItem::Stop);
    QString adown=KGlobal::dirs()->findResource( "data", QString("k9copy/adown.png"));
    QString aright=KGlobal::dirs()->findResource( "data", QString("k9copy/aright.png"));
    #endif

    Ui_Progress.gbOutput->setStyleSheet("*::indicator:unchecked {image:url("+aright+"); } *::indicator:checked {image:url("+adown+"); } ");
    m_movie=NULL;


}

void k9Progress::showEvent ( QShowEvent *  ) {
    k9Config config;
    Ui_Progress.gbOutput->setChecked(config.getPrefShowOutput());
}


k9Progress::~k9Progress() {
    delete m_process;
}


void k9Progress::setElapsed(const QString _text) {
    Ui_Progress.lblElapsed->setText(_text);
}

void k9Progress::setTitle(const QString _text) {
    Ui_Progress.lblTitle->setText(_text);
}


void k9Progress::setLabelText(const QString _text) {
    Ui_Progress.LabelText->setText(_text);
}

void k9Progress::setProgress(long _position,long _total) {
    Ui_Progress.ProgressBar->setRange(0,_total);
    Ui_Progress.ProgressBar->setValue(_position);
}

int k9Progress::execute() {
    if (! m_process->isRunning()) {
	qDebug() << m_process->debug();
        if (!m_process->start())
            return -1;
    }

//   m_timer.start(200,FALSE);
    show();
    m_canceled=false;
    //the sync method allows to wait for the process end while receiving stdout.
    m_process->sync();

//   m_timer.stop();
    close();
    if (m_canceled)
	return 0;
    else if (m_process->normalExit())
        return 1;
    else
        return -1;
}


void k9Progress::bCancelClick() {
    m_process->kill();
    m_canceled=true;
}

k9Process* k9Progress::getProcess() const {
    return m_process;
}

/*$SPECIALIZATION$*/


void k9Progress::setImage(QString _fileName) {
    m_wimage->setImage(_fileName);
}

void k9Progress::setImage(const QImage &_image) {
    m_wimage->setImage(_image);
}


void k9Progress::setMovie(QString _fileName) {
    m_wimage->hide();
    Ui_Progress.image->setPalette(this->palette());
    m_movie=new QMovie(_fileName);
    m_movie->start();
    Ui_Progress.image->setMovie(m_movie);

}


bool k9Progress::getCanceled() const {
    return m_canceled;
}


void k9Progress::gbOutputToggled(bool state) {
    k9Config config;
   if (!state) {
        m_outputSize=Ui_Progress.image->height();
        if (m_movie)
            Ui_Progress.image->clear();
        Ui_Progress.image->setMinimumSize(0,0);
        this->resize(this->width(),this->height()-m_outputSize);
        m_outputSize-=Ui_Progress.image->height();
        this->setMaximumHeight(this->height());

    } else {
        this->setMaximumHeight(32768);
        this->resize(this->width(),this->height()+m_outputSize);
        if (m_movie)
          Ui_Progress.image->setMovie(m_movie);
    }

    config.setPrefShowOutput(state);
    config.save();
    Ui_Progress.image->setVisible(state);
}

void k9Progress::closeEvent ( QCloseEvent *  ) {

}
