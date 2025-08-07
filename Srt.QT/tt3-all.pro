TEMPLATE = subdirs

SUBDIRS +=  \
    tt3 \
    tt3-db-api \
    tt3-gui \
    tt3-skin-admin \
    tt3-util \
    tt3-ws

tt3.depends = tt3-gui tt3-ws tt3-db-api tt3-util
tt3-skin-admin.depends = tt3-gui tt3-util
tt3-gui.depends = tt3-util
tt3-ws.depends = tt3-db-api tt3-util
tt3-db-api.depends = tt3-util
