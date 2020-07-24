
SHELL		= /bin/sh
CXX		= c++
CXXFLAGS	= -I.. -O4
DSOFLAGS	= -I.. -O4
DSOLIBS		=  -lm
INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
AR		= ar rv
RANLIB          = ranlib
libdir          = /cdat/daf9/mccann/jimpaw/roottrials/Rio/lib
incdir          = /cdat/daf9/mccann/jimpaw/roottrials/Rio/include
tmpdir          = /cdat/daf9/mccann/jimpaw/roottrials/Rio/tmp
PREFIX          = /cdat/daf9/mccann/jimpaw/roottrials/Rio
TARGETDIR	= ../Linux-c++
SRCDIR		= ../source
TARGETLIBS	= ../Linux-c++/libRio.so
TARGETSTATICLIBS = ../Linux-c++/libRio.a

LDFLAGS         = -L$(TARGETDIR) -lRio -lm
LD_SHARED	= c++ -shared
CXXLIBS		= 
.SUFFIXES: .exe .so .cxx .c

all:library staticlibs tests

.c.o:
	$(CXX) -c $(CXXFLAGS) $(@:.o=.c) -o $@

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(@:.o=.cxx) -o $@

#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
ZIPOBJS	= Trees.o Deflate.o Inflate.o Bits.o
FULLZIPOBJS= $(patsubst %.o, $(TARGETDIR)/%.o, $(ZIPOBJS))
$(FULLZIPOBJS): $(TARGETDIR)/%.o : $(SRCDIR)/zip/%.c
	@echo "Compiling file" $< "...";
	$(CXX) -c $(CXXFLAGS) $(ALL_INCLUDE) $< -o $@;
	@echo "done"

COREOBJS = smanip.o Debug.o Lib.o Printer.o Out.o Date.o Arguments.o Map.o Object.o Named.o List.o ObjArray.o FreeSegment.o Class.o StreamerElement.o StreamerInfo.o CoreDictionary.o Buffer.o Key.o Directory.o File.o
FULLCOREOBJS= $(patsubst %.o, $(TARGETDIR)/%.o, $(COREOBJS))
$(FULLCOREOBJS): $(TARGETDIR)/%.o : $(SRCDIR)/Core/%.cxx
	@echo "Compiling file" $< "...";
	$(CXX) -c $(CXXFLAGS) $(ALL_INCLUDE) $< -o $@;
	@echo "done"

TREEOBJS = BaseLeaf.o Leaf.o LeafObject.o Basket.o Branch.o BranchObject.o Tree.o TreeDictionary.o
FULLTREEOBJS= $(patsubst %.o, $(TARGETDIR)/%.o, $(TREEOBJS))
$(FULLTREEOBJS): $(TARGETDIR)/%.o : $(SRCDIR)/Tree/%.cxx
	@echo "Compiling file" $< "...";
	$(CXX) -c $(CXXFLAGS) $(ALL_INCLUDE) $< -o $@;
	@echo "done"

FULLRIOOBJS = $(FULLZIPOBJS) $(FULLCOREOBJS) $(FULLTREEOBJS)

#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
TESTS = Rio_test.exe
FULLTESTS= $(patsubst %.exe, $(TARGETDIR)/%.exe, $(TESTS))
$(FULLTESTS): $(TARGETDIR)/%.exe : ../tests/%.cxx
	@echo "Compiling file" $< "...";
	$(CXX) $(CXXFLAGS) $(ALL_INCLUDE) $< $(LDFLAGS) -o $@;
	@echo "done"

tests: $(FULLTESTS)
#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
library: $(TARGETLIBS)

staticlibs: $(TARGETSTATICLIBS)

install:
	@if test -d $(libdir); then echo ' ' ; else mkdir -p $(libdir) ; fi
	@if test -d $(incdir); then echo ' ' ; else mkdir -p $(incdir) ; fi
	@if test -d $(incdir)/Rio; then echo ' ' ; else mkdir -p $(incdir)/Rio ; fi
	@if test -d $(incdir)/Rio/zip; then echo ' ' ; else mkdir -p $(incdir)/Rio/zip ; fi
	@if test -d $(incdir)/Rio/Interfaces; then echo ' ' ; else mkdir -p $(incdir)/Rio/Interfaces/zip ; fi
	@if test -d $(incdir)/Rio/Core; then echo ' ' ; else mkdir -p $(incdir)/Rio/Core ; fi
	@if test -d $(incdir)/Rio/Tree; then echo ' ' ; else mkdir -p $(incdir)/Rio/Tree ; fi
	cp $(TARGETDIR)/lib* $(libdir)
	cp ../Rio/zip/*.h $(incdir)/Rio/zip
	cp ../Rio/Interfaces/*.h $(incdir)/Rio/Interfaces
	cp ../Rio/Core/*.h $(incdir)/Rio/Core
	cp ../Rio/Tree/*.h $(incdir)/Rio/Tree

$(TARGETDIR)/libRio.so: $(FULLRIOOBJS)
	$(LD_SHARED) $(DSOFLAGS) -o $(TARGETDIR)/libRio.so $(FULLRIOOBJS) ${DSOLIBS}

$(TARGETDIR)/libRio.a: $(FULLRIOOBJS)
	$(AR) $(TARGETDIR)/libRio.a $(FULLRIOOBJS)
	$(RANLIB) $(TARGETDIR)/libRio.a

#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////////////
clean:
	/bin/rm  -rf $(TARGETDIR) *~ so_locations $(TARGETLIBS)\
	  $(TARGETSTATICLIBS) config.cache ok.cfg \
	  config.log config.status ti_files

trim:
	/bin/rm  -rf $(TARGETDIR)/*.o so_locations \
	  config.cache ok.cfg config.log config.status ti_files
