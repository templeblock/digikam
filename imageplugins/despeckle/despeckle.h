/* ============================================================
 * File  : despeckle.cpp
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 * Date  : 2004-08-24
 * Description : Despeckle image filter for ImageEditor
 * 
 * Copyright 2004 by Gilles Caulier
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

#ifndef DESPECKLE_H
#define DESPECKLE_H

// KDE include.

#include <kdialogbase.h>

class QPushButton;
class QCheckBox;

class KIntNumInput;

namespace Digikam
{
class ImagePreviewWidget;
}

namespace DigikamDespeckleFilterImagesPlugin
{

class DespeckleDialog : public KDialogBase
{
    Q_OBJECT

public:

    DespeckleDialog(QWidget* parent);
    ~DespeckleDialog();

protected:

    void closeEvent(QCloseEvent *e);
   
    
private:

    QWidget      *m_parent;
    QPushButton  *m_helpButton;
    
    KIntNumInput *m_radiusInput;
    KIntNumInput *m_blackLevelInput;
    KIntNumInput *m_whiteLevelInput;
    
    QCheckBox    *m_useAdaptativeMethod;
    QCheckBox    *m_useRecursiveMethod;
    
    Digikam::ImagePreviewWidget *m_imagePreviewWidget;
    
    void despeckle(uint* data, int w, int h, int despeckle_radius, 
                   int black_level, int white_level, 
                   bool adaptativeFilter, bool recursiveFilter);
    
private slots:

    void slotHelp();
    void slotUser1();
    void slotEffect();
    void slotOk();
};

}  // NameSpace DigikamDespeckleFilterImagesPlugin

#endif /* DESPECKLE_H */
