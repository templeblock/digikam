/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-02-13
 * Description : tabbed queue items list.
 *
 * Copyright (C) 2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "queuetab.h"
#include "queuetab.moc"

// KDE includes.

#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>

// Local includes.

#include "queuelist.h"

namespace Digikam
{

class QueueTabPriv
{

public:

    QueueTabPriv()
    {
    }

};

QueuePool::QueuePool(QWidget *parent)
        : KTabWidget(parent), d(new QueueTabPriv)
{
    setTabBarHidden(false);
    addQueue();
}

QueuePool::~QueuePool()
{
    delete d;
}

void QueuePool::addQueue()
{
    QueueListView* queue = new QueueListView(this);
    addTab(queue, SmallIcon("vcs_diff"), QString("#%1").arg(count()+1));

    connect(queue, SIGNAL(signalImageListChanged()),
            this, SIGNAL(signalImageListChanged()));

    connect(queue, SIGNAL(signalItemSelected(const AssignedBatchTools&)),
            this, SIGNAL(signalItemSelected(const AssignedBatchTools&)));
}

QueueListView* QueuePool::currentQueue() const
{
    return (dynamic_cast<QueueListView*>(currentWidget()));
}

void QueuePool::slotRemoveCurrentQueue()
{
    removeTab(indexOf(currentQueue()));
    if (count() == 0)
        addQueue();

    emit signalQueuePoolChanged();
}

void QueuePool::slotClearList()
{
    currentQueue()->slotClearList();
}

void QueuePool::slotRemoveSelectedItems()
{
    currentQueue()->slotRemoveSelectedItems();
}

void QueuePool::slotRemoveItemsDone()
{
    currentQueue()->slotRemoveItemsDone();
}

void QueuePool::slotAddItems(const ImageInfoList& list, const ImageInfo &current)
{
    currentQueue()->slotAddItems(list, current);
}

void QueuePool::slotAssignedToolsChanged(const AssignedBatchTools& tools4Item)
{
    currentQueue()->slotAssignedToolsChanged(tools4Item);
}

}  // namespace Digikam
