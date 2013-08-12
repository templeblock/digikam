/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 20013-08-05
 * Description : Tag Manager Tree View derived from TagsFolderView to implement
 *               a custom context menu and some batch view options, such as
 *               expanding multiple items
 *
 * Copyright (C) 2013 by Veaceslav Munteanu <veaceslav dot munteanu90 at gmail dot com>
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

#include <QModelIndex>
#include <QQueue>

#include "tagmngrtreeview.h"

namespace Digikam {

class TagMngrTreeView::TagMngrTreeViewPriv
{

public:
    TagMngrTreeViewPriv()
    {
        expandLevel = 0;
    }

    int expandLevel;
};

TagMngrTreeView::TagMngrTreeView(QWidget* parent, TagModel* model)
                : TagFolderView(parent, model), d(new TagMngrTreeViewPriv())
{

}

TagMngrTreeView::~TagMngrTreeView()
{

}

void TagMngrTreeView::slotExpandSelected()
{
    QModelIndexList list = selectionModel()->selectedIndexes();
    foreach(QModelIndex index, list)
    {
        expand(index);
    }
}

void TagMngrTreeView::slotExpandTree()
{
    QModelIndex root = this->model()->index(0,0);
    QItemSelectionModel* model = this->selectionModel();
    QModelIndexList selected = model->selectedIndexes();

    QQueue<QModelIndex> greyNodes;

    greyNodes.append(root);

    while(!greyNodes.isEmpty())
    {
        QModelIndex current = greyNodes.dequeue();

        if(!(current.isValid()))
        {
            continue;
        }
        int it = 0;
        QModelIndex child = current.child(it++,0);

        while(child.isValid())
        {

            if(this->isExpanded(child))
            {
                greyNodes.enqueue(child);
            }
            else
            {
                expand(child);
            }
            child = current.child(it++,0);
        }
    }
}

} // namespace Digikam