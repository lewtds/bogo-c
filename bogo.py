#!/usr/bin/env python3

import copy
import re


class Tone:
    GRAVE = 0
    ACUTE = 1
    HOOK = 2
    TILDE = 3
    DOT = 4
    NONE = 5


class Mark:
    NONE = 0
    HAT = 1
    BREVE = 2
    HORN = 3
    BAR = 4


VOWELS = "àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
    "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy"

# FIXME This design cannot encode w-MARK-BREVE-a and w-MARK-HORN-o-w at the same
# time.
input_rules = [
    {
        "key": "s",
        "type": "TONE",
        "effect": Tone.ACUTE
    },
    {
        "key": "f",
        "type": "TONE",
        "effect": Tone.GRAVE
    },
    {
        "key": "x",
        "type": "TONE",
        "effect": Tone.TILDE
    },
    {
        "key": "j",
        "type": "TONE",
        "effect": Tone.DOT
    },
    {
        "key": "r",
        "type": "TONE",
        "effect": Tone.HOOK
    },
    {
        "key": "a",
        "type": "MARK",
        "effect": Mark.HAT,
        "on": "a"
    },
    {
        "key": "e",
        "type": "MARK",
        "effect": Mark.HAT,
        "on": "e"
    },
    {
        "key": "w",
        "type": "MARK",
        "effect": Mark.HORN,
        "on": "u"
    },
    {
        "key": "w",
        "type": "MARK",
        "effect": Mark.HORN,
        "on": "o"
    },
    {
        "key": "w",
        "type": "MARK",
        "effect": Mark.BREVE,
        "on": "a"
    },
    {
        "key": "d",
        "type": "MARK",
        "effect": Mark.BAR,
        "on": "d"
    },
    {
        "key": "w",
        "type": "APPEND",
        "effect": "ư"
    },
    {
        "key": "]",
        "type": "APPEND",
        "effect": "ư"
    },
    {
        "key": "[",
        "type": "APPEND",
        "effect": "ơ"
    },
]


#
# Utils
#

def is_vowel(chr):
    return chr != "" and chr in VOWELS


def next_append(trans_list, index):
    try:
        while trans_list[index + 1]["type"] != "APPEND":
            index += 1
        return trans_list[index + 1]
    except:
        return None

#
# Mark processing
#


def add_mark_to_char(chr, mark):
    famililies = [
        "aâă  ",
        "eê   ",
        "oô ơ ",
        "u  ư ",
        "d   đ"
    ]

    if chr == " ":
        return chr

    current_tone = get_tone_from_char(chr)
    stripped_chr = add_tone_to_char(chr, Tone.NONE)

    result = " "
    for family in famililies:
        try:
            family.index(stripped_chr)
            result = family[mark]
        except:
            pass

    result = add_tone_to_char(result, current_tone)

    return result if result != " " else chr


def find_mark_target(trans_list, trans):
    if trans["type"] != "MARK":
        return

    i = len(trans_list) - 1
    target_count = 0

    while i > -1 and target_count < len(trans["on"]):
        if trans_list[i]["type"] == "APPEND" and \
                trans_list[i]["key"] in trans["on"]:
            target_count += 1
            try:
                trans["target"] = i
            except:
                trans["target"] = i
        i -= 1

    if target_count == 0:
        trans["type"] = "APPEND"


#
# Tone processing
#

def get_tone_from_char(chr):
    try:
        position = VOWELS.index(chr)
        return position % 6
    except:
        return Tone.NONE


def add_tone_to_char(chr, tone):
    try:
        position = VOWELS.index(chr)
        current_tone = position % 6
        offset = tone - current_tone
        return VOWELS[position + offset]
    except:
        return chr


def find_rightmost_vowels(trans_list):
    i = len(trans_list) - 1
    end = -1
    vowel_indexes = []
    while i > -1:
        if trans_list[i]["type"] == "APPEND":
            if is_vowel(trans_list[i]["key"]):
                if end == -1:
                    end = i

                vowel_indexes.append(i)
            else:
                if end != -1:
                    break
        i -= 1

    vowel_indexes.reverse()
    return vowel_indexes


def find_tone_target(trans_list, trans):
    vowels = find_rightmost_vowels(trans_list)

    vlen = len(vowels)

    if vlen == 1:
        trans["target"] = vowels[0]
    elif vlen == 2:
        if next_append(trans_list, vowels[1]) is None:
            trans["target"] = vowels[0]
        else:
            trans["target"] = vowels[1]
    elif vlen == 3:
        trans["target"] = vowels[2]
    else:
        trans["type"] = "APPEND"


#
# Putting it all together
#


def process_char(trans_list, char):
    # A keypress can behave differently when it goes with a certain key
    # e.g.
    # Input: aouw
    # the letter w may add a breve to a or add a horn to both o and w
    applicable_rules = [copy.deepcopy(rule)
                        for rule in input_rules if rule["key"] == char]

    append_trans = {
        "type": "APPEND",
        "key": char,
        "effect": char
    }

    if len(applicable_rules) == 0:
        trans_list.append(append_trans)
    else:
        for rule in applicable_rules:
            if rule["type"] == "MARK":
                find_mark_target(trans_list, rule)
            elif rule["type"] == "TONE":
                find_tone_target(trans_list, rule)

        preprocessed_rules = [rule for rule in applicable_rules
                              if rule["type"] != "APPEND"]

        preprocessed_rules.sort(key=lambda rule: rule["target"], reverse=True)

        l = len(preprocessed_rules)
        if l >= 2:
            fst_tgt = preprocessed_rules[0]["target"]
            snd_tgt = preprocessed_rules[1]["target"]

            if trans_list[fst_tgt]["effect"] == "o" and \
                    trans_list[snd_tgt]["effect"] == "u":
                trans_list.append(preprocessed_rules[1])
                trans_list.append(preprocessed_rules[0])
        elif l == 1:
            trans_list.append(preprocessed_rules[0])
        else:
            trans = {
                "type": "APPEND",
                "key": char,
                "effect": char
            }

    trans_list.append(trans)


def process_string(string, trans_list=None):
    if type(trans_list) is not list:
        trans_list = []
    for char in string:
        process_char(trans_list, char)

    return flatten(trans_list), trans_list


def flatten(trans_list):

    def apply_trans(trans, function):
        target_index = trans["target"]
        dest_index = trans_list[target_index]["dest"]
        result[dest_index] = function(
            result[dest_index], trans["effect"])

    result = []
    for trans in trans_list:
        if trans["type"] == "APPEND":
            result.append(trans["effect"])
            trans["dest"] = len(result) - 1
        elif trans["type"] == "TONE":
            apply_trans(trans, add_tone_to_char)
        elif trans["type"] == "MARK":
            apply_trans(trans, add_mark_to_char)

    flat_tmp = "".join(result)
    flat_tmp = re.sub('(.*)u([ờớởỡợơ].+)', '\\1ư\\2', flat_tmp)
    return flat_tmp


#
# Miscellaneous operations
#


def undo(trans_list):
    trans_list = copy.deepcopy(trans_list)

    i = len(trans_list) - 1
    while i > -1:
        if trans_list[i]["type"] in ("MARK", "TONE"):
            trans_list[i]["type"] = "UNDONE"
            break
        i -= 1

    return trans_list


def remove_last_char(trans_list):
    trans_list = copy.deepcopy(trans_list)

    # Find the latest APPEND transformation
    i = len(trans_list) - 1
    last_append_index = -1
    while i > -1:
        if trans_list[i]["type"] == "APPEND":
            last_append_index = i
            break
        i -= 1

    if last_append_index == -1:
        return trans_list

    # Then find all transformations that target
    # that APPEND transformation
    remove_list = [last_append_index]
    for i, trans in enumerate(trans_list):
        try:
            if last_append_index == trans["target"]:
                remove_list.append(i)
        except KeyError:
            pass

    # Finally, remove those transformations
    remove_list.sort()
    remove_list.reverse()
    for index in remove_list:
        trans_list.pop(index)

    return trans_list


def clear_transformations(trans_list):
    trans_list = copy.deepcopy(trans_list)
    for trans in trans_list:
        trans["type"] = "APPEND"
        trans["effect"] = trans["key"]

    return trans_list


test_array = [
    {
        "type": "APPEND",
        "key": "h",
        "effect": "h"
    },
    {
        "type": "APPEND",
        "key": "a",
        "effect": "a"
    },
    {
        "type": "TONE",
        "key": "f",
        "effect": Tone.GRAVE,
        "target": 1
    },
    {
        "type": "APPEND",
        "key": "n",
        "effect": "n"
    }
]

test_array2 = [
    {
        "type": "APPEND",
        "key": "h",
        "effect": "h"
    },
    {
        "type": "APPEND",
        "key": "u",
        "effect": "u"
    },
    {
        "type": "APPEND",
        "key": "o",
        "effect": "o"
    },
    {
        "type": "MARK",
        "key": "w",
        "effect": Mark.HORN,
        "target": 2
    }
]

print(flatten(test_array))        # hàn
print(flatten(undo(test_array)))  # han
print(flatten(clear_transformations(undo(test_array))))  # hafn

print(flatten(test_array2))       # hươ
print(flatten(remove_last_char(test_array2)))  # hư

print(process_string("meofe")[0])      # mều
print(process_string("dieend")[0])    # điên
print(process_string("vaix")[0])      # vãi
print(process_string("chuongwr")[0])  # chưởng
print(process_string("w")[0])  # chưởng
