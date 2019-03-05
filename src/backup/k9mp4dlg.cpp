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


#include "k9mp4dlg.h"
#include <qlabel.h>
#include <QProgressBar>
#include <qpainter.h>
#include <qlayout.h>
#include "k9drawimage.h"
#include "k9logview.h"
#if QT_VERSION >= 0x050000
#include <qdialog.h>
#include <KGuiItem>
#include <KStandardGuiItem>
#include <QStandardPaths>
#else
#include <kdialog.h>
#include <kstandarddirs.h>
#endif


k9MP4Dlg::k9MP4Dlg(QWidget* parent)
        : QDialog(parent) {
    Ui_MP4Dlg.setupUi(this);
    setModal(true);
#if QT_VERSION >= 0x050000
    setWindowTitle((i18n("Encoding")));
#else
    setWindowTitle(KDialog::makeStandardCaption(i18n("Encoding"),this));
#endif
    m_wimage=new k9DrawImage(Ui_MP4Dlg.image);
    QGridLayout *l=new QGridLayout(Ui_MP4Dlg.image);
    l->addWidget(m_wimage,0,0);
    l = new QGridLayout(Ui_MP4Dlg.tabLog);
    k9LogView *log=new k9LogView(Ui_MP4Dlg.tabLog);
    l->setSpacing(6);
    l->setMargin(0);
    l->addWidget(log, 0, 0, 1, 1);

    m_update=true;
    m_timer.start();
    m_timerUpdate.setInterval(200);
    connect(&m_timerUpdate,SIGNAL(timeout()),this,SLOT(Update()));
    m_timerUpdate.start();

#if QT_VERSION >= 0x050000
    KGuiItem::assign(Ui_MP4Dlg.bCancel, KStandardGuiItem::stop() );
#else
    Ui_MP4Dlg.bCancel->setGuiItem(KStandardGuiItem::Stop);
#endif

}

k9MP4Dlg::~k9MP4Dlg() {
}

/*$SPECIALIZATION$*/

void k9MP4Dlg::Update() {
    if (m_update) {
        m_update=false;
        Ui_MP4Dlg.lblTitle->setText(m_titleLabel);
        Ui_MP4Dlg.lblfps->setText(m_fps);
        Ui_MP4Dlg.lblRemain->setText(m_remain);
        Ui_MP4Dlg.pbProgress->setValue(m_progress);
        Ui_MP4Dlg.lblbitrate->setText(m_bitrate);
        Ui_MP4Dlg.lblsize->setText(m_size);
        Ui_MP4Dlg.lbllabelSize->setHidden(m_size.isEmpty());
        m_wimage->setImage(m_fileName);
    }

}

void k9MP4Dlg::Cancel() {
    //QDialog::accept();
    emit sigCancel();
}

void k9MP4Dlg::setTitleLabel(QString _titleLabel) {
     m_titleLabel=_titleLabel;
     m_update=true;
}


void k9MP4Dlg::setfps(QString _fps) {
    m_fps=_fps;
    m_update=true;
}

void k9MP4Dlg::setremain(QString _remain) {
    if (m_timer.elapsed() >=1000) {
        m_remain=_remain;
        m_timer.restart();
    }
}

void k9MP4Dlg::setProgress(int _progress) {
   m_progress=_progress;
    m_update=true;
}

void k9MP4Dlg::setbitrate(QString _bitrate) {
   m_bitrate=_bitrate;
   m_update=true;
}

void k9MP4Dlg::setsize( QString _size) {
    m_size=_size;
    m_update=true;
}


void k9MP4Dlg::setImage(QString _fileName) {
   m_fileName=_fileName;
   m_update=true;
}

