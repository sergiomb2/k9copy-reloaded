//
// C++ Implementation: k9dvdread
//
// Description: 
//
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "k9dvdread.h"
//PM DVDNAV #include "dvdread.h"
#include "dvdread/dvd_reader.h" 
#include "k9ifo2.h"

k9DVDRead::k9DVDRead(){
	m_dvd=NULL;
}

void k9DVDRead::deleteIfos() {
//	while (!m_ifos.isEmpty()) delete m_ifos.takeFirst();
	qDeleteAll(m_ifos);
	m_ifos.clear();
}
void k9DVDRead::deleteFiles() {
//	while (!m_files.isEmpty()) delete m_files.takeFirst();
	qDeleteAll(m_files);
	m_files.clear();
}

k9DVDRead::~k9DVDRead() {
	if (m_dvd !=NULL)
		close();
	deleteIfos();
	deleteFiles();
}

k9DVDFile::k9DVDFile(k9DVDRead *_dvd) {
	m_file=NULL;
	m_dvd=_dvd;
}

k9DVDFile::~k9DVDFile() {
	if (m_file != NULL)
		close();
}

/*!
    \fn k9DVDRead::openDevice(const QString & _device)
 */
void k9DVDRead::openDevice(const QString & _device) {
	m_dvd=DVDOpen(_device.toUtf8());
	//turn UDF cache off
	//DVDUDFCacheLevel(m_dvd, 0 );
        if (m_dvd) {
            k9Ifo2 *ifoz=new k9Ifo2(this);
            ifoz->openIFO(0);
            m_ifos.append(ifoz);
            ifo_handle_t *hifoz=ifoz->getIFO();
	    if(!hifoz) {
		m_dvd=NULL;
		return;
	    }
            int nrTS= hifoz->vmgi_mat->vmg_nr_of_title_sets;
    
            for (int iTS=1 ; iTS<=nrTS;iTS++) {
                k9Ifo2 *ifo=new k9Ifo2(this);
                if (!ifo->openIFO(iTS)) {
                    close();
                    return;
                }

                m_ifos.append(ifo);
            }
        }
}

k9Ifo2 *k9DVDRead::getIfo(int _num) {
            ///TODO:PTZ170219 woa, check on _num!!! it is a QList <k9Ifo2*> so might be protected anyhow. but still.
   k9Ifo2 *ifo=m_ifos[_num];

    return ifo;
}

QString k9DVDRead::getDiscId() {
uchar ID[17];
QString id="";
if (DVDDiscID(m_dvd,ID) !=-1) {
	ID[16]=0;
	id=QString::fromLatin1((const char*)ID);
}
return id;
}
/*!
    \fn k9DVDRead::close()
 */
void k9DVDRead::close()
{
	DVDClose(m_dvd);
	deleteIfos();
	deleteFiles();
	m_dvd=NULL;
}

bool k9DVDRead::opened() {
	return (m_dvd !=NULL);
}


k9DVDFile * k9DVDRead::openIfo(uint _vts) {
	k9DVDFile *file =new k9DVDFile(this);
	file->openIfo(_vts);
	m_files.append(file);
	return file;
}

k9DVDFile *k9DVDRead::openMenu(uint _vts) {
	k9DVDFile *file =new k9DVDFile(this);
	file->openMenu(_vts);
	m_files.append(file);
	return file;
}

k9DVDFile *k9DVDRead::openTitle(uint _vts) {
	k9DVDFile *file =new k9DVDFile(this);
	file->openTitle(_vts);
	m_files.append(file);
	return file;
}

/*!
    \fn k9DVDFile::openIfo(uint _vts)
 */
void k9DVDFile::openIfo(uint _vts) {
	m_file=DVDOpenFile(m_dvd->getDvd(),_vts,DVD_READ_INFO_FILE);
}


/*!
    \fn k9DVDFile::openMenu(uint _vts)
 */
void k9DVDFile::openMenu(uint _vts)
{
	m_file=DVDOpenFile(m_dvd->getDvd() ,_vts,DVD_READ_MENU_VOBS);
}


/*!
    \fn k9DVDFile::openTitle(uint _vts)
 */
void k9DVDFile::openTitle(uint _vts)
{
    	m_file=DVDOpenFile(m_dvd->getDvd(),_vts,DVD_READ_TITLE_VOBS);
}


/*!
    \fn k9DVDFile::close()
 */
void k9DVDFile::close()
{
    if (m_file != NULL) {
	DVDCloseFile(m_file);
	m_file=NULL;
    }
}


/*!
    \fn k9DVDFile::read(uchar *_buffer,uint32_t _size)
 */
int k9DVDFile::readBytes(uchar *_buffer,uint32_t _size)
{
	if (m_file != NULL)
		return  DVDReadBytes(m_file, _buffer, _size);
	else 
		return -1;
}

// Thread 1 (Thread 0x7fbb61fc9b40 (LWP 7763)):
// [KCrash Handler]
// #6  DVDReadBlocks (dvd_file=0x1, offset=587660, block_count=800, data=0x7ffc1c84fca0 "") at src/dvd_reader.c:1357
// TODO: PTZ161129 <===== here dvd_file=0x1 != NULL  !!!! is has been changed in another thread ?!
// #7  0x00005649eba9c12f in k9DVDFile::readBlocks (this=0x5649ed0a0d80, _sector=587660, _size=800, _buffer=0x7ffc1c84fca0 "") at /usr/src/k9copy-code/k9copy/src/core/k9dvdread.cpp:182
// #8  0x00005649eba943be in k9DVD::stream_vob (this=0x5649ece67a30, title=1, startblock=345260, lastblock=2772457, sc=0x7ffc1c9e0090) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:910
// #9  0x00005649eba93c85 in k9DVD::calcStreamSize (this=0x5649ece67a30, track=...) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:790
// #10 0x00005649eba934e4 in k9DVD::scandvd (this=0x5649ece67a30, device=..., _quickScan=false) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:667
// #11 0x00005649eba1c909 in k9Main::Open (this=0x5649ecd506e0) at /usr/src/k9copy-code/k9copy/src/main/k9main.cpp:513
// #12 0x00005649eba42b4f in k9Copy::fileOpen (this=0x5649ecc12b00) at /usr/src/k9copy-code/k9copy/src/main/k9copy.cpp:391
// #13 0x00005649eba87aac in k9Copy::qt_static_metacall (_o=0x5649ecc12b00, _c=QMetaObject::InvokeMetaMethod, _id=15, _a=0x7ffc1c9e1040) at /usr/src/k9copy-code/k9copy/bld/k9copy_automoc.dir/moc_k9copy_L6ZQNUUCNNIJGA.cpp:152
// #14 0x00007fbb7744bc89 in QMetaObject::activate (sender=sender@entry=0x5649ecca7d80, signalOffset=<optimized out>, local_signal_index=local_signal_index@entry=1, argv=argv@entry=0x7ffc1c9e1040) at kernel/qobject.cpp:3740
// TODO: PTZ170117 need  a semaphore... on m_file somewhere...
//#Thread 1 (Thread 0x7fd3e223a340 (LWP 29318)):
//#[KCrash Handler]
//##6  0x00007fd3fd2c5f0f in DVDReadBlocks (dvd_file=0x5632c3b7a480, offset=800, block_count=800, data=0x7fff76116fa0 "") at src/dvd_reader.c:1357
//#7  0x00005632c2c292af in k9DVDFile::readBlocks (this=0x5632c3b759c0, _sector=800, _size=800, _buffer=0x7fff76116fa0 "") at /usr/src/k9copy-code/k9copy/src/core/k9dvdread.cpp:195
//#8  0x00005632c2c2147c in k9DVD::stream_vob (this=0x5632c3b8f720, title=1, startblock=0, lastblock=11446, sc=0x7fff762a7390) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:931
//#9  0x00005632c2c20d43 in k9DVD::calcStreamSize (this=0x5632c3b8f720, track=...) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:811
//#10 0x00005632c2c205a1 in k9DVD::scandvd (this=0x5632c3b8f720, device=..., _quickScan=false) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:688

int k9DVDFile::readBlocks(uint32_t _sector,uint32_t _size,uchar*_buffer) {
	if (m_file != NULL)  {
	   return DVDReadBlocks(m_file,_sector,_size,_buffer);
	}
	else 
		return -1;
}

//PTZ170215 a debugging session from first call...
// #0  0x00007f9611954dae in ?? () from target:/usr/lib/x86_64-linux-gnu/libdvdcss.so.2
// #1  0x00007f9611952880 in dvdcss_seek () from target:/usr/lib/x86_64-linux-gnu/libdvdcss.so.2
// #2  0x00007f963794fc29 in DVDReadBlocks () from target:/usr/lib/x86_64-linux-gnu/libdvdread.so.4
// #3  0x0000560ad8a9920f in k9DVDFile::readBlocks (this=0x560adabb0cf0, _sector=314400, _size=800, _buffer=0x7ffd1e4e5430 "") at /usr/src/k9copy-code/k9copy/src/core/k9dvdread.cpp:203
// #4  0x0000560ad8a913dc in k9DVD::stream_vob (this=0x560ada979eb0, title=1, startblock=0, lastblock=3150687, sc=0x7ffd1e675820) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:931
// #5  0x0000560ad8a90ca3 in k9DVD::calcStreamSize (this=0x560ada979eb0, track=...) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:811
// #6  0x0000560ad8a90501 in k9DVD::scandvd (this=0x560ada979eb0, device=..., _quickScan=false) at /usr/src/k9copy-code/k9copy/src/core/k9dvd.cpp:688
// #7  0x0000560ad8a19987 in k9Main::Open (this=0x560ada9bb3b0) at /usr/src/k9copy-code/k9copy/src/main/k9main.cpp:513
// #8  0x0000560ad8a3fbcd in k9Copy::fileOpen (this=0x560ada684f20) at /usr/src/k9copy-code/k9copy/src/main/k9copy.cpp:391
// #9  0x0000560ad8a84b2a in k9Copy::qt_static_metacall (_o=0x560ada684f20, _c=QMetaObject::InvokeMetaMethod, _id=15, _a=0x7ffd1e6767c0) at /usr/src/k9copy-code/k9copy/bld/k9copy_automoc.dir/moc_k9copy_L6ZQNUUCNNIJGA.cpp:152
// #10 0x00007f96320205e9 in QMetaObject::activate(QObject*, int, int, void**) () from target:/usr/lib/x86_64-linux-gnu/libQt5Core.so.5
// #11 0x00007f96328d11c2 in QAction::triggered(bool) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #12 0x00007f96328d3bb0 in QAction::activate(QAction::ActionEvent) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #13 0x00007f96329c1a0d in ?? () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #14 0x00007f96329c1c44 in QAbstractButton::mouseReleaseEvent(QMouseEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #15 0x00007f9632a8dd3a in QToolButton::mouseReleaseEvent(QMouseEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #16 0x00007f963291f278 in QWidget::event(QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #17 0x00007f9632a8de19 in QToolButton::event(QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #18 0x00007f96328d7b8c in QApplicationPrivate::notify_helper(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #19 0x00007f96328e01fd in QApplication::notify(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #20 0x00007f9631ff49e0 in QCoreApplication::notifyInternal2(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Core.so.5
// #21 0x00007f96328de8ad in QApplicationPrivate::sendMouseEvent(QWidget*, QMouseEvent*, QWidget*, QWidget*, QWidget**, QPointer<QWidget>&, bool) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #22 0x00007f9632939906 in ?? () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #23 0x00007f963293c313 in ?? () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #24 0x00007f96328d7b8c in QApplicationPrivate::notify_helper(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #25 0x00007f96328df341 in QApplication::notify(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5
// #26 0x00007f9631ff49e0 in QCoreApplication::notifyInternal2(QObject*, QEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Core.so.5
// #27 0x00007f963233cb03 in QGuiApplicationPrivate::processMouseEvent(QWindowSystemInterfacePrivate::MouseEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
// #28 0x00007f963233e685 in QGuiApplicationPrivate::processWindowSystemEvent(QWindowSystemInterfacePrivate::WindowSystemEvent*) () from target:/usr/lib/x86_64-linux-gnu/libQt5Gui.so.5
// #29 0x00007f963231c61b in QWindowSystemInterface::sendWindowSystemEvents(QFlags<QEventLoop::ProcessEventsFlag>) () from target:/usr/lib/x86_64-linux-gnu/libQt5Gui.so.5


