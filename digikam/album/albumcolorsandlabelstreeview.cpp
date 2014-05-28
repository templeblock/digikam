/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2014-05-17
 * Description : Colors and Labels Tree View.
 *
 * Copyright (C) 2014 Mohamed Anwer <mohammed dot ahmed dot anwer at gmail dot com>
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

#include "albumcolorsandlabelstreeview.h"

// QT includes

#include <QTreeWidget>
#include <QPainter>
#include <QDebug>

// KDE includes

#include <kapplication.h>
#include <KIconLoader>

// Local includes

#include "searchxml.h"
#include "searchtabheader.h"
#include "albummanager.h"
#include "albumtreeview.h"
#include "databaseconstants.h"

namespace Digikam
{

class ColorsAndLabelsTreeView::Private
{
public:
    Private():
        rating(0),
        labels(0),
        colors(0),
        isCheckableTreeView(false)
    {
        starPolygon << QPoint(0,  12);
        starPolygon << QPoint(10, 10);
        starPolygon << QPoint(14,  0);
        starPolygon << QPoint(18, 10);
        starPolygon << QPoint(28, 12);
        starPolygon << QPoint(20, 18);
        starPolygon << QPoint(22, 28);
        starPolygon << QPoint(14, 22);
        starPolygon << QPoint(6,  28);
        starPolygon << QPoint(8,  18);
    }

    QFont                rootFont;
    QFont                regularFont;
    QSize                iconSize;
    QSize                rootSizeHint;

    QPolygon             starPolygon;

    QTreeWidgetItem*     rating;
    QTreeWidgetItem*     labels;
    QTreeWidgetItem*     colors;

    bool                 isCheckableTreeView;
};

ColorsAndLabelsTreeView::ColorsAndLabelsTreeView(QWidget *parent, bool setCheckable) :
    QTreeWidget(parent), d(new Private)
{
    d->rootFont            = QFont("Times",18,-1,false);
    d->regularFont         = QFont("Times",12,-1,false);
    d->iconSize            = QSize(30 ,30);
    d->rootSizeHint        = QSize(1,40);
    d->isCheckableTreeView = setCheckable;

    setHeaderLabel("Colors And Labels");
    setUniformRowHeights(false);
    setIconSize(d->iconSize);
    initTreeView();

    if(d->isCheckableTreeView)
    {
        QTreeWidgetItemIterator it(this);
        while(*it)
        {
            if((*it)->parent())
            {
                (*it)->setFlags((*it)->flags()|Qt::ItemIsUserCheckable);
                (*it)->setCheckState(0, Qt::Unchecked);
            }
            ++it;
        }
    }
    else
    {
        setSelectionMode(QAbstractItemView::MultiSelection);
        connect(this,SIGNAL(itemSelectionChanged()),
                this,SLOT(slotSelectionChanged()));
    }
}

ColorsAndLabelsTreeView::~ColorsAndLabelsTreeView()
{
    delete d;
}

void ColorsAndLabelsTreeView::initTreeView()
{
    initRatingsTree();
    initLabelsTree();
    initColorsTree();
    expandAll();
    setRootIsDecorated(false);
}

void ColorsAndLabelsTreeView::initRatingsTree()
{
    d->rating = new QTreeWidgetItem(this);
    d->rating->setText(0, tr("Rating"));
    d->rating->setSizeHint(0,d->rootSizeHint);
    d->rating->setFont(0,d->rootFont);
    d->rating->setFlags(Qt::ItemIsEnabled);

    QPixmap goldenStarPixmap = QPixmap(30, 30);
    goldenStarPixmap.fill(Qt::transparent);

    QPainter p1(&goldenStarPixmap);
    p1.setRenderHint(QPainter::Antialiasing, true);
    p1.setBrush(QColor(0xff,0xd7,0x00));
    p1.setPen(palette().color(QPalette::Active, foregroundRole()));
    p1.drawPolygon(d->starPolygon, Qt::WindingFill);
    p1.end();

    QPixmap silverStarPixmap = QPixmap(30, 30);
    silverStarPixmap.fill(Qt::transparent);

    QPainter p2(&silverStarPixmap);
    p2.setRenderHint(QPainter::Antialiasing, true);
    p2.setBrush(QColor(204,204,204));
    p2.setPen(palette().color(QPalette::Active, foregroundRole()));
    p2.drawPolygon(d->starPolygon, Qt::WindingFill);
    p2.end();

    d->rating->setIcon(0,goldenStarPixmap);

    QTreeWidgetItem* noRate = new QTreeWidgetItem(d->rating);
    noRate->setText(0,tr("No Rating"));
    noRate->setIcon(0,silverStarPixmap.scaled(20,20,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
    noRate->setFont(0,d->regularFont);

    QStringList ratingList;
    ratingList << "1 Star" << "2 Stars" << "3 Stars" << "4 Stars" << "5 Stars";

    foreach(QString ratelabel, ratingList)
    {
        QTreeWidgetItem* rateWidget = new QTreeWidgetItem(d->rating);
        rateWidget->setText(0,ratelabel);
        rateWidget->setFont(0,d->regularFont);
        rateWidget->setIcon(0,goldenStarPixmap.scaled(20,20,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
        rateWidget->setSizeHint(0,QSize(1,20));
    }

}

void ColorsAndLabelsTreeView::initLabelsTree()
{
    d->labels = new QTreeWidgetItem(this);
    d->labels->setText(0, tr("Labels"));
    d->labels->setIcon(0,KIconLoader::global()->loadIcon("flag-green", KIconLoader::NoGroup, 30));
    d->labels->setSizeHint(0,d->rootSizeHint);
    d->labels->setFont(0,d->rootFont);
    d->labels->setFlags(Qt::ItemIsEnabled);

    QStringList labelSetNames;
    labelSetNames << "No Label" << "Rejected Item" << "Pending Item" << "Accepted Item";

    QStringList labelSetIcons;
    labelSetIcons << "emblem-unmounted" << "flag-red" << "flag-yellow" << "flag-green";

    foreach (QString label, labelSetNames) {
        QTreeWidgetItem* labelWidgetItem = new QTreeWidgetItem(d->labels);
        labelWidgetItem->setText(0,label);
        labelWidgetItem->setFont(0,d->regularFont);
        labelWidgetItem->setIcon(0,KIconLoader::global()->loadIcon(labelSetIcons.at(labelSetNames.indexOf(label)), KIconLoader::NoGroup, 20));
    }
}

void ColorsAndLabelsTreeView::initColorsTree()
{
    d->colors = new QTreeWidgetItem(this);
    d->colors->setText(0, tr("Colors"));
    QPixmap rootColorIcon(30, 30);
    rootColorIcon.fill(QColor(254,128,128));
    d->colors->setIcon(0,QIcon(rootColorIcon));
    d->colors->setSizeHint(0,d->rootSizeHint);
    d->colors->setFont(0,d->rootFont);
    d->colors->setFlags(Qt::ItemIsEnabled);

    QTreeWidgetItem* noColor = new QTreeWidgetItem(d->colors);
    noColor->setText(0,tr("No Color"));
    noColor->setFont(0,d->regularFont);
    noColor->setIcon(0,KIconLoader::global()->loadIcon("emblem-unmounted", KIconLoader::NoGroup, 20));

    QStringList colorSet;
    colorSet << "red" << "orange" << "yellow" << "darkgreen" << "darkblue" << "magenta" << "darkgray" << "black" << "lightgray";

    QStringList colorSetNames;
    colorSetNames << "Red"  << "Orange" << "Yellow" << "Green" << "Blue" << "Magenta" << "Gray" << "Black" << "White";

    foreach (QString color, colorSet) {
        QTreeWidgetItem* colorWidgetItem = new QTreeWidgetItem(d->colors);
        colorWidgetItem->setText(0,colorSetNames.at(colorSet.indexOf(color)));
        colorWidgetItem->setFont(0,d->regularFont);
        QPixmap colorIcon(18,18);
        colorIcon.fill(QColor(color));
        colorWidgetItem->setIcon(0,QIcon(colorIcon));
        colorWidgetItem->setSizeHint(0,QSize(1,20));
    }
}

QString ColorsAndLabelsTreeView::createXMLForCurrentSelection()
{
    SearchXmlWriter writer;

    if(!selectedRatings().isEmpty())
    {
        foreach (int val, selectedRatings())
        {
            writer.writeGroup();
            writer.setGroupOperator(SearchXml::Or);
            writer.writeField("rating",SearchXml::Equal);
            writer.writeValue(val);
            writer.finishField();

            if(!selectedLabels().isEmpty())
            {
                writer.writeField("tagid",SearchXml::InTree);
                writer.writeValue(selectedLabels());
                writer.finishField();
            }

            writer.finishGroup();
        }
    }
    else if(!selectedLabels().isEmpty())
    {
        writer.writeGroup();
        writer.setGroupOperator(SearchXml::Or);
        writer.writeField("tagid",SearchXml::InTree);
        writer.writeValue(selectedLabels());
        writer.finishField();
        writer.finishGroup();
    }
    else
    {
        writer.writeGroup();
        writer.finishGroup();
    }

    writer.finish();

    return writer.xml();
}

QList<int> ColorsAndLabelsTreeView::selectedRatings()
{
    QList<int> selectedRatings;
    foreach (QModelIndex index , selectedIndexes()) {
        if(index.parent().data().toString() == "Rating")
        {
            if(index.row() == 0)
                // to be similar to the one in the Advanced search
                selectedRatings << index.row()-1;
            else
                selectedRatings << index.row();
        }
    }

    return selectedRatings;
}

QList<int> ColorsAndLabelsTreeView::selectedLabels()
{
    QList<int> selectedLabels;
    foreach (QModelIndex index , selectedIndexes()) {
        if(index.parent().data().toString() == "Colors")
        {
            selectedLabels << index.row()+8;
        }

        if(index.parent().data().toString() == "Labels")
        {
            selectedLabels << index.row()+18;
        }
    }

    return selectedLabels;
}

SAlbum* ColorsAndLabelsTreeView::search(const QString& xml)
{
    SAlbum* album = AlbumManager::instance()->findSAlbum(SAlbum::getTemporaryTitle(DatabaseSearch::KeywordSearch));

    if (album)
    {
        AlbumManager::instance()->updateSAlbum(album, xml,
                                               SAlbum::getTemporaryTitle(DatabaseSearch::KeywordSearch),
                                               DatabaseSearch::AdvancedSearch);
    }
    else
    {
        album = AlbumManager::instance()->createSAlbum(SAlbum::getTemporaryTitle(DatabaseSearch::KeywordSearch),
                                                       DatabaseSearch::AdvancedSearch, xml);
    }

    return album;
}

void ColorsAndLabelsTreeView::slotSelectionChanged()
{
    QString xml   = createXMLForCurrentSelection();
    SAlbum* album = search(xml);

    if (album)
    {
        AlbumManager::instance()->setCurrentAlbums(QList<Album*>() << album);
    }
}

} // namespace Digikam