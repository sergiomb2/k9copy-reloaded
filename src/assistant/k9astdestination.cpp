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

#include "k9astdestination.h"
#include "k9dvd.h"
#include "k9cddrive.h"
#include <QComboBox>
#include "QDebug"


#if QT_VERSION >= 0x050000
#include <QUrl>
#include <QFileDialog>
#else
#include <KFileDialog>
#endif


k9astDestination::k9astDestination(k9Assistant* parent,k9CdDrives *_drives,k9DVD *_dvd)
        : QWidget(parent) {
    Ui_astDestination.setupUi(this);
    m_parent=parent;
    m_drives=_drives;
    m_dvd=_dvd;
    fillList();
    m_parent->setDVDdestination(dstDVD);
    Ui_astDestination.rbDVD->setChecked(false);
    Ui_astDestination.rbDVD->setChecked(true);

    Ui_astDestination.urFolder->setMode(KFile::Directory | KFile::ExistingOnly);

    Ui_astDestination.urMpeg->setVisible(false);
    Ui_astDestination.urAvi->setVisible(false);
    Ui_astDestination.urAudio->setVisible(false);
    Ui_astDestination.urAvi->setFilter("*.avi|Audio Video Interleave (*.avi)\n*.mpg|MPEG-1 and MPEG-2 systems (*.mpg)\n*.mp4|MPEG-4 format (*.mp4)\n*.asf|Advanced Streaming Format (*.asf)\n*.swf|Adobe Flash (*.swf)\n*.flv|Adobe Flash video files (*.flv)\n*.rm|RealVideo (*.rm)\n*.mov|QuickTime (*.mov)\n*.mkv|Matroska (*.mkv)");
    Ui_astDestination.urAvi->setMode(KFile::File) ;
	#if QT_VERSION >= 0x050000 //Qt5 Code
    Ui_astDestination.urAvi->fileDialog()->setAcceptMode(QFileDialog::AcceptSave);
	#else //Qt4 Code
    Ui_astDestination.urAvi->fileDialog()->setOperationMode(KFileDialog::Saving);
	#endif
    Ui_astDestination.urIso->setFilter("*.iso|Iso Image (*.iso)");
    Ui_astDestination.urIso->setMode(KFile::File) ;
       #if QT_VERSION >= 0x050000
        Ui_astDestination.urIso->fileDialog()->setAcceptMode(QFileDialog::AcceptSave);
	#else 
    Ui_astDestination.urIso->fileDialog()->setOperationMode(KFileDialog::Saving);
	#endif
    Ui_astDestination.urMpeg->setFilter("*.mpg|Mpeg video file (*.mpg)");
    Ui_astDestination.urMpeg->setMode(KFile::File);
	#if QT_VERSION >= 0x050000
    Ui_astDestination.urMpeg->fileDialog()->setAcceptMode(QFileDialog::AcceptSave);
	#else 
    Ui_astDestination.urMpeg->fileDialog()->setOperationMode(KFileDialog::Saving);
	#endif
    Ui_astDestination.urAudio->setMode(KFile::Directory | KFile::ExistingOnly);


    updategbDVD();
}


void k9astDestination::fillList() {
    connect(m_drives,SIGNAL(deviceAdded( k9CdDrive*)),this,SLOT(deviceAdded( k9CdDrive* )));
    connect(m_drives,SIGNAL(deviceRemoved( k9CdDrive*)),this,SLOT(deviceRemoved( k9CdDrive* )));

}


void k9astDestination::updatePaths() {
  k9Config config;
   QString title=m_parent->getDvd()->getDVDTitle();
   title= title.isEmpty() ? i18n("unknown"):title;
#if QT_VERSION >= 0x050000
   Ui_astDestination.urFolder->setUrl(QUrl(QDir::cleanPath(config.getPrefOutput())));
   Ui_astDestination.urIso->setUrl(QUrl(QDir::cleanPath(config.getPrefOutputIso()+"/"+title+".iso") ));
   Ui_astDestination.urAudio->setUrl(QUrl(QDir::cleanPath(config.getPrefOutputAudio())));
   Ui_astDestination.urAvi->setUrl(QUrl(QDir::cleanPath(config.getPrefOutputMpeg4()+"/"+title+".avi") ));
   Ui_astDestination.urMpeg->setUrl(QUrl(QDir::cleanPath(config.getPrefOutputMpeg2()+"/"+title+".mpg") ));
#else
   Ui_astDestination.urFolder->setUrl(KUrl(QDir::cleanPath(config.getPrefOutput())));
   Ui_astDestination.urIso->setUrl(KUrl(QDir::cleanPath(config.getPrefOutputIso()+"/"+title+".iso") ));
   Ui_astDestination.urAudio->setUrl(KUrl(QDir::cleanPath(config.getPrefOutputAudio())));
   Ui_astDestination.urAvi->setUrl(KUrl(QDir::cleanPath(config.getPrefOutputMpeg4()+"/"+title+".avi") ));
   Ui_astDestination.urMpeg->setUrl(KUrl(QDir::cleanPath(config.getPrefOutputMpeg2()+"/"+title+".mpg") ));  

#endif
}

void k9astDestination::deviceAdded(k9CdDrive *_drive) {

    if (_drive->canWriteDVD) {
         Ui_astDestination.cbDrives->addItem(QString("%1 (%2)").arg(_drive->name).arg(_drive->device),_drive->device );

    }
}


void k9astDestination::rbDVDToggled(bool _state) {
   Ui_astDestination.gbDVD->setVisible(_state);
   if (_state) {
    Ui_astDestination.rbAvi->setChecked(false);
    Ui_astDestination.rbMpeg->setChecked(false);
    Ui_astDestination.rbAudio->setChecked(false);
   }
   m_parent->setDestination(dstDVD);
}

void k9astDestination::rbAviToggled(bool _state) {
   Ui_astDestination.urAvi->setVisible(_state);
   if (_state) {
   	Ui_astDestination.rbDVD->setChecked(false);
        Ui_astDestination.rbMpeg->setChecked(false);
        Ui_astDestination.rbAudio->setChecked(false);
   }
   m_parent->setDestination(dstAvi);
}

void k9astDestination::rbMpegToggled(bool _state) {
   Ui_astDestination.urMpeg->setVisible(_state);
   if (_state) {
      Ui_astDestination.rbDVD->setChecked(false);
      Ui_astDestination.rbAvi->setChecked(false);
      Ui_astDestination.rbAudio->setChecked(false);
   }
   m_parent->setDestination(dstMpeg);
}

void k9astDestination::rbAudioToggled(bool _state) {
   Ui_astDestination.urAudio->setVisible(_state);
   if (_state) {
      Ui_astDestination.rbDVD->setChecked(false);
      Ui_astDestination.rbAvi->setChecked(false);
      Ui_astDestination.rbMpeg->setChecked(false);
   }
   m_parent->setDestination(dstAudio);
}

void k9astDestination::updategbDVD() {
    Ui_astDestination.cbDrives->setVisible(Ui_astDestination.rbDVDDisc->isChecked());
    Ui_astDestination.urFolder->setVisible(Ui_astDestination.rbDVDFolder->isChecked());
    Ui_astDestination.urIso->setVisible(Ui_astDestination.rbDVDIso->isChecked());

}

void k9astDestination::rbDVDDiscToggled(bool _state) {
   updategbDVD();
   if (_state) {
       m_parent->setDVDdestination(dstDVD);
   }

}
void k9astDestination::rbDVDFolderToggled(bool _state) {
   updategbDVD();
   if (_state) {
       m_parent->setDVDdestination(dstFolder);
   }

}
void k9astDestination::rbDVDIsoToggled(bool _state) {
   updategbDVD();
   if (_state) {
       m_parent->setDVDdestination(dstIso);
   }
}


const QString & k9astDestination::getPath() {
   m_path="";
   if (Ui_astDestination.rbDVD->isChecked()) {
      if (Ui_astDestination.rbDVDIso->isChecked())

        m_path=Ui_astDestination.urIso->url().toLocalFile() ;

      if (Ui_astDestination.rbDVDFolder->isChecked())
	m_path=Ui_astDestination.urFolder->url().path();
      if (Ui_astDestination.rbDVDDisc->isChecked())
	m_path=Ui_astDestination.cbDrives->itemData(Ui_astDestination.cbDrives->currentIndex()).toString();
   }else if (Ui_astDestination.rbMpeg->isChecked())
	m_path=Ui_astDestination.urMpeg->url().path();
   else if (Ui_astDestination.rbAvi->isChecked())
	m_path=Ui_astDestination.urAvi->url().path();
   else if (Ui_astDestination.rbAudio->isChecked())
	m_path=Ui_astDestination.urAudio->url().path();


   return m_path;
}

bool k9astDestination::isOk() {
  return getPath() !="";
}


void k9astDestination::deviceRemoved(k9CdDrive *) {

}


bool k9astDestination::isMatroska() {
   return getPath().endsWith(".mkv");
}

k9astDestination::~k9astDestination() {
}


/*$SPECIALIZATION$*/

