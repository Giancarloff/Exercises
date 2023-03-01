def max_frequency(numbers: list[int] = []):
    if not isinstance(numbers, list):
        raise TypeError("Expected a list")
    
    max_freq = 0
    for num in numbers:
        count = numbers.count(num)
        if max_freq < count:
            max_freq = count

    nums_with_max_freq = list()
    for num in numbers:
        if numbers.count(num) == max_freq:
            nums_with_max_freq.append(num)
    
    nums_with_max_freq.sort()

    return nums_with_max_freq[0]

# POST FEEDBACK
# NOTE: ChatGPT forgot it was supposed to return the smallest integer with the highest frequency

def max_frequency_gpt(numbers: list) -> int:
    if not numbers:
        raise ValueError("Input list cannot be empty")

    counts = {}
    for num in numbers:
        counts[num] = counts.get(num, 0) + 1
    
    max_count = max(counts.values())
    nums_with_max_count = [num for num, count in counts.items() if count == max_count]
    
    return min(nums_with_max_count)
