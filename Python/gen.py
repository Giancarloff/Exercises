import random

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
        return f"{self.number}"
    
    def __str__(self) -> str:
        return f"{self.number}"

class Biote:

    def __init__(self, starting_genes: list[Quaternary] = list()) -> None:
        self.gene = starting_genes

    def __repr__(self) -> str:
        return f"{self.gene}"

    def __str__(self) -> str:
        return f"{self.gene}"

class Biome:

    def __init__(self, starting_biome: Quaternary) -> None:
        self.biome = starting_biome

class Evolution:

    def __sv_equality(self):
        for biote in self.biotes:
            if not any(gene for gene in biote.gene if gene == self.biome.biome):
                self.unfit.append(biote)
            else:
                self.fit.append(biote)

    def __sv_base_diff(self):
        for biote in self.biotes:
            gene_stuff = [gene for gene in biote.gene if gene.to_int() + Quaternary.b == self.biome.biome or gene.to_int() - Quaternary.b == self.biome.biome]
            print(gene_stuff)
            if not any(gene_stuff):
                self.unfit.append(biote)
            else:
                self.fit.append(biote)

    SV_EQUALITY = 0
    SV_BASE_DIFF = 1

    __SURVIVAL_FUNC_LIST = [
        __sv_equality,
        __sv_base_diff
    ]

    ############

    def __rp_bipartition(self):
        for biote in self.fit:
            new_biote = Biote(list(biote.gene))
            mutation = random.randint(0, 4)
            random.sample(new_biote.gene, 1)[0].number += Quaternary(mutation)

    RP_BIPARTITION = 0

    __RP_FUNC_LIST = [
        __rp_bipartition
    ]

    def __init__(self, biome: Biome, biotes: list[Biote], survival_function: int, reproduction_function: int) -> None:
        self.biome = biome
        self.biotes = biotes
        self.fit: list[Biote] = list()
        self.unfit: list[Biote] = list()
        self.__sv_function = Evolution.__SURVIVAL_FUNC_LIST[survival_function]
        self.__rp_function = Evolution.__RP_FUNC_LIST[reproduction_function]

    def reproduce(self):
        for _ in self.fit:
            self.__rp_function
        
    def tick(self):
        for _ in self.biotes:
            self.__sv_function(self)
        for biote in self.unfit:
            if biote in self.biotes:
                self.biotes.remove(biote)
        for _ in self.fit:
            self.__rp_function(self)

def main():
    print("Starting simulation.")

    print("=> Generating Biome.")
    biome_num = 16
    biome_q = Quaternary(biome_num)
    biome = Biome(biome_q)
    print("=> Biome generated.")

    print("=> Generating Biotes.")
    biote_amount = 10
    gene_amount = 2
    gene_biome_deviation = 8
    if gene_biome_deviation > biome_num:
        print("!!! gene_biome_deviation is greater than biome_num. This can not be (Quaternary does not support negative numbers).")
        return
    gene_interval_min = biome_num - gene_biome_deviation
    gene_interval_max = biome_num + gene_biome_deviation
    biotes: list[Biote] = list()
    gene_list: list[Quaternary] = list()

    for _ in range(biote_amount):
        for _ in range(gene_amount):
            gene_number = random.randint(gene_interval_min, gene_interval_max)
            new_gene = Quaternary(gene_number)
            gene_list.append(new_gene)
        new_biote = Biote(list(gene_list))
        biotes.append(new_biote)
        gene_list.clear()

    print("=> Biotes generated.")
    print("=> Would you like to control the ticks (by pressing enter each round)? y/n")

    wants_control = input()
    will_control = False

    match wants_control:

        case "y":
            will_control = True
        case "Y":
            will_control = True
        case "n":
            pass
        case "N":
            pass
        case _:
            print("=> Input not identified. Defaulting to tick control.")
    

    print("=> Opening/Creating log file.")

    output_file = open("gen_output.log", "w")

    print("=> Starting evolution.")

    O = Evolution(biome, biotes, Evolution.SV_EQUALITY, Evolution.RP_BIPARTITION)
    i = 0

    while i < 50:
        print(f"==> Biome: {O.biome.biome}")
        output_file.write(f"==> Biome: {O.biome.biome}\n")
        print(f"==> Biotes alive in t = {i}: ", end="")
        output_file.write(f"==> Biotes alive in t = {i}: ")
        for biote in O.biotes:
            print(f"{biote.gene}", end=" ")
            output_file.write(f"{biote.gene} ")
        output_file.write("\n")
        print()
        O.tick()
        i += 1
        if will_control:
            input()

    print("=> Evolution ended.")

    print("=> Closing log file.")
    output_file.close()

    print("Exiting simulation.")
    
if __name__ == "__main__":
    main()
