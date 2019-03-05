
if(QT5_BUILD)
ecm_install_icons( ${DATA_INSTALL_DIR}/k9copy/icons)
else()
kde4_install_icons( ${DATA_INSTALL_DIR}/k9copy/icons)
endif()

