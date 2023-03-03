def is_pangram_original(input_str: str) -> bool:
    alphabet = set("abcdefghijklmnopqrstuvwxyz")

    lower_input = input_str.lower()

    for letter in alphabet:
        if letter not in lower_input:
            return False
        
    return True

# GPT:

def is_pangram(input_str: str) -> bool:
    alphabet = set("abcdefghijklmnopqrstuvwxyz")
    lower_input = input_str.lower()
    return all(letter in lower_input for letter in alphabet)

