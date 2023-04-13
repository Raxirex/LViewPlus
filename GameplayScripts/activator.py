from LViewPlus64 import *
from commons.targeting import *
from time import time

show_healable = False
auto_qss = True
heal_percent = 0
auto_heal = True

LViewPlus64_script_info = {
    "script": "Activator",
    "author": "bckd00r",
    "description": "Activator",
}


def LViewPlus64_load_cfg(cfg):
    global auto_heal, heal_percent
    global auto_qss
    auto_qss = cfg.get_bool("Auto QSS", True)
    heal_percent = cfg.get_float("heal_percent", 20)
    auto_heal = cfg.get_bool("auto_heal", True)


def LViewPlus64_save_cfg(cfg):
    global auto_heal, heal_percent
    cfg.set_float("heal_percent", heal_percent)
    cfg.set_bool("auto_heal", auto_heal)


def LViewPlus64_draw_settings(game, ui):
    global auto_heal, heal_percent

    ui.begin("WS+ Activator")
    if ui.treenode("Heal"):
        auto_heal = ui.checkbox("Enabled", auto_heal)
        heal_percent = ui.sliderfloat("Auto heal percent %", heal_percent, 1, 100)
        ui.treepop()
    ui.end()


def AutoHeal(game):
    global heal_percent
    heal = game.player.get_summoner_spell(SummonerSpellType.Heal)
    if heal == None:
        return

    hp = int(game.player.health / game.player.max_health * 100)
    if hp < heal_percent and heal.get_current_cooldown(game.time) == 0.0:
        heal.trigger(False)


def Ignite(game):
    ignite = game.player.get_summoner_spell(SummonerSpellType.Ignite)
    if ignite == None:
        return
    target = GetBestTargetsInRange(game, 550)
    if target and IsReady(game, ignite):
        IGdamage = 50 + 20 * game.player.lvl - (target.health_regen * 3)
        if target.health <= IGdamage:
            ignite.move_and_trigger(game.world_to_screen(target.pos))


def LViewPlus64_update(game, ui):
    global auto_heal, heal_percent, show_healable

    self = game.player

    if self.is_alive and self.is_visible and game.is_point_on_screen(self.pos):
        Ignite(game)
        if auto_heal:
            AutoHeal(game)
