#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifndef DS
ifndef GBA
$(error "you must pass either GBA=1 or DS=1.")
endif
endif

ifdef DS
include $(DEVKITARM)/ds_rules
endif

ifdef GBA
include $(DEVKITARM)/gba_rules
endif

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
ifdef GBA
ext := "_GBA"
else
ext := "_NDS"
endif

TARGET		:=	$(shell basename $(CURDIR))$(ext)
GRAPHICS	:=	example_code/images
ifdef GBA
BUILD		:=	GBAbuild
else
BUILD       :=  DSbuild
endif
SOURCES		:=	gfx source data example_code example_code/images
INCLUDES	:=	include build NDSA
TILEMAPS    :=  example_code/images

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

ifdef GBA
CFLAGS += -mcpu=arm7tdmi -mtune=arm7tdmi -DGBA
else
CFLAGS += -march=armv5te -mtune=arm946e-s  -DDS
endif
CFLAGS	+=	-g -Wall -O2 \
 			-fomit-frame-pointer\
			-ffast-math -fexceptions -Wno-unused-variable \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:= $(CFLAGS) -std=c++17 -Wno-reorder

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)
ifdef DS
LDFLAGS += -specs=ds_arm9.specs
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
ifdef DS
LIBS	:= -lnds9
else
LIBS    := -lgba
endif
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS) $(LIBGBA)
 
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))
PNGFILES 	:=  $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
TMXFILES    :=  $(foreach dir,$(TILEMAPS),$(notdir $(wildcard $(dir)/*.tmx)))
CSVFILES    :=  $(subst .tmx,.csv,$(TMXFILES))
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(BINFILES:.bin=.o) $(CSVFILES:.csv=.o) $(PNGFILES:.png=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
 
.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
 
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba 
 
 
#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
ifdef DS
$(OUTPUT).nds	: 	$(OUTPUT).elf
endif
ifdef GBA
$(OUTPUT).gba   :   $(OUTPUT).elf
endif
$(OUTPUT).elf	:	$(OFILES)
 
#---------------------------------------------------------------------------------
%.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

%.c %.h: %.csv
	@echo $(notdir $(subst .csv,.c,$<))
	@../tools/mapToC.py $< $(notdir $(subst .csv,.c,$<) $(subst .csv,.h,$<))

%.csv: %.tmx
	@echo $(notdir $(subst .tmx,.csv,$<))
	@tiled --export-map csv $< $(notdir $(subst .tmx,.csv,$<))


 #---------------------------------------------------------------------------------
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders
#---------------------------------------------------------------------------------
%.s %.h: %.png %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*

-include $(DEPENDS)
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
