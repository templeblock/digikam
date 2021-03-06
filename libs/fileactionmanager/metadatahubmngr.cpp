/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2015-08-21
 * Description : metadatahub manager
 *
 * Copyright (C) 2015 by Veaceslav Munteanu <veaceslav dot munteanu90 at gmail dot com>
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

#include "metadatahubmngr.h"

// Qt includes

#include <QMutex>
#include <QDebug>

// Local includes

#include "imageinfo.h"
#include "metadatahub.h"
#include "imageinfolist.h"
#include "metadatasynchronizer.h"

namespace Digikam
{

QPointer<MetadataHubMngr> MetadataHubMngr::internalPtr = QPointer<MetadataHubMngr>();

class MetadataHubMngr::Private
{
public:

    Private(): mutex()
    {
    }

    ImageInfoList pendingItems;
    QMutex mutex;
};

MetadataHubMngr* MetadataHubMngr::instance()
{
    if (internalPtr.isNull())
        internalPtr = new MetadataHubMngr();

    return internalPtr;
}

MetadataHubMngr::~MetadataHubMngr()
{
    delete d;
}

void MetadataHubMngr::addPending(ImageInfo &info)
{
    QMutexLocker locker(&d->mutex);

    if (!d->pendingItems.contains(info))
        d->pendingItems.append(info);

    emit signalPendingMetadata(d->pendingItems.size());
}

void MetadataHubMngr::slotApplyPending()
{
    QMutexLocker lock(&d->mutex);

    if (d->pendingItems.isEmpty())
        return;

    ImageInfoList infos(d->pendingItems);
    d->pendingItems.clear();

    emit signalPendingMetadata(0);

    MetadataSynchronizer* const tool = new MetadataSynchronizer(infos,
                                                                MetadataSynchronizer::WriteFromDatabaseToFile);
    tool->start();
}

MetadataHubMngr::MetadataHubMngr()
    : d(new Private())
{
}

} // namespace Digikam
