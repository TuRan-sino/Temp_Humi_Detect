#############################################
#        Embedded IDE Makefile 模板
#############################################

# 输出根目录
BUILD_ROOT = build

# 包含 eide 项目信息（强制包含，没有该文件将失败）
include $(BUILD_ROOT)/target.mk

# 包含 eide unify_builder 生成的编译器参数
# 如果 builder.mk 文件不存在，注释以下行，并手动
# 在后面的 “附加编译器参数” 中加入编译参数
# include $(BUILD_ROOT)/$(CUR_TARGET)/builder.mk
SRC_INC = $(foreach path,$(INCLUDE_FOLDERS),$(INC_PREFIX)$(path))
LIB_INC = $(foreach path,$(LIB_FOLDERS),$(LIB_PREFIX)$(path))
DEFS = $(foreach str,$(DEFINES),$(DEF_PREFIX)$(str))
CFLAGS += $(SRC_INC) $(DEFS)
CXXFLAGS += $(SRC_INC) $(DEFS)
LDFLAGS += $(LIB_INC)

########################################
#        编译器全局配置，必填
########################################

# 编译器可执行文件目录, 如果路径不为空，则必须以 '/' 结尾
# 例如：CC_FOLDER = D:/xpack-riscv-none-embed-gcc-8.3.0-2.3/bin/
CC_FOLDER = 
# 可执行文件前缀
CC_PREFIX = 

################################
#        应用程序生成配置
################################

# 可执行文件名称
EXE_NAME := app

# 输出目录
BUILD_DIR := $(BUILD_ROOT)/$(CUR_TARGET)

# 输出二进制类型，默认：elf，可选值：static_lib, elf
OUTPUT_TYPE := elf

################################
#     要生成的可执行文件列表
################################

ifeq ($(OUTPUT_TYPE),static_lib)
ifeq ($(AR_SUFFIX),a)
EXE_NAME := lib$(EXE_NAME)
endif
EXE_FILES += $(BUILD_DIR)/$(EXE_NAME).$(AR_SUFFIX)
else
EXE_FILES += $(BUILD_DIR)/$(EXE_NAME).$(ELF_SUFFIX)
EXE_FILES += $(BUILD_DIR)/$(EXE_NAME).hex
EXE_FILES += $(BUILD_DIR)/$(EXE_NAME).bin
endif

###############################
#        附加编译器参数
###############################

# CFLAGS
CFLAGS +=

# CXXFLAGS
CXXFLAGS +=

# ASMFLAGS
ASMFLAGS +=

# LDFLAGS
LDFLAGS +=

# LDLIBS
LDLIBS +=

################################
#        SOURCE FILES
################################

C_FILTER := %.c
CPP_FILTER := %.cpp %c++ %cxx %cc
ASM_FILTER := %.asm %.s %.a51
OBJ_FILTER := %.o %.lib %.a %.obj

# C sources
C_SRC = $(foreach path,$(filter $(C_FILTER),$(C_SOURCES)),$(path))

# Cpp sources
CPP_SRC = $(foreach path,$(filter $(CPP_FILTER),$(CPP_SOURCES)),$(path))

# ASM sources
ASM_SRC = $(foreach path,$(filter $(ASM_FILTER),$(ASM_SOURCES)),$(path))

# Obj sources
OBJ_SRC = $(foreach path,$(filter $(OBJ_FILTER),$(OBJ_SOURCES)),$(path))

########################################
#        COMPILER CONFIG
########################################

ifeq ($(COMPILER_TYPE),IAR_STM8)

# 编译器可执行文件
CC = $(CC_FOLDER)$(CC_PREFIX)iccstm8
AS = $(CC_FOLDER)$(CC_PREFIX)iasmstm8
LD = $(CC_FOLDER)$(CC_PREFIX)ilinkstm8
AR = $(CC_FOLDER)$(CC_PREFIX)iarchive

# binutils 可执行文件
#SZ = 
HEX = $(CC_FOLDER)$(CC_PREFIX)ielftool
BIN = $(CC_FOLDER)$(CC_PREFIX)ielftool

# 生成 hex, bin 的命令
HEX_FLAGS = --silent --ihex
HEX_OUT_CMD =
BIN_FLAGS = --silent --bin
BIN_OUT_CMD =

# static lib flags
AR_FLAGS = --create

# 包含命令，宏定义命令的前缀
INC_PREFIX = -I #
LIB_PREFIX = -I #
DEF_PREFIX = -D #

# 编译器输出命令
CC_OUT_CMD = -o
AS_OUT_CMD = -o
LD_OUT_CMD = -o
AR_OUT_CMD = -o

# 二进制文件后缀
OBJ_SUFFIX = o
ELF_SUFFIX = out
AR_SUFFIX = lib

endif

ifeq ($(COMPILER_TYPE),SDCC)

# 编译器可执行文件
CC = $(CC_FOLDER)$(CC_PREFIX)sdcc
AS = $(CC_FOLDER)$(CC_PREFIX)sdcc
LD = $(CC_FOLDER)$(CC_PREFIX)sdcc
AR = $(CC_FOLDER)$(CC_PREFIX)sdar

# binutils 可执行文件
#SZ = 
#HEX = 
#BIN = 

# 生成 hex, bin 的命令
#HEX_FLAGS = -O ihex
#HEX_OUT_CMD =
#BIN_FLAGS = -O binary -S
#BIN_OUT_CMD =

# static lib flags
AR_FLAGS = 

# 包含命令，宏定义命令的前缀
INC_PREFIX = -I
LIB_PREFIX = -L
DEF_PREFIX = -D

# 编译器输出命令
CC_OUT_CMD = -o
AS_OUT_CMD = -o
LD_OUT_CMD = -o
AR_OUT_CMD = -rcv

# 二进制文件后缀
OBJ_SUFFIX = rel
ELF_SUFFIX = ihx
AR_SUFFIX = lib

endif

# for AC5/AC6 compiler
ifeq ($(findstring AC,$(COMPILER_TYPE)),AC)

# 编译器可执行文件
ifeq ($(COMPILER_TYPE),AC5)
CC = $(CC_FOLDER)$(CC_PREFIX)armcc
else
CC = $(CC_FOLDER)$(CC_PREFIX)armclang
endif

AS = $(CC_FOLDER)$(CC_PREFIX)armasm
LD = $(CC_FOLDER)$(CC_PREFIX)armlink
AR = $(CC_FOLDER)$(CC_PREFIX)armar

# binutils 可执行文件
#SZ = 
HEX = $(CC_FOLDER)fromelf
BIN = $(HEX)

# 生成 hex, bin 的命令
HEX_FLAGS = --i32combined 
HEX_OUT_CMD = --output
BIN_FLAGS = --bincombined 
BIN_OUT_CMD = --output

# static lib flags
AR_FLAGS = 

# 包含命令，宏定义命令的前缀
INC_PREFIX = -I
LIB_PREFIX = -L
DEF_PREFIX = -D

# 编译器输出命令
CC_OUT_CMD = -o
AS_OUT_CMD = -o
LD_OUT_CMD = -o
AR_OUT_CMD = -rcv

# 二进制文件后缀
OBJ_SUFFIX = o
ELF_SUFFIX = axf
AR_SUFFIX = lib

endif

# for gcc famliy compiler
ifeq ($(findstring GCC,$(COMPILER_TYPE)),GCC)

# 编译器可执行文件
CC = $(CC_FOLDER)$(CC_PREFIX)gcc
AS = $(CC_FOLDER)$(CC_PREFIX)gcc
LD = $(CC_FOLDER)$(CC_PREFIX)gcc
AR = $(CC_FOLDER)$(CC_PREFIX)ar

# binutils 可执行文件
SZ = $(CC_FOLDER)$(CC_PREFIX)size
HEX = $(CC_FOLDER)$(CC_PREFIX)objcopy
BIN = $(CC_FOLDER)$(CC_PREFIX)objcopy

# 生成 hex, bin 的命令
HEX_FLAGS = -O ihex
HEX_OUT_CMD =
BIN_FLAGS = -O binary -S
BIN_OUT_CMD =

# static lib flags
AR_FLAGS = 

# 包含命令，宏定义命令的前缀
INC_PREFIX = -I
LIB_PREFIX = -L
DEF_PREFIX = -D

# 编译器输出命令
CC_OUT_CMD = -o
AS_OUT_CMD = -o
LD_OUT_CMD = -o
AR_OUT_CMD = -rcv

# 二进制文件后缀
OBJ_SUFFIX = o
ELF_SUFFIX = elf
AR_SUFFIX = a

endif

# not support Keil_C51
ifeq ($(findstring Keil_C51,$(COMPILER_TYPE)),Keil_C51)
$(error !!! We not support for '$(COMPILER_TYPE)' compiler !!!)
endif

############################################################################
# !!! START BUILD THE APPLICATION, DON'T MODIFY THE FOLLOWING CONTENTS !!!
############################################################################

all: $(EXE_FILES)

# print color
COLOR_END = "\e[0m"
COLOR_WARN = "\e[33;1m"
COLOR_DONE = "\e[32;1m"
COLOR_ERR = "\e[31;1m"

# merge all objs
OBJS += $(addprefix $(BUILD_DIR)/,$(notdir $(addsuffix .$(OBJ_SUFFIX),$(basename $(C_SRC))))) \
		$(addprefix $(BUILD_DIR)/,$(notdir $(addsuffix .$(OBJ_SUFFIX),$(basename $(CPP_SRC))))) \
		$(addprefix $(BUILD_DIR)/,$(notdir $(addsuffix .$(OBJ_SUFFIX),$(basename $(ASM_SRC))))) \
		$(OBJ_SRC)

# add source search folder
vpath %.c $(sort $(dir $(C_SRC)))
vpath %.cpp $(sort $(dir $(CPP_SRC)))
vpath %.cc $(sort $(dir $(CPP_SRC)))
vpath %.cxx $(sort $(dir $(CPP_SRC)))
vpath %.c++ $(sort $(dir $(CPP_SRC)))
vpath %.s $(sort $(dir $(ASM_SRC)))
vpath %.asm $(sort $(dir $(ASM_SRC)))
vpath %.a51 $(sort $(dir $(ASM_SRC)))

# compile C source file
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.c Makefile | $(BUILD_DIR) 
	@echo CC '$<' ...
	@$(CC) $(CFLAGS) $< $(CC_OUT_CMD) $@

# compile CPP source file
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.cpp Makefile | $(BUILD_DIR) 
	@echo CC '$<' ...
	@$(CC) $(CXXFLAGS) $< $(CC_OUT_CMD) $@
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.cc Makefile | $(BUILD_DIR) 
	@echo CC '$<' ...
	@$(CC) $(CXXFLAGS) $< $(CC_OUT_CMD) $@
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.cxx Makefile | $(BUILD_DIR) 
	@echo CC '$<' ...
	@$(CC) $(CXXFLAGS) $< $(CC_OUT_CMD) $@
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.c++ Makefile | $(BUILD_DIR) 
	@echo CC '$<' ...
	@$(CC) $(CXXFLAGS) $< $(CC_OUT_CMD) $@

# compile ASM program objects
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.s Makefile | $(BUILD_DIR)
	@echo AS '$<' ...
	@$(AS) $(ASMFLAGS) $< $(AS_OUT_CMD) $@
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.asm Makefile | $(BUILD_DIR)
	@echo AS '$<' ...
	@$(AS) $(ASMFLAGS) $< $(AS_OUT_CMD) $@
$(BUILD_DIR)/%.$(OBJ_SUFFIX): %.a51 Makefile | $(BUILD_DIR)
	@echo AS '$<' ...
	@$(AS) $(ASMFLAGS) $< $(AS_OUT_CMD) $@

# link executable file
$(BUILD_DIR)/$(EXE_NAME).$(ELF_SUFFIX): $(OBJS) Makefile | $(BUILD_DIR)
	@echo LINK '$@' ...
	@$(LD) $(OBJS) $(LDFLAGS) $(LD_OUT_CMD) $@ $(LDLIBS)
ifdef SZ
	@$(SZ) $@
endif
	@echo -e $(COLOR_DONE)"#################### Build successfully ! ####################"$(COLOR_END)

# static lib
$(BUILD_DIR)/$(EXE_NAME).$(AR_SUFFIX): $(OBJS) Makefile | $(BUILD_DIR)
	@echo AR '$@' ...
	@$(AR) $(AR_OUT_CMD) $@ $(AR_FLAGS) $(OBJS) 
	@echo -e $(COLOR_DONE)"#################### Archive lib successfully ! ####################"$(COLOR_END)

# generate hex
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.$(ELF_SUFFIX) | $(BUILD_DIR)
ifdef HEX
	@$(HEX) $(HEX_FLAGS) $< $(HEX_OUT_CMD) $@
else
	@echo -e $(COLOR_WARN)"Not found hex command. Skip output hex file !"$(COLOR_END)
endif

# generate bin
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.$(ELF_SUFFIX) | $(BUILD_DIR)
ifdef BIN
	@$(BIN) $(BIN_FLAGS) $< $(BIN_OUT_CMD) $@
else
	@echo -e $(COLOR_WARN)"Not found bin command. Skip output bin file !"$(COLOR_END)
endif
	
$(BUILD_DIR):
	mkdir $@		

#####################
# CLEAN ALL OBJECTS
#####################
.PHONY : clean
clean:
	-rm -fR $(BUILD_DIR)/*.$(OBJ_SUFFIX)
	-rm -fR $(BUILD_DIR)/*.$(ELF_SUFFIX)
	-rm -fR $(BUILD_DIR)/*.$(AR_SUFFIX) 
	-rm -fR $(BUILD_DIR)/*.hex $(BUILD_DIR)/*.bin
