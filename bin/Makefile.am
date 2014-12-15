include copperspice.am

# path to gcc (windows)
gccpath=/c/MingW/bin

# list of gcc libraries (windows)
gcclibs=libgcc_s_sjlj-1  libstdc++-6  libwinpthread-1

# list of CopperSpice libraries
cslibs=Core Xml

# location of includes
AM_CXXFLAGS+=-I../src

if OSTYPE_LINUX
endif

if OSTYPE_DARWIN
endif

if OSTYPE_WIN
   # disable console on window
   AM_CXXFLAGS+=-mwindows

   # location for 'make install'
   installdir=$(abs_top_builddir)/install
endif

# target app name
bin_PROGRAMS = Doxygen

# copy required CopperSpice and gcc files for deployment
.PHONY: deploy

if OSTYPE_LINUX
endif

if OSTYPE_DARWIN
endif

if OSTYPE_WIN
deploy : all
	mkdir -p $(deploydir)

	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)

#	rsync -W -a --modify-window=2  $(top_srcdir)/resources/diamond.ico $(deploydir)

	rsync -W -a --modify-window=2  $(patsubst %,$(gccpath)/%.dll,$(gcclibs))  $(deploydir)
	rsync -W -a --modify-window=2  $(patsubst %,$(CS_HOME)/bin/libCs%1-0.dll,$(cslibs))  $(deploydir)
endif

# search path for source
VPATH = ../

# linking
Doxygen_LDFLAGS=$(AM_LDFLAGS) $(patsubst %,-lCs%1,$(cslibs))


Doxygen_SOURCES = \
   src/arguments.cpp \
   src/cite.cpp \
   src/clangparser.cpp \
   src/classdef.cpp \
   src/classlist.cpp \
   src/cmdmapper.cpp \
   src/condparser.cpp \
   src/context.cpp \
   src/cppvalue.cpp \
   src/dbusxmlscanner.cpp \
   src/debug.cpp \
   src/defgen.cpp \
   src/define.cpp \
   src/definition.cpp \
   src/dia.cpp \
   src/diagram.cpp \
   src/dirdef.cpp \
   src/docbookgen.cpp \
   src/docbookvisitor.cpp \
   src/docparser.cpp \
   src/docsets.cpp \
   src/dot.cpp \
   src/doxygen.cpp \
   src/eclipsehelp.cpp \
   src/entry.cpp \
   src/filedef.cpp \
   src/filename.cpp \
   src/fileparser.cpp \
   src/formula.cpp \
   src/ftextstream.cpp \
   src/ftvhelp.cpp \
   src/groupdef.cpp \
   src/htags.cpp \
   src/htmldocvisitor.cpp \
   src/htmlentity.cpp \
   src/htmlgen.cpp \
   src/htmlhelp.cpp \
   src/image.cpp \
   src/index.cpp \
   src/language.cpp \
   src/latexdocvisitor.cpp \
   src/latexgen.cpp \
   src/layout.cpp \
   src/lodepng.cpp \
   src/logos.cpp \
   src/main.cpp \
   src/mandocvisitor.cpp \
   src/mangen.cpp \
   src/markdown.cpp \
   src/marshal.cpp \
   src/memberdef.cpp \
   src/membergroup.cpp \
   src/memberlist.cpp \
   src/membername.cpp \
   src/message.cpp \
   src/msc.cpp \
   src/namespacedef.cpp \
   src/objcache.cpp \
   src/outputgen.cpp \
   src/outputlist.cpp \
   src/pagedef.cpp \
   src/perlmodgen.cpp \
   src/plantuml.cpp \
   src/portable.cpp \
   src/qhp.cpp \
   src/qhpxmlwriter.cpp \
   src/reflist.cpp \
   src/resourcemgr.cpp \
   src/rtfdocvisitor.cpp \
   src/rtfgen.cpp \
   src/rtfstyle.cpp \
   src/sortedlist.cpp \
   src/searchindex.cpp \
   src/sqlite3gen.cpp \
   src/store.cpp \
   src/tagreader.cpp \
   src/template.cpp \
   src/textdocvisitor.cpp \
   src/tooltip.cpp \
   src/util.cpp \
   src/xmldocvisitor.cpp \
   src/xmlgen.cpp

RESOURCES = doxygen.qrc

# use wilcard to obtain just the dependency list
qrc_doxygen.cpp: doxygen.qrc   $(wildcard ../resources/*)

# required to compile qrc
nodist_Doxygen_SOURCES=qrc_doxygen.cpp