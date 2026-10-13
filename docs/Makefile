# TOP = ..
# 
# export TOP

include ../CONFIG

html: $(wildcard *.rst) $(wildcard *.py)
	$(SPHINXBUILD) -nE -b html . html
# 	$(SPHINXBUILD) -nE -b html . html

install: html

clean:
	rm -rf html

.PHONY: clean install
