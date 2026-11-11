import os
import sys
import shutil
import subprocess


def flash_firmware():
    # Set default target binary path or use CLI argument
    bin_path = (
        sys.argv[1] if len(sys.argv) > 1 else os.path.join("build", "firmware.bin")
    )

    # Verify binary existence
    if not os.path.exists(bin_path):
        print(f"[ERROR] Binary file not found: {bin_path}")
        print("[INFO] Run 'scons' first to compile the project.")
        sys.exit(1)

    # Search for flash tools
    openocd_path = shutil.which("openocd")
    clt_programmer = (
        r"C:\ST\STM32CubeCLT_1.21.0\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
    )
    programmer_cli_path = shutil.which("STM32_Programmer_CLI") or (
        clt_programmer if os.path.exists(clt_programmer) else None
    )

    if programmer_cli_path:
        print(f"[INFO] Flashing {bin_path} via STM32_Programmer_CLI...")
        cmd = [
            programmer_cli_path,
            "-c",
            "port=SWD",
            "mode=UR",
            "-w",
            bin_path,
            "0x08000000",
            "-v",
            "-rst",
        ]
    elif openocd_path:
        print(f"[INFO] Flashing {bin_path} via OpenOCD...")
        cmd = [
            openocd_path,
            "-f",
            "interface/stlink.cfg",
            "-f",
            "target/stm32c0x.cfg",
            "-c",
            f"program {bin_path} verify reset exit",
        ]
    else:
        print("[ERROR] Neither 'STM32_Programmer_CLI' nor 'openocd' were found.")
        print("[INFO] Please verify your STM32CubeCLT installation path.")
        sys.exit(1)

    try:
        result = subprocess.run(cmd, check=True)
        if result.returncode == 0:
            print("[SUCCESS] Firmware successfully flashed to board!")
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Flashing failed with error code: {e.returncode}")
        sys.exit(e.returncode)


if __name__ == "__main__":
    flash_firmware()
