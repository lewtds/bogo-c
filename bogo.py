#!/usr/bin/env python3

import copy


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
input_rules = {
    "s": {
        "type": "TONE",
        "effect": Tone.ACUTE
    },
    "f": {
        "type": "TONE",
        "effect": Tone.GRAVE
    },
    "x": {
        "type": "TONE",
        "effect": Tone.TILDE
    },
    "j": {
        "type": "TONE",
        "effect": Tone.DOT
    },
    "r": {
        "type": "TONE",
        "effect": Tone.HOOK
    },
    "a": {
        "type": "MARK",
        "effect": Mark.HAT,
        "affinity": ["a"]
    },
    "e": {
        "type": "MARK",
        "effect": Mark.HAT,
        "affinity": ["e"]
    },
    "w": {
        "type": "MARK",
        "effect": Mark.HORN,
        "affinity": ["u", "o"]
    },
    "d": {
        "type": "MARK",
        "effect": Mark.BAR,
        "affinity": ["d"]
    }
}


def is_vowel(chr):
    return chr != "" and chr in VOWELS


def next_append(trans_list, index):
    try:
        while trans_list[index + 1]["type"] != "APPEND":
            index += 1
        return trans_list[index + 1]
    except:
        return None


def add_tone_to_char(chr, tone):
    try:
        position = VOWELS.index(chr)
        current_tone = position % 6
        offset = tone - current_tone
        return VOWELS[position + offset]
    except:
        return chr


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

    result = " "
    for family in famililies:
        try:
            family.index(chr)
            result = family[mark]
        except:
            pass

    return result if result != " " else chr


def find_mark_target(trans_list, trans):
    if trans["type"] != "MARK":
        return

    i = len(trans_list) - 1
    target_count = 0

    while i > -1 and target_count < len(trans["affinity"]):
        if trans_list[i]["type"] == "APPEND" and \
                trans_list[i]["key"] in trans["affinity"]:
            target_count += 1
            try:
                trans["target"].append(i)
            except:
                trans["target"] = [i]
        i -= 1

    if target_count == 0:
        trans["type"] = "APPEND"


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
        trans["target"] = [vowels[0]]
    elif vlen == 2:
        if next_append(trans_list, vowels[1]) is None:
            trans["target"] = [vowels[0]]
        else:
            trans["target"] = [vowels[1]]
    elif vlen == 3:
        trans["target"] = [vowels[2]]


def process_char(trans_list, chr):
    if not chr in input_rules:
        trans = {
            "type": "APPEND",
            "key": chr
        }
    else:
        trans = copy.deepcopy(input_rules[chr])
        trans["key"] = chr
        if trans["type"] == "MARK":
            find_mark_target(trans_list, trans)
        elif trans["type"] == "TONE":
            find_tone_target(trans_list, trans)

    trans_list.append(trans)


def process_string(string):
    trans_list = []
    for char in string:
        process_char(trans_list, char)

    return flatten(trans_list)


def flatten(trans_list):

    def apply_trans(trans, function):
        for target in trans["target"]:
            dest_index = trans_list[target]["dest"]
            result[dest_index] = function(
                result[dest_index], trans["effect"])

    result = []
    for trans in trans_list:
        if trans["type"] == "APPEND":
            result.append(trans["key"])
            trans["dest"] = len(result) - 1
        elif trans["type"] == "TONE":
            apply_trans(trans, add_tone_to_char)
        elif trans["type"] == "MARK":
            apply_trans(trans, add_mark_to_char)

    return "".join(result)


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
            if last_append_index in trans["target"]:
                if len(trans["target"]) == 1:
                    remove_list.append(i)
                else:
                    trans["target"].remove(last_append_index)
        except:
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

    return trans_list


test_array = [
    {
        "type": "APPEND",
        "key": "h"
    },
    {
        "type": "APPEND",
        "key": "a"
    },
    {
        "type": "TONE",
        "key": "f",
        "effect": Tone.GRAVE,
        "target": [1]
    },
    {
        "type": "APPEND",
        "key": "n"
    }
]

test_array2 = [
    {
        "type": "APPEND",
        "key": "h"
    },
    {
        "type": "APPEND",
        "key": "u"
    },
    {
        "type": "APPEND",
        "key": "o"
    },
    {
        "type": "MARK",
        "key": "w",
        "effect": Mark.HORN,
        "target": [1, 2]
    }
]

print(flatten(test_array))        # hàn
print(flatten(undo(test_array)))  # han
print(flatten(clear_transformations(undo(test_array))))  # hafn

print(flatten(test_array2))       # hươ
print(flatten(remove_last_char(test_array2)))  # hư

print(process_string("meof"))      # mèo
print(process_string("dieend"))    # điên
print(process_string("vaix"))      # vãi
print(process_string("chuongwr"))  # chưởng
