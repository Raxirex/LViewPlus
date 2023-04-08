from LViewPlus64 import *
from commons.utils import *
from commons.skills import *
from commons.items import *
from commons.targeting import *
from evade import checkEvade
import json, time
from pprint import pprint

LViewPlus64_script_info = {
    "script": "Aim",
    "author": "bckd00r",
    "description": "Aim helper",
}

cast_keys = {"Q": 0, "W": 0, "E": 0, "R": 0}


def LViewPlus64_load_cfg(cfg):
    global cast_keys
    cast_keys = json.loads(cfg.get_str("cast_keys", json.dumps(cast_keys)))


def LViewPlus64_save_cfg(cfg):
    global cast_keys
    cfg.set_str("cast_keys", json.dumps(cast_keys))


def LViewPlus64_draw_settings(game, ui):
    global cast_keys
    for slot, key in cast_keys.items():
        cast_keys[slot] = ui.keyselect(f"Key to cast {slot}", key)


def LViewPlus64_update(game, ui):
    global cast_keys

    if game.player.is_alive and game.player.is_visible and not game.isChatOpen:
        for slot, key in cast_keys.items():
            if game.was_key_pressed(key):
                skill = getattr(game.player, slot)
                target = GetBestTargetsInRange(game, skill.cast_range)
                cursor = game.get_cursor()
                if IsReady(game, skill):
                    if target:
                        if cursor.distance(game.world_to_screen(target.pos)) <= 100:
                            game.draw_circle_world(target.pos, 200, 100, 1, Color.RED)
                            cast_point = castpoint_for_collision(
                                game, skill, game.player, target
                            )
                            skill.move_and_trigger(game.world_to_screen(cast_point))
