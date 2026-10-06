# TOP = ..
# 
# export TOP
# 
# include $(TOP)/configure/RELEASE
# include $(TOP)/configure/CONFIG_SITE
# export EPICS_BASE

include ../CONFIG

html: $(wildcard *.rst) $(wildcard *.py)
	$(SPHINXBUILD) -nE -b html . html
# 	$(SPHINXBUILD) -nE -b html . html

install: html

clean:
	rm -rf html

.PHONY: clean install
