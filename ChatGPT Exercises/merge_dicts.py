def merge_dicts_original(dict1: dict, dict2: dict) -> dict:
    merged_dict = dict(dict2)
    for key in dict1.keys():
        if key not in dict2.keys():
            merged_dict[key] = dict1[key]

    return merged_dict

# GPT

def merge_dicts(source_dict: dict, target_dict: dict) -> dict:
    merged_dict = {**target_dict, **source_dict} # First time I'm seeing this unpacking operator
    return merged_dict
