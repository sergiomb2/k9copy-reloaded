/***************************************************************************
*    Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014

 *   Copyright (C) 2005 by Jean-Michel Petit                               *
 *   k9copy@free.fr                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "k9backupdlg.h"
#include <qtimer.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qprogressbar.h>
#include <qlabel.h>

#if QT_VERSION >= 0x050000
// Qt 5 Code
#include <kstandardguiitem.h>
#include <qpushbutton.h>
#else
// Qt 4 Code
#include <kiconloader.h>
#include <kstdguiitem.h>
#include <kpushbutton.h>
#endif
#include <kguiitem.h>

#include <qlabel.h>
#include <qpainter.h>
#include <qlayout.h>
#include "k9drawimage.h"
#if QT_VERSION >= 0x050000
#include <qdialog.h>
#include <KGuiItem>
#include <KStandardGuiItem>
#else
#include <kdialog.h>
#endif

//Thread 1 (Thread 0x7ff692074b40 (LWP 12828)):
//[KCrash Handler]
//#6  QWidgetPrivate::init (this=0x55879c45aad0, parentWidget=0x55879c607840, f=...) at kernel/qwidget.cpp:1128
//#7  0x00007ff6a6a316a5 in QDialog::QDialog (this=0x55879c5802d0, parent=0x55879c607840, f=...) at dialogs/qdialog.cpp:324
//#8  0x000055879924b3d9 in k9BackupDlg::k9BackupDlg (this=0x55879c5802d0, parent=0x55879c607840) at /usr/src/k9copy-code/k9copy/src/backup/k9backupdlg.cpp:55
//#9  0x000055879924003f in k9DVDBackup::k9DVDBackup (this=0x55879c699010, _dvd=0x55879bdd20b0) at /usr/src/k9copy-code/k9copy/src/backup/k9dvdbackup.cpp:114
//#10 0x000055879922fbba in k9ExecCopy::copyDVD (this=0x7ffe496397e0) at /usr/src/k9copy-code/k9copy/src/backup/k9execcopy.cpp:87
//#11 0x000055879918f51b in k9Main::Copy (this=0x55879bdbc2f0) at /usr/src/k9copy-code/k9copy/src/main/k9main.cpp:436
//#12 0x00005587991b61a1 in k9Copy::ActionCopy (this=0x55879bb13fb0) at /usr/src/k9copy-code/k9copy/src/main/k9copy.cpp:438

k9BackupDlg::k9BackupDlg(QWidget* parent)
: QDialog(parent) //// TODO: PTZ161111 this crashes time to time...!?
, k9InternalPlayer() {
    ui_backupDlg.setupUi(this);
    setModal(true);
#if QT_VERSION >= 0x050000
    setWindowTitle((i18n("Backup progression")));
#else
    setWindowTitle(KDialog::makeStandardCaption(i18n("Backup progression"),this));
#endif
    Abort=false;
    timer = new QTimer( this );
    time.start();
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    timer->start(500 );
    totalCopied=0;
    m_progressTotal=0;
    m_progressLabel="";
    m_totalSteps=0;
    m_factor="";
    m_progress=0;
//    connect(&m_decoder, SIGNAL(pixmapReady(QImage *)), this, SLOT(drawPixmap(QImage *)));
    m_decoder.setBackupDlg(this);
    m_stop=false;
    m_playmovie=true;
    m_wimage=new k9DrawImage(ui_backupDlg.image);
    QGridLayout *l=new QGridLayout(ui_backupDlg.image);
    l->addWidget(m_wimage,0,0);

#if QT_VERSION >= 0x050000
    KGuiItem::assign(ui_backupDlg.bAbort, KStandardGuiItem::stop() );
#else
    ui_backupDlg.bAbort->setGuiItem(KStandardGuiItem::Stop);
#endif
    #if QT_VERSION >= 0x050000
    // Qt5 code
    QString aright=QStandardPaths::locate(QStandardPaths::GenericDataLocation, "k9copy/aright.png");
    QString adown=QStandardPaths::locate(QStandardPaths::GenericDataLocation, "k9copy/adown.png");
    #else
    // Qt4 code
    QString aright=KGlobal::dirs()->findResource( "data", QString("k9copy/aright.png"));
    QString adown=KGlobal::dirs()->findResource( "data", QString("k9copy/adown.png"));
    #endif
    ui_backupDlg.gbOutput->setStyleSheet("*::indicator:unchecked {image:url("+aright+"); } *::indicator:checked {image:url("+adown+"); } ");


    m_timer.start();
}

void k9BackupDlg::drawImage(QImage *_image) {
    //m_wimage->setImage(*_image);
    static int cpt=0;
    if (cpt>4) {
        m_image=*_image;
        m_wimage->setImage(m_image);
        cpt=0;
    } else
        cpt++;
}

void k9BackupDlg::bPlayToggled( bool state) {
    m_playmovie=state;
}

void k9BackupDlg::gbOutputToggled(bool state) {
    m_playmovie=state;
    if (!state) {
        m_outputSize=ui_backupDlg.image->height();
        this->resize(this->width(),this->height()-m_outputSize);
        m_outputSize-=ui_backupDlg.image->height();
        this->setMaximumHeight(this->height());

    } else {
        this->setMaximumHeight(32768);
        this->resize(this->width(),this->height()+m_outputSize);
    }
    k9Config config;
    config.setPrefShowOutput(state);
    config.save();
    ui_backupDlg.image->setVisible(state);
}

void k9BackupDlg::playMovie(uchar *_data,uint32_t _size) {
    if (!m_playmovie  || (m_timer.elapsed() <3000))
        return;;
    if (m_mutex.tryLock()) {
	uint32_t maxSize=_size;
        for (uint32_t i=0;i< maxSize-2048;i+=2048) {
            m_decoder.addData(_data+i ,2048);
        }
      //  m_decoder.Stop();   
        m_mutex.unlock();
    }
    m_timer.restart();
}

k9BackupDlg::~k9BackupDlg() {
    delete timer;
}

void k9BackupDlg::updateMe() {
    ui_backupDlg.lblStep->setText(m_progressLabel);
    ui_backupDlg.pbTotal->setValue(m_progressTotal);
    ui_backupDlg.pbStep->setMaximum(m_totalSteps);
    ui_backupDlg.lblFactor->setText(m_factor);
    ui_backupDlg.pbStep->setValue(m_progress);
}

void k9BackupDlg::timerDone() {
    m_mutex.lock();
    QTime time2(0,0);
    time2=time2.addMSecs(time.elapsed());
    QString remain("--:--:--");
    if (m_progressTotal>0) {
        QTime time3(0,0);
        time3=time3.addMSecs((time.elapsed()/m_progressTotal)*ui_backupDlg.pbTotal->maximum());
        remain=time3.toString("hh:mm:ss");
    }

    ui_backupDlg.lblTime->setText(time2.toString("hh:mm:ss") +" / " +remain);
    updateMe();
    m_mutex.unlock();
}

void k9BackupDlg::setTotalSteps(uint32_t _totalSteps) {
    m_mutex.lock();
    m_totalSteps=_totalSteps;
    m_mutex.unlock();
}

void k9BackupDlg::setProgress(uint32_t _position) {
    m_mutex.lock();
    m_progress=_position;
    m_mutex.unlock();
}

void k9BackupDlg::setTotalMax(uint32_t _max) {
    m_mutex.lock();
    ui_backupDlg.pbTotal->setMaximum(_max);
    m_mutex.unlock();
}

void k9BackupDlg::setProgressTotal(uint32_t _position) {
    m_mutex.lock();
    totalCopied+=_position;
    uint64_t total=totalCopied*2048;
    total/=(1024*1024);
    m_progressTotal=total;
    m_mutex.unlock();
}

void k9BackupDlg::setProgressLabel(QString _text) {
    m_mutex.lock();
    m_progressLabel=_text;
    update();
    m_mutex.unlock();
}

bool k9BackupDlg::getAbort() {
    return Abort;
}

void k9BackupDlg::bAbortClick() {
    Abort=true;
    close();

}

void k9BackupDlg::setFactor(QString _factor) {
    m_mutex.lock();
    m_factor=_factor;
    m_mutex.unlock();
}


void k9BackupDlg::showEvent ( QShowEvent *  ) {
    k9Config config;
    ui_backupDlg.gbOutput->setChecked(config.getPrefShowOutput());
}


/*$SPECIALIZATION$*/
