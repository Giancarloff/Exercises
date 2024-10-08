class Quaternary:
    
    b = 4
    BASE = 4
    
    @classmethod
    def to_quaternary(cls, num: int, prefix = False) -> str:
        new_num = str()
        while num != 0:                
            new_num += f"{num % cls.b}"
            num = num // cls.b
        
        new_num = new_num[::-1]
        
        if prefix:
            return "0q" + new_num
        
        return new_num
    
    def to_int(self) -> int:
        int_num = 0
        self_num = self.number
        
        if self.prefixed:
            self_num = self_num[-1:1:-1]
            self_num = self_num[::-1]
            
        self_num = self_num[::-1]
        
        for i, c in enumerate(self_num):
            int_num += int(c)*(Quaternary.b**i)
        
        return int_num
    
    def __init__(self, num: int, prefixed = False) -> None:
        self.number = Quaternary.to_quaternary(num, prefixed)
        self.prefixed = prefixed

    def __add__(self, other):
        if isinstance(other, Quaternary):
            int_sum = self.to_int() + other.to_int()
            return Quaternary(int_sum)
        raise TypeError(f"Invalid type for sum (+) {type(self)} + {type(other)}")

    def __sub__(self, other):
        if isinstance(other, Quaternary):
            int_sub = self.to_int() - other.to_int()
            return Quaternary(int_sub)
        raise TypeError(f"Invalid type for sub (-) {type(self)} - {type(other)}")

    def __mul__(self, other):
        if isinstance(other, Quaternary):
            int_mul = self.to_int() * other.to_int()
            return Quaternary(int_mul)
        raise TypeError(f"Invalid type for mul (*) {type(self)} * {type(other)}")

    def __truediv__(self, other):
        if isinstance(other, Quaternary):
            int_div = self.to_int() / other.to_int()
            return Quaternary(int_div)
        raise TypeError(f"Invalid type for div (/) {type(self)} / {type(other)}")
    
    def __repr__(self) -> str:
        return f"({self.number}, {self.prefixed}"
    
    def __str__(self) -> str:
        return f"{self.number}"
    


class Gene:
    
    def __init__(self, starting_genes: list[Quaternary] = list()) -> None:
        self.gene = starting_genes
        
    
    
        

qua = Quaternary(10)
que = Quaternary(11)
print(qua+que)
        
