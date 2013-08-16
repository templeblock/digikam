/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-05-16
 * Description : fingerprints generator
 *
 * Copyright (C) 2008-2013 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2012      by Andi Clemens <andi dot clemens at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "fingerprintsgenerator.moc"

// Qt includes

#include <QString>
#include <QTimer>
#include <QDir>
#include <QFileInfo>

// KDE includes

#include <kcodecs.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstandardguiitem.h>

// Local includes

#include "dimg.h"
#include "albumdb.h"
#include "albummanager.h"
#include "databaseaccess.h"
#include "metadatasettings.h"
#include "maintenancethread.h"

namespace Digikam
{

class FingerPrintsGenerator::Private
{
public:

    Private() :
        rebuildAll(true),
        thread(0)
    {
    }

    bool               rebuildAll;

    QStringList        allPicturesPath;

    AlbumList          albumList;

    MaintenanceThread* thread;
};

FingerPrintsGenerator::FingerPrintsGenerator(const bool rebuildAll, const AlbumList& list, ProgressItem* const parent)
    : MaintenanceTool("FingerPrintsGenerator", parent),
      d(new Private)
{
    setLabel(i18n("Finger-prints"));
    ProgressManager::addProgressItem(this);

    d->albumList  = list;
    d->rebuildAll = rebuildAll;
    d->thread     = new MaintenanceThread(this);

    // NOTE: A part of finger-print task is done outside dedicated thread. We need to wait until it.
    //       So we don't use MaintenanceThread::signalCompleted() to know if task is complete. 
    //       We will check if all process items are done through slotAdvance().

    connect(d->thread, SIGNAL(signalAdvance(QPixmap)),
            this, SLOT(slotAdvance(QPixmap)));
}

FingerPrintsGenerator::~FingerPrintsGenerator()
{
    delete d;
}

void FingerPrintsGenerator::slotCancel()
{
    d->thread->cancel();
    MaintenanceTool::slotCancel();
}

void FingerPrintsGenerator::slotStart()
{
    MaintenanceTool::slotStart();

    if (d->albumList.isEmpty())
    {
        d->albumList = AlbumManager::instance()->allPAlbums();
    }

    // Get all digiKam albums collection pictures path, depending of d->rebuildAll flag.

    for (AlbumList::ConstIterator it = d->albumList.constBegin();
         !canceled() && (it != d->albumList.constEnd()); ++it)
    {
        d->allPicturesPath += DatabaseAccess().db()->getItemURLsInAlbum((*it)->id());

        if (!d->rebuildAll)
        {
            QStringList dirty = DatabaseAccess().db()->getDirtyOrMissingFingerprintURLs();

            foreach(QString path, dirty)
            {
                if (dirty.contains(path))
                {
                    d->allPicturesPath.removeAll(path);
                }
            }
        }
    }

    if (d->allPicturesPath.isEmpty())
    {
        slotDone();
        return;
    }

    setTotalItems(d->allPicturesPath.count());

    d->thread->setUseMultiCore(true);
    d->thread->generateFingerprints(d->allPicturesPath);
    d->thread->start();
}

void FingerPrintsGenerator::slotAdvance(const QPixmap& pix)
{
    setThumbnail(pix);
    if (advance(1))
        slotDone();
}

void FingerPrintsGenerator::slotDone()
{
    // Switch on scanned for finger-prints flag on digiKam config file.
    KGlobal::config()->group("General Settings").writeEntry("Finger Prints Generator First Run", true);

    MaintenanceTool::slotDone();
}

}  // namespace Digikam
