#
# Copyright (c) 2010-2016 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

add_subdirectory(advancedrename)
add_subdirectory(queuemanager)
add_subdirectory(imageeditor)
add_subdirectory(lighttable)
add_subdirectory(importui)
add_subdirectory(slideshow)
add_subdirectory(presentation)
add_subdirectory(assistants)
add_subdirectory(setup)
add_subdirectory(fuzzysearch)
add_subdirectory(maintenance)
add_subdirectory(searchwindow)
add_subdirectory(metadataedit)
add_subdirectory(kdesupport)
add_subdirectory(facemanagement)

if(${MARBLE_FOUND})
    add_subdirectory(geolocation)
endif()
