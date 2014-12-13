.PHONY: all wc

all:
	for i in `find . -mindepth 1 -maxdepth 1 -type d | grep -v ".git" | grep -v 3party` ; do (cd $$i; make) ; done

wc:
	(find . -iname "*.cc" ; find . -iname "*.h") | grep -v "/build/" | grep -v 3party | xargs wc -l
