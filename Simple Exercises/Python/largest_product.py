# n³ because i'm running on 3 hour sleep and am too tired to think
def largest_product(nums: list) -> int:

    for n1 in nums:
        for n2 in nums:
            for n3 in nums:
                fprod = n1*n2*n3
                if (prod < fprod):
                    prod = fprod
    
    return prod