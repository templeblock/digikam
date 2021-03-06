/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-28-04
 * Description : first run assistant dialog
 *
 * Copyright (C) 2009-2016 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef PREVIEW_PAGE_H
#define PREVIEW_PAGE_H

// Local includes

#include "firstrundlgpage.h"

namespace Digikam
{

class PreviewPage : public FirstRunDlgPage
{
    Q_OBJECT

public:

    explicit PreviewPage(FirstRunDlg* const dlg);
    ~PreviewPage();

    void saveSettings();

private:

    class Private;
    Private* const d;
};

}   // namespace Digikam

#endif /* PREVIEW_PAGE_H */
