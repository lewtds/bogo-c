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
        "aâăaa",
        "eêeee",
        "oôoơo",
        "uuuưu",
        "ddddđ"
    ]
    for family in famililies:
        try:
            family.index(chr)
            return family[mark]
        except:
            pass
    return chr


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
            trans_list.pop(i)
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
print(flatten(test_array2))       # hươ
print(flatten(remove_last_char(test_array2)))  # hư
