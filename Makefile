ccsrc = $(wildcard src/*.cc)
csrc = $(wildcard src/*.c)					  
ccobj = $(ccsrc:.cc=.o)
cobj = $(csrc:.c=.o)
obj = $(cobj) $(ccobj)
lib_a = libutk.a
dist_file = ubertk.tar.gz
diag_file = diag.ps

CC = gcc
CXX = g++
AR = ar
CCFLAGS = -std=c89 -pedantic -Wall -g -Isrc
CXXFLAGS = -ansi -pedantic -Wall -g -Isrc `pkg-config --cflags freetype2`

.PHONY: all
all: $(lib_a) test

$(lib_a): $(obj)
	$(AR) rcs $@ $(obj)

test: test.o test_text.o $(lib_a)
	$(CXX) -o $@ test.o test_text.o $(lib_a) -lglut -lGL -lGLU `pkg-config --libs freetype2`

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
