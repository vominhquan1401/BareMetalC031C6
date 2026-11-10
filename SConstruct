import os
from SCons.Script import *

# ==========================================
# CẤU HÌNH BIÊN DỊCH VÀ TOOLCHAIN
# ==========================================
env = Environment(
    ENV=os.environ,
    CC="arm-none-eabi-gcc",
    AS="arm-none-eabi-gcc",
    LINK="arm-none-eabi-gcc",
    OBJCOPY="arm-none-eabi-objcopy",
)

# Cờ biên dịch cho STM32C031 (Cortex-M0+)
cflags = [
    "-mcpu=cortex-m0plus",
    "-mthumb",
    "-O2",
    "-Wall",
    "-Wextra",
    "-ffunction-sections",
    "-fdata-sections",
    "-g",
    "-DSTM32C031xx",  # Macro bắt buộc để kích hoạt đúng thanh ghi trong stm32c0xx.h
]

# Cờ liên kết trỏ trực tiếp vào file Linker trong thư mục board/linker
linkflags = [
    "-mcpu=cortex-m0plus",
    "-mthumb",
    "-T",
    "board/linker/stm32c031c6_flash.ld",
    "-Wl,--gc-sections",
    "-Wl,-Map=build/firmware.map",
    "-Wl,--no-warnings",  # Tắt toàn bộ cảnh báo từ trình liên kết Linker
    "--specs=nano.specs",
    "--specs=nosys.specs",
]

env.Append(CCFLAGS=cflags)
env.Append(
    ASFLAGS=["-c"] + cflags
)  # Ép cờ -c để biên dịch file .s thành .o (tránh lỗi Linker)
env.Append(LINKFLAGS=linkflags)

# Khai báo các đường dẫn chứa file Header (.h)
env.Append(CPPPATH=["include", "board/cmsis/core", "board/cmsis/device"])

# ==========================================
# GOM MÃ NGUỒN TỰ ĐỘNG
# ==========================================
# Lấy file code ứng dụng (.c) từ src/ và file khởi tạo (.c, .s) từ board/startup/
user_sources = Glob("src/*.c")
board_sources = Glob("board/startup/*.c") + Glob("board/startup/*.s")
all_sources = user_sources + board_sources

# ==========================================
# QUY TRÌNH BIÊN DỊCH (ELF -> BIN)
# ==========================================
elf_file = env.Program(target="build/firmware.elf", source=all_sources)

Clean(elf_file, "build/firmware.map")

bin_builder = Builder(
    action="$OBJCOPY -O binary $SOURCE $TARGET", suffix=".bin", src_suffix=".elf"
)
env.Append(BUILDERS={"MakeBin": bin_builder})

bin_file = env.MakeBin(target="build/firmware.bin", source=elf_file)

Default(bin_file)
