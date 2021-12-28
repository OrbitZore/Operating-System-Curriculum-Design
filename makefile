all: build

build/all.md : doc/d*.md
	awk 1 header/header.md doc/d*.md > build/all.md

build : build/all.md
	$(MAKE) -C build all

clean:
	rm -f build/all.md
	$(MAKE) -C build clean