'''
TODO:
    - Revisitar modelagem das transições, talvez deixar a classe da maquina mais parecida com a formulação teórica
    - Terminar função de cálculo de número da fita atual __converter_fita_para_numero
'''


class AcaoDeCabecalhoInvalida(Exception):

    def __init__(self, *args: object) -> None:
        super().__init__(*args)

    def __str__(self) -> str:
        return f"{self.args[0]}: Ação de cabeçalho inválida!"
    
    def __repr__(self) -> str:
        return f"{self.args[0]}: Ação de cabeçalho inválida!"
    
class CaractereDesconhecido(Exception):

    def __init__(self, *args: object) -> None:
        super().__init__(*args)

    def __str__(self) -> str:
        return f"{self.args[0]}: Caractere desconhecido, não pertence ao alfabeto {self.args[1]}"
    
    def __repr__(self) -> str:
        return f"{self.args[0]}: Caractere desconhecido, não pertence ao alfabeto! {self.args[1]}"
    

class Estado:

    def __init__(self, id: int, inicial: bool = False, parada: bool = False) -> None:
        self.inicial = inicial   
        self.parada = parada
        self.id = id

class MaquinaTuring:

    ESQ = -1
    NMV = 0
    DIR = 1

    '''
    Esperamos que o alfabeto inclua conetudo de fita e conteudo de escrita.
    Deve ser chamada inicializar() apos o construtor e antes de qualquer outro metodo do objeto.
    '''
    def __init__(self, fita: list, estados: set[Estado], transicoes: dict[tuple, Estado], alfabeto: set) -> None:
        self.fita = fita
        self.cabecalho = 0
        self.estados = estados
        self.transicoes = transicoes
        self.alfabeto = alfabeto
        self.estado = next(e for e in estados if e.inicial).id

    def inicializar(self):
        self.configuracao = (self.fita[self.cabecalho], self.cabecalho, self.estado)
        self.estados_enumerados = enumerate(self.estados)
        self.alfabeto_enumerado = enumerate(self.alfabeto)

    def transitar(self, acao_cabecalho: int = None, escreve: bool = False, caractere: str = None):
        configuracao_atual = tuple(self.configuracao)
        fita_atual = list(self.fita)
        
        proximo_estado = self.transicoes[configuracao_atual].id
        proximo_cabecalho = int(self.cabecalho)
        proxima_configuracao = None

        if escreve:
            if caractere not in self.alfabeto:
                raise CaractereDesconhecido(self)
            fita_atual[self.cabecalho] = caractere

        if acao_cabecalho:
            proximo_cabecalho = self.cabecalho + acao_cabecalho
        else:
            raise AcaoDeCabecalhoInvalida # raise já retorna
        
        proxima_configuracao = (fita_atual[proximo_cabecalho], self.cabecalho, proximo_estado)

        # Atualiza a maquina
        self.fita = list(fita_atual)
        self.configuracao = tuple(proxima_configuracao)
        self.cabecalho = int(proximo_cabecalho)

        # Ajudando o GC
        del fita_atual
        del proxima_configuracao
        del proximo_cabecalho

    def calcular_assinatura(self) -> int:
        exp2 = self.__converter_fita_para_numero(self.fita)
        exp3 = int(self.cabecalho)
        exp5 = next(n for n, e in self.estados_enumerados if e == self.estado)
        
        return (2**exp2)*(3**exp3)*(5**exp5)

    def __converter_fita_para_numero(self, fita: list) -> int:
        indice = 0
        base = len(self.alfabeto)
        numero_base = 0
        numero_decimal = 0
        
        for l in fita:
            numero_base += next(n for n, c in self.alfabeto_enumerado if l == c)*(base**indice)
            indice += 1

        print(numero_base)


def main():

    estados = {Estado(0, True), Estado(5, False, True)}.union({Estado(i) for i in range(1, 5)})
    transicoes = {
        ('a', 0) : 1,
        ('b', 0) : 0,
        ('a', 1) : 2,
        ('b', 0) : 3
    }
    M = MaquinaTuring(list('aaabbb'), estados, )