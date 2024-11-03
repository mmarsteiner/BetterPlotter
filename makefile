# ----------------------------
# Makefile Options
# ----------------------------

NAME = BPLOT
ICON = icon.png
DESCRIPTION = "Better Plotter"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
