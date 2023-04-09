import subprocess


def LViewPlus64_load_cfg(cfg):
    pass


def LViewPlus64_save_cfg(cfg):
    pass


def LViewPlus64_draw_settings(objs, ui):
    pass


def LViewPlus64_update(game, ui):
    pass


def format():
    print("🎀 Formatting files")
    subprocess.call(["python", "-m" "black", "."])


if __name__ == "__main__":
    format()
