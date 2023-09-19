def find_pairs_original(numbers: list[int], target: int) -> list[tuple[int, int]]:
    pairs = list()
    for num in numbers:
        foo = target - num
        if foo in numbers:
            pairs.append(tuple((num, foo)))
            numbers.remove(num)
    
    return pairs

# GPT'S SOLUTION

def find_pairs(numbers: list[int], target: int) -> list[tuple[int, int]]:
    pairs = []
    seen = set()
    for num in numbers:
        diff = target - num
        if diff in seen:
            pairs.append((num, diff))
        seen.add(num)
    return pairs