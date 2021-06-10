import subprocess

def winstealer_load_cfg(cfg):
    pass


def winstealer_save_cfg(cfg):
    pass


def winstealer_draw_settings(objs, ui):
    pass


def winstealer_update(game, ui):
    pass

def format():
    print("🎀 Formatting files")
    subprocess.call(["python", "-m" "black", "."])


if __name__ == "__main__":
    format()
