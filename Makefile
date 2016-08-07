NAME = pgf

CSRC = main.cpp logging.cpp bitarr.cpp genimage.cpp yuvimage.cpp image.cpp hcoder.cpp pdistrib.cpp pgfcoder.cpp plinear.cpp pmed.cpp predictor.cpp psim.cpp arcoder.cpp pequal.cpp
HDRS = logging.h bitarr.h genimage.h yuvimage.h image.h hcoder.h pdistrib.h pgfcoder.h plinear.h pmed.h polygon.h predictor.h psim.h arcoder.h pequal.h
OBJDIR = obj
_OBJS = $(patsubst %.cpp,%.o,$(CSRC))
OBJS =	$(patsubst %, $(OBJDIR)/%, $(_OBJS))


DEBUG =
DEFS = DEBUG

CCFLAGS = -O3 -std=c++11 -Wno-format-security -g3 $(addprefix -D,$(DEFS)) 
LINKFLAGS = -lm  #-lquadmath

all:		$(NAME)

clean:
		rm -f $(OBJS) $(NAME)

$(NAME):	$(OBJS) $(HDRS)
		clang++  -o $@ $(OBJS) $(LINKFLAGS)

$(OBJDIR)/%.o:	%.cpp $(HDRS)
		clang++ $(CCFLAGS) -o $@ -c $<

