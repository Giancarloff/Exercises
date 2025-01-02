# !py/thon

'''
This is meant to be a grammar that generates grammars. I will call this grammar a 'grammar-scheme' in this comment block.
I do not know if this is correct (neither the grammar-scheme nor the script), very little thought was given to this.

terminal_vars are terminals of the grammar-scheme that will be interpreted as variables in the words they appear. 
Note that words of the grammar-scheme are formal grammar definitions (ish). terminal_trms are analogous but for terminals of the 
generated grammar. terminal_struct are terminals of the grammar-scheme meant to facilitate the interpretation of generated
words by giving them a lookalike structure of regular formal grammars with "head -> tail" productions.

I am trying to generate words of the form

|head > tail||other head > other tail|;|third head > third tail|; ...

where head and tail are strings of terminals of the grammar-scheme which would then be interpreted as either
variables of terminals of the generated grammar. The example above has two grammars, one with two productions
and the other with just one.

Not proud of this code but I'll leave it in here in case I ever come back to it.
'''

variables = set("SNFRHT$")
terminal_struct = set("|;>")
terminal_vars = set("UVWXYZ")
terminal_trms = set("abcdef")

terminals = set(terminal_struct | terminal_vars | terminal_trms)

productions: dict[str, list] = {
    "S" : ["", "N"],
    "N" : ["|R|F"],
    "F" : ["N", ";S"],
    "R" : ["$H > T"],
    "H" : ["", "$H"],
    "T" : ["", "$T"],
    "$" : []
}

for t in [x for x in terminals if x in terminal_vars or x in terminal_trms]:
    productions["$"].append(t)

class Node:

    def __init__(self, data: str, left, right):
        self.data = data
        self.left = left
        self.right = right

def treeprint(root: Node, level = 0):
    if root is None:
        return
    
    print("\t"*level, end="")
    print(root.data)
    treeprint(root.left, level + 1)
    treeprint(root.right, level + 1)

def print_words(root: Node):
    if root is None:
        return
    
    for c in root.data:
        if c in variables:
            return
    
    print(root.data)
    print_words(root.left)
    print_words(root.right)

def generate(root: Node):
    if (root.left is None and root.right is None):
        for c in root.data:
            if c in variables:
                if len(productions[c]) == 1:
                    new_word = root.data.replace(c, productions[c][0])
                    root.left = Node(new_word, None, None)
                else:
                    left_word = root.data.replace(c, productions[c][0])
                    root.left = Node(left_word, None, None)
                    right_word = root.data.replace(c, productions[c][1])
                    root.right = Node(right_word, None, None)
    else:
        if (root.left is not None):
            generate(root.left)
        if (root.right is not None):
            generate(root.right)
                

root = Node("S", None, None)

generating = True
i = 1
ctr = 1
step = 10
while generating:
    print(i)
    generate(root)
    print_words(root)
    print()
    i += 1
    ctr += 1
    if (ctr > step):
        print(f"Generate {step} more rounds? [y/n]")
        if (input() == "y"):
            ctr = 1
        else:
            generating = False