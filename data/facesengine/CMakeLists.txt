#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Sometime Haar Cascade XML files used by Face Detection are not installed with OpenCV2 rules.
# With OpenCV3, as cv::face go to contrib repository, XML files can miss.
# So we always generalize installation of these files directly through digiKam,
# to be sure that Face Management always work.

if(ENABLE_OPENCV3)
    file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/opencv3/haarcascade_*.xml")
else()
    file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/opencv2/haarcascade_*.xml")
endif()

install(FILES ${files}
        DESTINATION ${DATA_INSTALL_DIR}/digikam/facesengine
)

# ---------------------------------------------------------------

# add_custom_command(OUTPUT face-funnel.data
#                    COMMAND ${CMAKE_COMMAND} -E tar xz ${CMAKE_CURRENT_SOURCE_DIR}/face-funnel.data.tgz)
#
# add_custom_target(extract_funnel_data ALL DEPENDS face-funnel.data)
#
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/face-funnel.data
#         DESTINATION ${DATA_INSTALL_DIR}/digikam/alignment-congealing
# )
#
# add_custom_command(OUTPUT flandmark_model.dat
#                    COMMAND ${CMAKE_COMMAND} -E tar xz ${CMAKE_CURRENT_SOURCE_DIR}/flandmark_model.dat.tgz)
#
# add_custom_target(extract_flandmark_data ALL DEPENDS flandmark_model.dat)
#
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/flandmark_model.dat
#         DESTINATION ${DATA_INSTALL_DIR}/digikam/alignment-flandmark)
