/**************************************************************************
*   Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014

*   Copyright (C) 2005 by Jean-Michel Petit                               *
*   k9copy@free.fr                                                            *
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

#include "k9common.h"
#include "k9dvdprogress.h"
#include <QProgressBar>
#include <QLabel>
#include <QCloseEvent>
#if QT_VERSION >= 0x050000
#include <qdialog.h>
#else
#include <kdialog.h>
#endif
k9DVDProgress::k9DVDProgress(QWidget* )
{
	Ui_DVDProgress.setupUi(this);
#if QT_VERSION >= 0x050000
        setWindowTitle((i18n("DVD Analyze")));
#else
        setWindowTitle(KDialog::makeStandardCaption(i18n("DVD Analyze"),this));
#endif
}

k9DVDProgress::~k9DVDProgress(){
}
/** No descriptions */
void k9DVDProgress::setpbTitleTotalSteps(unsigned int total){
    //TODO:PTZ170217 if (Ui_DVDProgress.pbTitle) total shall be ... or else nasties ahead
        {
            Ui_DVDProgress.pbTitle->setMaximum(total);
        }

}
/** No descriptions */
void k9DVDProgress::setpbTitleStep(unsigned int position){
    // [KCrash Handler]//TODO:PTZ170217 
    // #6  0x000055fa12764340 in k9DVDProgress::setpbTitleStep (this=0x0, position=6400) at /usr/src/k9copy-code/k9copy/src/core/k9dvdprogress.cpp:52
    // #7  0x000055fa12756ad8 in k9DVD::slotVobProgress (this=0x55fa147d43c0, position=6400, total=34860) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:1057
    // #8  0x000055fa12784bf8 in k9DVD::qt_static_metacall (_o=0x55fa147d43c0, _c=QMetaObject::InvokeMetaMethod, _id=4, _a=0x7ffceca9f9c0) at /usr/src/k9copy-code/k9copy/bld/k9_5_automoc.dir/moc_k9dvd_RHQA5WVO7RR5FY.cpp:106
    // #9  0x00007fc5037295e9 in QMetaObject::activate(QObject*, int, int, void**) () from /usr/lib/x86_64-linux-gnu/libQt5Core.so.5
    // #10 0x000055fa12784f91 in k9DVD::sigVobProgress (this=0x55fa147d43c0, _t1=6400, _t2=34860) at /usr/src/k9copy-code/k9copy/bld/k9_5_automoc.dir/moc_k9dvd_RHQA5WVO7RR5FY.cpp:186
    // #11 0x000055fa12756414 in k9DVD::stream_vob (this=0x55fa147d43c0, title=7, startblock=109435, lastblock=144295, sc=0x7ffcecc2fe20) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:932
    if (Ui_DVDProgress.pbTitle) { 
        Ui_DVDProgress.pbTitle->setValue(position);
    }
}
/** No descriptions */
void k9DVDProgress::setpbTotalTotalSteps(int total) {
    if (Ui_DVDProgress.pbTotal) {
        Ui_DVDProgress.pbTotal->setMaximum(total);
    }
}
/** No descriptions */
void k9DVDProgress::setpbTotalStep(int position) {
    //if (&this != NULL) //return;
    //TODO::PTZ170113 some like this...actually might want to time-wait on Ui_DVDProgress.pbTotal ?!
    // Ui_DVDProgress::setupUi does the  QMetaObject::connectSlotsByName(DVDProgress)
    // then it might have been disconected also! not sure.
    //
    // Thread 1 (Thread 0x7fd85efd2340 (LWP 9448)):
    // [KCrash Handler]
    // #6  0x00005617264ba415 in k9DVDProgress::setpbTotalStep (this=0x0, position=1) at /usr/src/k9copy-code/k9copy/src/core/k9dvdprogress.cpp:60
    // #7  0x00005617264acbc2 in k9DVD::slotTitleProgress (this=0x5617289800b0, position=1, total=27) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:1063
    // #8  0x00005617264dac87 in k9DVD::qt_static_metacall (_o=0x5617289800b0, _c=QMetaObject::InvokeMetaMethod, _id=5, _a=0x7fffd0b66280) at /usr/src/k9copy-code/k9copy/bld/k9_5_automoc.dir/moc_k9dvd_RHQA5WVO7RR5FY.cpp:107
    // #9  0x00007fd87472e4e9 in QMetaObject::activate(QObject*, int, int, void**) () from /usr/lib/x86_64-linux-gnu/libQt5Core.so.5
    // #10 0x00005617264db04f in k9DVD::sigTitleProgress (this=0x5617289800b0, _t1=1, _t2=27) at /usr/src/k9copy-code/k9copy/bld/k9_5_automoc.dir/moc_k9dvd_RHQA5WVO7RR5FY.cpp:193
    // #11 0x00005617264a9f56 in k9DVD::scandvd (this=0x5617289800b0, device=..., _quickScan=false) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:466
    // ...

    if (Ui_DVDProgress.pbTotal)
        {
            Ui_DVDProgress.pbTotal->setValue(position);
        }
}
/** No descriptions */
void k9DVDProgress::setlblTitle(QString & text){
    if (Ui_DVDProgress.lblTitle) {
        Ui_DVDProgress.lblTitle->setText(text);
    }
}
/** No descriptions */
void k9DVDProgress::setlblTotal(QString& text){
    //TODO::PTZ170106 lots...like this
    //    [KCrash Handler]
    //#6  0x0000555a17a57468 in k9DVDProgress::setlblTotal (this=0x0, text=...) at /usr/src/k9copy-code/k9copy/src/core/k9dvdprogress.cpp:68
    //#7  0x0000555a17a49c3e in k9DVD::slotTotalText (this=0x555a1990fef0, text=...) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:1073
    //...from scandvd(,)
    if (Ui_DVDProgress.lblTotal) { //TODO::PTZ170106 might do the trick
        //not when reused...free does not work?!
        Ui_DVDProgress.lblTotal->setText(text);
    }
}

void k9DVDProgress::closeEvent( QCloseEvent* ce )
{
  ce->ignore();
  return;
}
