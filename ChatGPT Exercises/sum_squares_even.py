import typing

def sum_squares_even(numbers: typing.Optional[int]) -> int:
    sum = 0
    for num in numbers:
        if num % 2 == 0:
            sum += num**2

    return sum

# POST FEEDBACK
def sum_squares_even(numbers: list[int] = []) -> int:
    if not isinstance(numbers, list):
        raise TypeError("Expected a list of integers")

    sum_of_squares = 0
    for num in numbers:
        if isinstance(num, int) and num % 2 == 0:
            sum_of_squares += num ** 2

    return sum_of_squares