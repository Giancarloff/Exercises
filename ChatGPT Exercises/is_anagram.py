def is_anagram_original(s1: str, s2: str) -> bool:
    s1 = s1.lower()
    s2 = s2.lower()

    if len(s1) != len(s2):
        return False

    for char in s1:
        if char not in s2:
            return False
    
    return True

# GPT's Code

def is_anagram(s1: str, s2: str) -> bool:
    s1 = s1.lower()
    s2 = s2.lower()

    if len(s1) != len(s2):
        return False

    s1_chars = sorted(list(s1))
    s2_chars = sorted(list(s2))

    return s1_chars == s2_chars
