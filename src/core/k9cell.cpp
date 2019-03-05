/**************************************************************************
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
#include "k9cell.h"
#include "k9dvdtitleset.h"

void k9VobuList::append(k9Vobu * _vobu) {
    QMutexLocker locker(&mutex);
    QList<k9Vobu *>::append(_vobu);
}

int k9VobuList::count() {
    QMutexLocker locker(&mutex);
    return QList<k9Vobu *>::count();
}

k9Vobu * k9VobuList::at(int _i) {
    QMutexLocker locker(&mutex);
    return this->QList<k9Vobu *>::at(_i);
}

k9Vobu * k9VobuList::first() {
    QMutexLocker locker(&mutex);
    return QList<k9Vobu *>::first();
}

k9Vobu * k9VobuList::last() {
    QMutexLocker locker(&mutex);
    return QList<k9Vobu *>::last();
}




bool k9VobuList::compareItems (const k9Vobu *item1,const k9Vobu *item2 ) {
    return(item1->oldSector < item2->oldSector);
}

void k9VobuList::clearList() {
    QMutexLocker locker(&mutex);
    //   while (!isEmpty()) delete takeFirst();
    qDeleteAll(*this);
    clear();
}

k9Vobu * k9VobuList::findVobu(uint32_t sector) {
    uint32_t c=count();
    c--;
    //if (c >0) {
    k9Vobu *v1,*v2;
    v1=first();
    v2=last();
    if (v1==NULL)
        return NULL;

    if (sector >= v1->oldSector && sector <= v2->oldSector)
        return findVobu(sector,0,c);
    else
        return NULL;
    //}
}

k9Vobu * k9VobuList::findVobu(uint32_t sector, uint32_t start, uint32_t end) {
    long lstart=start;
    long lend=end;
    while (lstart <=lend) {
        long  m =(lstart+lend)/2;
        k9Vobu *v = (k9Vobu *)at(m);       
        if ( v->oldSector == sector) {
            return v;
        } else if (  v->oldSector >sector) {
            lend = m-1;
        } else {
            lstart=m+1;
        }
    }
    return NULL;
}

uint32_t k9Cell::getnewSize() {
    uint32_t size = 0;
    //for (k9Vobu * vobu=vobus.first();vobu;vobu=vobus.next())
    for (int i=0; i < vobus.count(); i++) {
        k9Vobu *vobu = vobus.at(i);
        size += vobu->size;
    }
    return size;
}

int k9Cell::getStreamID(int type) {
    int abase=0;
    if (type >= 0x80 && type <= 0x87) {
        // AC3 audio
        abase = 0x80;
    } else if (type >= 0x88 && type <= 0x8f) {
        // DTS audio
        abase = 0x88;
    } else if (type >= 0xa0 && type <= 0xbf) {
        // LPCM audio
        abase = 0xa0;
    } else if (type >= 0xc0 && type <= 0xdf) {
        // MPEG audio
        abase = 0xc0;
    } else if (type >=0x20 && type <=0x3f) {
        //subpicture;
        abase =0x20;
    }
    return (type-abase );

}


streamType_t k9Cell::identifyStream( uchar *buffer,int *packetType ) {
    *packetType = buffer[17];

    if ( (*packetType >= 0xE0) && (*packetType <= 0xEF) ) {                // video streams
        return stVideo;
    } else if ( *packetType == 0xBB ) {                                      // system header
        return stOther;
    } else if ( *packetType == 0xBE ) {                                      // padding
        return stOther;
    } else if ( *packetType == 0xBF ) {                                      // nav pack
        return stOther;
    } else if ( (*packetType >= 0xC0) && (*packetType <= 0xDF) ) {           // mpeg audio
        return stAudio;
    } else if ( *packetType == 0xBD ) {                                      // private stream, check content
        *packetType = buffer[23+buffer[22]];
        if (( (*packetType >=0x80) && (*packetType <=0x8f)) || ((*packetType >=0xa0) && (*packetType <=0xa7)) || ((*packetType >=0xc0) && (*packetType <=0xdf)))
            return stAudio;
        if ( (*packetType >=0x20) && (*packetType <=0x3f))
            return stSubpicture;
        return stOther;
    } else
        return stOther;

}



bool k9Cell::isNavPack (uchar *ptr, long _len) {
    uint32_t start_code;
    //uchar *ptr=_ptr;
    if(_len < 0x403) {
        qDebug("ignoring this NavPack::short buffer@0x%x [0x%x < 0x403]"
                   , ptr, (unsigned int)_len
                   );
        return 0;
    }
    if ((ptr [0]!=0) || (ptr [1] !=0) || (ptr [2] != 0x01) || (ptr [3] != 0xba))
        return 0;

    if ((ptr [4] & 0xc0) != 0x40)
        return 0;

    // ptr += 14;

    start_code  = (uint32_t) (ptr [14]) << 24;
    start_code |= (uint32_t) (ptr [15]) << 16;
    start_code |= (uint32_t) (ptr [16]) <<  8;
    start_code |= (uint32_t) (ptr [17]);

    if (start_code != 0x000001bb)
        return 0;

    //  ptr += 24;

    start_code  = (uint32_t) (ptr [0x26]) << 24;
    start_code |= (uint32_t) (ptr [0x27]) << 16;
    start_code |= (uint32_t) (ptr [0x28]) <<  8;
    start_code |= (uint32_t) (ptr [0x29]);

    if (start_code != 0x000001bf)
        return 0;

    //  ptr += 986;

    start_code  = (uint32_t) (ptr [0x400]) << 24;
    start_code |= (uint32_t) (ptr [0x401]) << 16;
    start_code |= (uint32_t) (ptr [0x402]) <<  8;
    start_code |= (uint32_t) (ptr [0x403]);

    if (start_code != 0x000001bf)
        return 0;

    return 1;
}

k9Cell::k9Cell() {
//    vobus.setAutoDelete(true);
    //m_mutex.lock();
    numVobu=0; //// TODO: PTZ161108 tied to vobus.size()
    //m_mutex.unlock();   
    vts=0;
    pgc=0;
    vob=0;
    startSector=0;
    lastSector=0;
    //TO REMOVE    oldStartSector=0;
    //TO REMOVE    oldLastSector=0;
    //TO REMOVE    newSize=0;
    id=0;
    selected=false;
    copied=false;
    //TO REMOVE    nbVideoNew=0;
    //TO REMOVE    nbVideoOld=0;
    angleBlock=angleNone;
}


k9Cell::~k9Cell() {
    vobus.clearList();
}

//add a title in the title list (titles using this cell)
void k9Cell::addTitle( k9DVDTitle *_title) {
    m_titles.append( _title);
}

bool k9Cell::getforceFactor() {
    bool m_forced=false;
    for (int i=0; i< m_titles.count() && !m_forced ;i++) {
        k9DVDTitle *title=m_titles.at(i);
        if (title->getforceFactor())
            m_forced=true;
    }
    return m_forced;
}

// returns the shrink factor.
float k9Cell::getFactor() {
    float factor=0;
    for (int i=0; i< m_titles.count() ;i++) {
        k9DVDTitle *title=m_titles.at(i);
        if ((title->getfactor()< factor || factor==0 )  && title->getforceFactor() )
            factor=title->getfactor();
    }
    return factor;
}


//looking for the end reference frame adress block
void k9Cell::addRefStream(k9Vobu *_vobu,uchar *_buffer,uint32_t _position) {
    if (_vobu->firstRefOk && _vobu->secondRefOk && _vobu->thirdRefOk)
        return;
    //offset of frame
    uint start=0x16+_buffer[0x16]+1;
    //PES Length
    uint length=_buffer[0x12];
    length=(length <<8 ) + _buffer[0x13];

    //end of PES
    uint end=0x14+length;

    if ( _vobu->frameType!=0x18) {
        if (!_vobu->firstRefOk)
            _vobu->firstRef=_position;
        else if (!_vobu->secondRefOk)
            _vobu->secondRef=_position;
        else if (!_vobu->thirdRefOk)
            _vobu->thirdRef=_position;
    }

    for (uint i=start;i <end-7;i++) {
        //if last picture is not a B-Frame, keep offset of last video

        //looking for start code picture
        if ((_buffer[i-5] ==0) &&( _buffer[i-4] ==0)	&&( _buffer[i-3] ==1)&&( _buffer[i-2] ==0)) {
            uchar frameType=_buffer[i] &0x38;
            //  if frameType != B-Frame and a reference picture was found
            if ((_vobu->frameType!=0x18) && (_vobu->frameType!=0)) {
                if (!_vobu->firstRefOk)  {
                    _vobu->firstRefOk=true;
                    _vobu->firstRef=_position;
                } else if (!_vobu->secondRefOk) {
                    _vobu->secondRefOk=true;
                    _vobu->secondRef=_position;
                } else if (!_vobu->thirdRefOk) {
                    _vobu->thirdRefOk=true;
                    _vobu->thirdRef=_position;
                }
            }
            //keep the current frame type
            _vobu->frameType=frameType;
        }
    }

}

k9Vobu * k9Cell::addVobu(uint32_t _sector) {
    k9Vobu * vobu = new k9Vobu(this,_sector);
    vobus.append(vobu);
    return vobu;
}
//Thread 6 (Thread 0x7fb5e77ae700 (LWP 21807)):
//[KCrash Handler]
//#6  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:58
//#7  0x00007fb60cb7140a in __GI_abort () at abort.c:89
//#8  0x00007fb60d80bdd1 in qt_message_fatal (context=..., message=<synthetic pointer>) at global/qlogging.cpp:1648
//#9  QMessageLogger::fatal (this=this@entry=0x7fb5e77ad4f0, msg=msg@entry=0x7fb60dac1630 "ASSERT failure in %s: \"%s\", file %s, line %d") at global/qlogging.cpp:790
//#10 0x00007fb60d807191 in qt_assert_x (where=<optimized out>, what=<optimized out>, file=<optimized out>, line=<optimized out>) at global/qglobal.cpp:3007
//vobus.size < i=62   <====// TODO: PTZ161108
//#//11 0x0000561a4f15f9ed in QList<k9Vobu*>::at (this=0x561a50ddf270, i=62) at /usr/include/x86_64-linux-gnu/qt5/QtCore/qlist.h:531
//#12 0x0000561a4f15f207 in k9Cell::addNewVobus
//(this=0x561a50ddf270, _buffer=0x7fb5d83e89d0 "", _len=2048, _position=23495, _vobNum=0, _vobPos=48117760)
//at /usr/src/k9copy-code/k9copy/src/core/k9cell.cpp:291
//#13 0x0000561a4f18ce11 in k9DVDBackup::getOutput (this=0x561a501c4720, _buf=0x7fb5d82051d0 "", _buflen=2113536) at /usr/src/k9copy-code/k9copy/src/backup/k9dvdbackup.cpp:430
//#/14 0x0000561a4f1b6810 in k9bgUpdate::run (this=0x561a502fc5c0) at /usr/src/k9copy-code/k9copy/src/vamps/k9vamps.cpp:1180
//#15 0x00007fb60d824d78 in QThreadPrivate::start (arg=0x561a502fc5c0) at thread/qthread_unix.cpp:341
//#16 0x00007fb6135fc464 in start_thread (arg=0x7fb5e77ae700) at pthread_create.c:333
//#17 0x00007fb60cc259df in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:105


void k9Cell::addNewVobus(char *_buffer, uint32_t _len, uint32_t _position, int _vobNum, long _vobPos) {
    uint32_t start = _position;//lastSector - _len ;
    k9Vobu *vobu;
    bool _bad_vobu = false;
    
    for (uint32_t i = 0 ; i < _len; i += DVD_BLOCK_LEN) {
        if (isNavPack((uchar*)_buffer + i, _len - i)) {
            //PTZ161107                 ///remapOffset : sector not found
            if(numVobu >= vobus.size()) {
                qDebug("error"
                       "::NavPack detected but no vobus[#%d] stacked"
                       "::remains %d DVD_BLOCKS_LEN"
                       "::keeping requested VOBU #%d"
                       , numVobu
                       , (_len - i) / DVD_BLOCK_LEN
                       , _vobNum
                       );
                _bad_vobu = true;
                vobu = addVobu(lastSector);
            } else {
                vobu = vobus.at(numVobu);
            }
            numVobu++;
            
            vobu->newSector = i / DVD_BLOCK_LEN + start;
            vobu->vobNum = _vobNum;
            vobu->vobPos = _vobPos;
            //QString c;
            //c.sprintf("vobu : %d  old: %d  new :%d",numVobu-1,vobu->oldSector,vobu->newSector);
            //qDebug (c.latin1());
            //qDebug("vobu : %d  old: %d  new :%d", numVobu-1, vobu->oldSector, vobu->newSector);
        } else {
            int packetType, id;
            streamType_t st = k9Cell::identifyStream((uchar*)_buffer + i, &packetType);
            vobu = vobus.at(numVobu - 1);
                        
            switch (st) {
            case stAudio:
                id = k9Cell::getStreamID(packetType);
                if (vobu->firstAudio[id]==-1) {
                    vobu->firstAudio[id]= ((i/ DVD_BLOCK_LEN) + start) - vobu->newSector;
                }
                break;
            case stSubpicture:
                id = k9Cell::getStreamID(packetType);
                if ((id >=0) && (id<32)) {
                    if (vobu->firstSubp[id]==-1) {
                        vobu->firstSubp[id]= ((i / DVD_BLOCK_LEN)+start) - vobu->newSector;
                    }
                }
                break;
            case stVideo:
                addRefStream(vobu, (uchar*)_buffer+i, (i / DVD_BLOCK_LEN + start) - vobu->newSector);
                if (vobu->firstVideo==-1) {
                    vobu->firstVideo =  ((i / DVD_BLOCK_LEN) + start) - vobu->newSector;
                }
                //TO REMOVE                nbVideoNew++;
                break;
            default:
		break;
            }
        }
        vobu->size = _position - vobu->newSector;
        // �v�ifier
        lastSector = _position;
    }

}

k9Vobu::k9Vobu(k9Cell *_parent,uint32_t _oldSector) {
    parent=_parent;
    oldSector = _oldSector;
    newSector = 0;
    size=0;
    for (int i=0;i<8;i++)
        firstAudio[i]=-1;
    for (int i=0;i<32;i++)
        firstSubp[i]=-1;
    firstVideo=-1;
    empty=false;
    vobPos=0;
    vobNum=0;
    frameType=0;
    firstRef=0;
    secondRef=0;
    thirdRef=0;
    firstRefOk=false;
    secondRefOk=false;
    thirdRefOk=false;
}


/*!
\fn k9Cell::findVobu(int _oldSector)
*/
k9Vobu * k9Cell::findVobu(uint32_t _oldSector) {
    return vobus.findVobu(_oldSector) ;
}


k9Vobu::~k9Vobu() {}


k9Cell* k9CellList::addCell(int _vts,int _pgc, int _vob) {
    k9Cell* cell=new k9Cell;
    cell->cellList=this;
    cell->vts=_vts;
    cell->vob=_vob;
    cell->pgc=_pgc;

    if (count()!=0) {
        k9Cell *prev=(k9Cell*)last();
        if (prev->vts==_vts)  {
            cell->startSector= prev->lastSector +1;
            if (_pgc!=prev->pgc)
                cell->id=1;
            else
                cell->id=prev->id+1;
        }
    } else {
        cell->startSector = 0;
        cell->id=1;
    }
    append (cell);
    return(cell);

}

k9Vobu *k9CellList::findVobu(uint32_t sector) {
    k9Vobu *vobu=NULL;
    k9Cell *cell=findCell(sector);
    if (cell==NULL)
        return NULL;
    else
        return cell->findVobu(sector);

    int nbCells=count();
    for (int i=0;i <nbCells;i++) {
        k9Cell * cell = (k9Cell*)at(i);
        vobu = cell->findVobu(sector);
        if (vobu !=NULL) {
            return vobu;
        }
        cell = (k9Cell*)at(nbCells-1-i);
        vobu = cell->findVobu(sector);
        if (vobu !=NULL) {
            return vobu;
        }


    }
    return vobu;
}

k9Cell * k9CellList::findCell(uint32_t sector) {
    return findCell(sector,0,count()-1);
}

k9Cell * k9CellList::findCell(uint32_t sector, uint32_t start, uint32_t end) {

    long lstart=start;
    long lend=end;

    while (lstart <=lend) {
        long  m =(lstart+lend)/2;
        k9Cell *cell = (k9Cell *)at(m);
        k9Vobu *fv=cell->vobus.first();
        k9Vobu *lv=cell->vobus.last();


        if ( sector >=fv->oldSector && sector<= lv->oldSector) {
            return cell;
        } else if ( fv->oldSector >sector) {
            lend = m-1;
        } else {
            lstart=m+1;
        }
    }
    return NULL;
}

k9DVDTitleset * k9Cell::getTitleset() {
    k9DVDTitle *title=m_titles.at(0);
    return title->gettitleset();
}
