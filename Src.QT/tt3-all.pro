TEMPLATE = subdirs

SUBDIRS +=  \
    tt3 \
    tt3-db-api \
    tt3-db-sql \
    tt3-db-xml \
    tt3-gui \
    tt3-help \
    tt3-report \
    tt3-report-worksummary \
    tt3-skin-admin \
    tt3-skin-slim \
    tt3-tools-backup \
    tt3-tools-restore \
    tt3-util \
    tt3-ws

tt3.depends = tt3-gui tt3-ws tt3-util
tt3-gui.depends = tt3-help tt3-ws tt3-db-api tt3-util
tt3-ws.depends = tt3-db-api tt3-util
tt3-db-api.depends = tt3-util

tt3-db-sql.depends = tt3-db-api tt3-util
tt3-db-xml.depends = tt3-gui tt3-db-api tt3-util

tt3-tools-backup.depends = tt3-gui tt3-ws tt3-util
tt3-tools-restore.depends = tt3-gui tt3-ws tt3-util

tt3-report-worksummary.depends = tt3-report tt3-gui tt3-ws tt3-util
tt3-report.depends = tt3-gui tt3-ws tt3-util

tt3-skin-admin.depends = tt3-report tt3-gui tt3-ws tt3-util
tt3-skin-slim.depends = tt3-report tt3-gui tt3-ws tt3-util

tt3-help.depends = tt3-util
