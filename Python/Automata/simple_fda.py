# This is WIP
"""
TODO:
- Input validation on init
- Check undefined initial state in transition(); Fixing last item could solve this issue
- Empty string check in compute()
- Imp. str and repr
- Explain init parameters

File-built FDA TODO:
- Easy way (has all 5 definitions)
- Transition based (Can have only the transition but will search for other definitions)
"""
class FDA:

    """
    Finite Deterministic Automaton (FDA):

    A finite deterministic automaton is a 5-tuple (Q, Σ, δ, q₀, F), where:\n
    Q is a finite set of states.\n
    Σ is a finite input alphabet.\n
    δ: Q x Σ → Q is the transition function. For a deterministic automaton, this function needs to be defined for all (q, s) on Q x Σ.\n
    q₀ ∈ Q is the initial state.\n
    F ⊆ Q is the set of accepting states.\n
    It recognizes regular languages and processes strings by transitioning between states according to input symbols.\n
    """

    def __init__(self, states: set, alphabet: set, transition: dict, initial: object, accepting: set) -> None:
        """
        Note:\n
        - Alphabets have to be sets of single characters.\n
        - Transition is a dictionary of tuple keys and string values, think Transition[(q1, s)] = q2.\n
        """

        if states is not None:
            if states.issubset({}):
                print("Empty states set given! Using single-element set instead.")
                self.__states = {1}
            else:
                self.__states = states
        else:
            print("No states set given! Using single-ement set instead.")
            self.__states = {1}
            
        self.__alphabet = alphabet
        self.__transition_function = transition
        self.__initial = initial
        self.__accepting = accepting

        self.__current_state = initial

    @property
    def states(self) -> set:
        return self.__states

    @property
    def alphabet(self) -> set:
        return self.__alphabet

    @property
    def initial_state(self) -> str:
        return self.__initial

    @property
    def accepting_states(self) -> set:
        return self.__accepting
    
    @property
    def current_state(self) -> object:
        return self.__current_state
    
    @property
    def transition_function(self) -> dict:
        return self.__transition_function

    def transition(self, symbol: str) -> None:
        """
        Computes the transition (current_state, symbol) in this FDA.\n
        """
        if symbol in self.alphabet:
            consumed = (self.current_state, symbol)
            if consumed in self.transition_function.keys():
                self.__current_state = self.transition_function[consumed]
            else: # Better safe than sorry?
                print("No idea what is wrong - check better safe than sorry.")
        else:
            print("Error at %s: %s not in alphabet!" % self, symbol)

    def compute(self, string: str) -> bool:
        string_as_set = set(string)
        if string_as_set.issubset(self.alphabet):
            for c in string:
                self.transition(c)
            return self.current_state in self.accepting_states
        else:
            print("Unknown symbol in string!")
            return False

    def __str__(self) -> str:
        pass

    def __repr__(self) -> str:
        pass