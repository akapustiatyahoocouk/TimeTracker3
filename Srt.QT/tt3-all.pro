TEMPLATE = subdirs

SUBDIRS +=  \
    tt3 \
    tt3-gui \
    tt3-util

tt3.depends = tt3-gui tt3-util
tt3-gui.depends = tt3-util
