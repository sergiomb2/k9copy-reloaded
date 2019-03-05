//
// C++ Implementation: k9assistant
//
// Description:
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "k9assistant.h"
#include "k9astsource.h"
#include "k9asttitles.h"
#include "k9aststreams.h"
#include "k9astdestination.h"
#include "k9astmp4.h"
#include "k9astmpeg.h"
#include "k9astdvdoption.h"
#include "k9dvd.h"
#include "k9cddrive.h"
#include "k9execcopy.h"
//PM #include "dvdread.h"
#include "dvdread/dvd_reader.h"
#include "k9settings.h"
#include "k9dvdtitle.h"
#if QT_VERSION >= 0x050000
#include <QIcon>
#include <KIconLoader>
#endif


k9Assistant *k9Assistant::createAssistant() {
    KPageWidget * pageWidget=new KPageWidget();
    return new k9Assistant(pageWidget);

}

k9Assistant::k9Assistant(KPageWidget *_pageWidget)
        : KAssistantDialog(_pageWidget) {
    m_pageWidget=_pageWidget;
    m_dvd=new k9DVD();
    m_drives=new k9CdDrives(this);
}
void k9Assistant::run() {

    setWindowTitle(i18n("DVD backup assistant"));
    KIconLoader *loader = KIconLoader::global();
    
    k9astSource *source=new k9astSource(this,m_drives,m_dvd);
    m_sourceItem=new KPageWidgetItem(source,i18n("source"));
    m_sourceItem->setHeader( i18n( "Select the source of the backup" ) );
#if QT_VERSION >= 0x050000
    m_sourceItem->setIcon(QIcon(loader->loadIconSet("media-optical",  KIconLoader::Desktop)));
#else
    m_sourceItem->setIcon(KIcon(loader->loadIconSet("media-optical",  KIconLoader::Desktop)));
#endif

    m_pageWidget->addPage(m_sourceItem);

    k9astDestination *destination=new k9astDestination(this,m_drives,m_dvd);
    m_destinationItem=new KPageWidgetItem(destination,i18n("destination"));
    m_destinationItem->setHeader( i18n( "Select the destination of the backup" ) );
#if QT_VERSION >= 0x050000
    m_destinationItem->setIcon(QIcon(loader->loadIconSet("media-optical-recordable",  KIconLoader::Toolbar)));
#else
    m_destinationItem->setIcon(KIcon(loader->loadIconSet("media-optical-recordable",  KIconLoader::Toolbar)));
#endif
    m_pageWidget->addPage(m_destinationItem);

    k9astTitles *titles=new k9astTitles(this,m_dvd);
    m_titlesItem=new KPageWidgetItem(titles,i18n("titles"));
    m_titlesItem->setHeader(i18n("Select the titles you want to copy"));
#if QT_VERSION >= 0x050000
    m_titlesItem->setIcon(QIcon("titles"));
    m_titlesItem->setIcon(QIcon(loader->loadIconSet("title",  KIconLoader::Toolbar)));
#else
    m_titlesItem->setIcon(KIcon("titles"));
    m_titlesItem->setIcon(KIcon(loader->loadIconSet("title",  KIconLoader::Toolbar)));
#endif

    m_pageWidget->addPage(m_titlesItem);

    k9astStreams *streams=new k9astStreams(this,m_dvd);
    m_streamsItem=new KPageWidgetItem(streams,i18n("streams"));
    m_streamsItem->setHeader(i18n("Select the streams you want to keep"));
#if QT_VERSION >= 0x050000
    m_streamsItem->setIcon(QIcon(loader->loadIconSet("speaker",  KIconLoader::Toolbar)));
#else
    m_streamsItem->setIcon(KIcon(loader->loadIconSet("speaker",  KIconLoader::Toolbar)));
#endif

    m_pageWidget->addPage(m_streamsItem);

    k9astMp4 *mp4=new k9astMp4(this,m_dvd);
    m_mp4Item=new KPageWidgetItem(mp4,i18n("mp4"));
    m_mp4Item->setHeader(i18n("Set mp4 options for each title"));
#if QT_VERSION >= 0x050000
    m_mp4Item->setIcon(QIcon(loader->loadIconSet("mp4",  KIconLoader::Toolbar)));
#else
    m_mp4Item->setIcon(KIcon(loader->loadIconSet("mp4",  KIconLoader::Toolbar)));
#endif

    m_pageWidget->addPage(m_mp4Item);

    k9astDVDOption *DVDOption=new k9astDVDOption(this,m_dvd);
    m_DVDOptionItem=new KPageWidgetItem(DVDOption,i18n("DVDOption"));
    m_DVDOptionItem->setHeader(i18n("Set DVD playback options"));
#if QT_VERSION >= 0x050000
    m_DVDOptionItem->setIcon(QIcon(loader->loadIconSet("dvdcopy",  KIconLoader::Toolbar)));
#else
    m_DVDOptionItem->setIcon(KIcon(loader->loadIconSet("dvdcopy",  KIconLoader::Toolbar)));
#endif

    m_pageWidget->addPage(m_DVDOptionItem);

    k9astMpeg *mpeg=new k9astMpeg(this,m_dvd);
    m_mpegItem=new KPageWidgetItem(mpeg,i18n("mpeg"));
    m_mpegItem->setHeader(i18n("Set mpeg options for each title"));
#if QT_VERSION >= 0x050000
    m_mpegItem->setIcon(QIcon(loader->loadIconSet("mpeg",  KIconLoader::Toolbar)));
#else
    m_mpegItem->setIcon(KIcon(loader->loadIconSet("mpeg",  KIconLoader::Toolbar)));
#endif

    m_pageWidget->addPage(m_mpegItem);

    connect(titles,SIGNAL(addTitle(k9DVDTitle*)),streams,SLOT(addTitle(k9DVDTitle*)));
    connect(titles,SIGNAL(removeTitle(k9DVDTitle*)),streams,SLOT(removeTitle(k9DVDTitle*)));
    connect(titles,SIGNAL(addTitle(k9DVDTitle*)),mp4,SLOT(addTitle(k9DVDTitle*)));
    connect(titles,SIGNAL(removeTitle(k9DVDTitle*)),mp4,SLOT(removeTitle(k9DVDTitle*)));

    connect(titles,SIGNAL(addTitle(k9DVDTitle*)),mpeg,SLOT(addTitle(k9DVDTitle*)));
    connect(titles,SIGNAL(removeTitle(k9DVDTitle*)),mpeg,SLOT(removeTitle(k9DVDTitle*)));

    connect(titles,SIGNAL(addTitle(k9DVDTitle*)),DVDOption,SLOT(addTitle(k9DVDTitle*)));
    connect(titles,SIGNAL(removeTitle(k9DVDTitle*)),DVDOption,SLOT(removeTitle(k9DVDTitle*)));

#if QT_VERSION >= 0x050000 
        buttonBox()->addButton(QDialogButtonBox::Help);
        connect(button(QDialogButtonBox::Help),SIGNAL(clicked()), SLOT(helpClicked()));
#else
    connect(this,SIGNAL(helpClicked()),this,SLOT(helpClicked()));
#endif

    m_drives->scanDrives();

#if QT_VERSION >= 0x050000
    button(QDialogButtonBox::Help)->setText(i18n("Settings"));
    button(QDialogButtonBox::Help)->setToolTip(i18n("k9copy settings"));
    button(QDialogButtonBox::Help)->setIcon(QIcon(SmallIcon("configure")));
#else
    setButtonText(KDialog::Help,i18n("Settings"));
    setButtonToolTip(KDialog::Help,i18n("k9copy settings"));
    setButtonIcon(KDialog::Help,KIcon(SmallIcon("configure")));
#endif
    if (m_path !="") {
	source->setPath(m_path);
    }
    exec();
}

void k9Assistant::back() {
    if (currentPage() ==m_destinationItem) {
	for (int i=0;i< m_dvd->gettitleCount();i++) {
	    k9DVDTitle *title=m_dvd->gettitle(i);
	    title->setSelected(false);

	}
    }
    KAssistantDialog::back();
}

void k9Assistant::next() {
    QString err="";
    k9astTitles *titles=(k9astTitles*)m_titlesItem->widget();
    k9astStreams *streams=(k9astStreams*)m_streamsItem->widget();
    k9astDestination *adestination=(k9astDestination*)m_destinationItem->widget();

    k9astMp4 *mp4=(k9astMp4*) m_mp4Item->widget();
    titles->stopPreview();

    if (currentPage() == m_sourceItem) {
        k9astSource *source=(k9astSource*)m_sourceItem->widget();
        if (source->openDvd()) {
            titles->fill();
	    adestination->updatePaths();
	} else 
	    err=m_dvd->geterrMsg();
    } else if (currentPage() == m_destinationItem) {

	if (!adestination->isOk())
		err=i18n("The destination of the copy isn't valid");
	streams->selectOne(false);
	if (adestination->isMatroska())
	   mp4->setMatroskaCodecs();
	if ((m_destination== dstAvi) && !adestination->isMatroska()) {
            streams->selectOne(true);
	    streams->hideSubtitles(false);
	    mp4->setAviCodecs();
	} else if (m_destination==dstAudio) {
	    mp4->setAudio();
	    streams->hideSubtitles(true);
	} else
	    streams->hideSubtitles(false);
	streams->updateColumns();
    } else if (currentPage() == m_titlesItem) {
        titles->updateTitles();
        if (!titles->isOk())
	    err=i18n("You must select at least on title !");
	else
            titles->stopPreview();

    } else if (currentPage() == m_DVDOptionItem) {
        k9astDVDOption *options=(k9astDVDOption*)m_DVDOptionItem->widget();
        options->updateOptions();
        k9astMpeg * mpeg=(k9astMpeg*)m_mpegItem->widget();
	mpeg->updateFactor();
    } else if (currentPage() == m_streamsItem) {
        streams->updateStreams();
        switch (destination()) {
        case dstAvi:
	case dstAudio:
            setAppropriate(m_DVDOptionItem,false);
            setAppropriate(m_mpegItem,false);
            setAppropriate(m_mp4Item,true);
            break;
        case dstMpeg:
            setAppropriate(m_DVDOptionItem,false);
            setAppropriate(m_mpegItem,true);
            setAppropriate(m_mp4Item,false);
            break;
        case dstDVD:
            setAppropriate(m_DVDOptionItem,true);
            setAppropriate(m_mpegItem,true);
            setAppropriate(m_mp4Item,false);
            break;
        default:
            break;

        }

    }
    if (err=="") {
    	KAssistantDialog::next();
	streams->updateColumns();
    } else
 	k9Dialogs::error(err,i18n("cannot continue"));

}

void k9Assistant::helpClicked() {
    k9Settings settings(this,i18n("Settings"));
    settings.exec();
    k9astTitles *titles=(k9astTitles*)m_titlesItem->widget();
    titles->stopPreview();
    titles->loadPlayer();

}

void k9Assistant::accept() {
    k9astDestination *destination=(k9astDestination*)m_destinationItem->widget();

    //stop preview before copying the dvd 
    k9astTitles *titles=(k9astTitles*)m_titlesItem->widget();
    titles->unloadPreview();


    k9ExecCopy execCopy;
    execCopy.setCopyMenus(m_copyMenus);
    execCopy.setPath(destination->getPath());
    if (m_DVDdestination== dstFolder)
	execCopy.setOutput(k9ExecCopy::oFolder);
    else if (m_DVDdestination==dstIso)
	execCopy.setOutput(k9ExecCopy::oISO);
    else 
	execCopy.setOutput(k9ExecCopy::oDVD);
    execCopy.setDvd(m_dvd);

    switch (m_destination) {
	case dstDVD:
    	    execCopy.copyDVD();
	    break;
	case dstAvi:
	    execCopy.CreateMP4();
	    break;
	case dstMpeg:
	    execCopy.extractMPEG2();
	    break;
	case dstAudio:
	    execCopy.extractAudio();
	default:
	    break;
     }

    KAssistantDialog::accept();
}

void k9Assistant::reject() {
    k9astTitles *titles=(k9astTitles*)m_titlesItem->widget();
    titles->unloadPreview();
    KAssistantDialog::reject();
}
k9Assistant::~k9Assistant() {
    delete m_dvd;
}


