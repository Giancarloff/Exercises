def second_largest(numbers: list) -> int:
    if not isinstance(numbers, list):
        raise TypeError("Expected list")
    
    if len(numbers) < 2:
        return None
    
    biggest = max(numbers)
    while biggest in numbers:
        numbers.remove(biggest)

    return max(numbers)

# POST FEEDBACK 
# NOTE: The first time I asked for feedback, ChatGPT did not provide an alternative code but only some
# edge cases comment. I had to explicitly ask for ChatGPT to revise the code, so I take that as an exercise well done.

def second_largest(numbers: list) -> int:
    if not isinstance(numbers, list):
        raise TypeError("Expected list")
    
    if len(numbers) < 2:
        return None
    
    numbers = sorted(set(numbers), reverse=True)
    if len(numbers) > 1:
        return numbers[1]
    else:
        return None