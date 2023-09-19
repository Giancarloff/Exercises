import typing

def is_palindrome(_string: str) -> bool:
    only_alphanum = ""
    for c in _string:
        if c.isalpha():
            only_alphanum += c

    only_alphanum = only_alphanum.lower()
    return only_alphanum == only_alphanum[::-1]

# POST FEEBACK

def is_palindrome_gpt(text: typing.Optional[str]) -> bool:
    only_alphanum = "".join(c.lower() for c in text if c.isalnum())

    return only_alphanum == only_alphanum[::-1]
