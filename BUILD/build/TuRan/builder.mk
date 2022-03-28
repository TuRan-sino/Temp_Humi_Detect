CFLAGS := -std=c11 -c -xc -mthumb -mcpu=cortex-m3 -Og -Wall -g -ffunction-sections -fdata-sections -I./../CORE -I./../USER -I./../FWLIB/inc -I./../SYSTEM -I./.eide/deps -I./../MODULE -I./../HARDWARE -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER
CXXFLAGS := -std=c++11 -c -xc++ -mthumb -mcpu=cortex-m3 -Og -Wall -g -ffunction-sections -fdata-sections -I./../CORE -I./../USER -I./../FWLIB/inc -I./../SYSTEM -I./.eide/deps -I./../MODULE -I./../HARDWARE -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER
ASMFLAGS := -c -x assembler-with-cpp -mthumb -mcpu=cortex-m3 -g -I./../CORE -I./../USER -I./../FWLIB/inc -I./../SYSTEM -I./.eide/deps -I./../MODULE -I./../HARDWARE
LDFLAGS := -mthumb -mcpu=cortex-m3 -T "c:/Users/TuRan/WORK/Graduation/CORE/stm32_flash.ld" --specs=nosys.specs --specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage -Wl,-Map=./build/TuRan/KEIL.map
LDLIBS := -lm
