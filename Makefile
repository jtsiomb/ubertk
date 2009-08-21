PREFIX = /usr/local

ccsrc = $(wildcard src/*.cc)
csrc = $(wildcard src/*.c)
ccobj = $(ccsrc:.cc=.o)
cobj = $(csrc:.c=.o)
obj = $(cobj) $(ccobj)
lib_a = libutk.a
soname = libspnav.so.0
lib_so = $(soname).1
dist_file = ubertk.tar.gz
diag_file = diag.ps

opt = -O3
dbg = -g

ifneq ($(shell uname -s), Darwin)
	pic = -fPIC
	gllibs = -lGL -lGLU -lglut
else
	gllibs = -framework OpenGL -framework GLUT
endif

CC = gcc
CXX = g++
AR = ar
INSTALL = install
CFLAGS = -std=c89 -pedantic -Wall $(opt) $(dbg) -Isrc $(pic)
CXXFLAGS = -ansi -pedantic -Wall $(opt) $(dbg) -Isrc $(pic) `pkg-config --cflags freetype2`
LDFLAGS = $(gllibs) `pkg-config --libs freetype2` -lpcre

.PHONY: all
all: $(lib_a) $(lib_so) test

$(lib_a): $(obj)
	$(AR) rcs $@ $(obj)

$(lib_so): $(obj)
	$(CXX) -shared -Wl,-soname,$(soname) -o $@ $(obj)


test: test.o test_text.o $(lib_a)
	$(CXX) -o $@ test.o test_text.o $(lib_a) $(LDFLAGS)

.PHONY: $(diag_file)
$(diag_file):
	echo 'digraph class_diagram {' >/tmp/cdiag.dot
	echo '    rankdir=BT;' >>/tmp/cdiag.dot
	for i in src/*.h; do grep class $$i | grep ':' | sed 's/class /    /' | sed 's/: public/->/' | sed 's/ {/;/' >>/tmp/cdiag.dot; done
	echo '}' >>/tmp/cdiag.dot
	dot -Tps /tmp/cdiag.dot >$(diag_file)


.PHONY: clean
clean:
	rm -f $(obj) $(lib_a) $(dist_file) $(diag_file) test.o test_text.o test

.PHONY: dist
dist: clean
	cd ..; tar czvf $(dist_file) ubertk; mv $(dist_file) ubertk/

.PHONY: install
install:
	$(INSTALL) -d $(PREFIX)/lib
	$(INSTALL) -m 644 $(lib_a) $(PREFIX)/lib/$(lib_a)
	$(INSTALL) -m 644 $(lib_so) $(PREFIX)/lib/$(lib_so)
	$(INSTALL) -d $(PREFIX)/include/utk
	cd src; $(INSTALL) -m 644 *.h *.inl $(PREFIX)/include/utk

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/lib/$(lib_a)
	rm -f $(PREFIX)/lib/$(lib_so)
	rm -f $(PREFIX)/include/utk/*
	rmdir $(PREFIX)/include/utk
