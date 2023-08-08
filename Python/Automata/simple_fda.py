import random
# This is WIP
"""
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
    Note:\n
        - Alphabets have to be sets of single characters.\n
        - Transition is a dictionary of tuple keys and string values, think Transition[('q1', s)] = 'q2'.\n
    """

    def __init__(self, states: set, alphabet: set, transition: dict, initial_state: object, accepting: set) -> None:
        """
        Args:\n
            states (set): Set of objects representing the states.
            alphabet (set): Set of chars for the automaton's alphabet.
            transition (dict): Dictionary representing the transition function t:(q1, s) -> q2.
            initial_state (object): The initial/entering state for the FDA.
            accepting (set): Set of accepting states.
        """

        # STATES
        if states is not None:
            if states.issubset({}):
                print("Empty states set given! Using {0} instead.")
                self.__states = {0}
            else:
                self.__states = states
        else:
            print("No states set given! Using {0} instead.")
            self.__states = {0}

        # ALPHABET
        if alphabet is not None:
            if alphabet.issubset({}):
                print("Empty states set given! Using {'s'} instead.")
                self.__alphabet = {'s'}
            else:
                self.__alphabet = alphabet
        else:
            print("No states set given! Using {'s'} instead.")
            self.__alphabet = {'s'}
            
        # TRANSITION FUNCTION
        if transition is not None:
            if transition.keys().isdisjoint({}):
                self.__transition_function = transition
            else:
                print("Transition dictionary has no keys! Generating random one.")
                key = tuple(self.__initial_state, random.sample(self.__alphabet, 1)[0])
                value = random.sample(self.__states)
                generated_transition = dict()
                generated_transition[key] = value
                self.__transition_function = generated_transition
        else:
            print("No transition provided! Generating random one.")
            key = tuple(self.__initial_state, random.sample(self.__alphabet, 1)[0])
            value = random.sample(self.__states)
            generated_transition = dict()
            generated_transition[key] = value
            self.__transition_function = generated_transition

        # INITIAL STATE
        if initial_state is None:
            print("No initial state provided! Sampling a random one from states.")
            sample = random.sample(self.__states, 1)[0]
            self.__initial_state = sample
            self.__current_state = sample
        elif initial_state not in self.__states:
            print("Initial state is not in the set of states! Sampling another one.")
            sample = random.sample(self.__states, 1)[0]
            self.__initial_state = sample
            self.__current_state = sample
        else:
            self.__initial_state = initial_state
            self.__current_state = initial_state

        # ACCEPTING
        if accepting is None:
            print("Accepting state is None! Using random one from states.")
            sample = random.sample(self.__states, 1)[0]
            self.__accepting = sample
        elif accepting not in self.__states:
            print("Accepting state is not in states set! Using random one.")
            sample = random.sample(self.__states, 1)[0]
            self.__accepting = sample
        else:
            self.__accepting = accepting
        

    @property
    def states(self) -> set:
        return self.__states

    @property
    def alphabet(self) -> set:
        return self.__alphabet

    @property
    def initial_state(self) -> str:
        return self.__initial_state

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
            print(f"Error at {self}: {symbol} not in alphabet!")

    def compute(self, string: str) -> bool:

        if string is None or string == "":
            return self.initial_state in self.accepting_states

        if self.current_state != self.initial_state:
            self.__current_state = self.__initial_state
        
        string_as_set = set(string)
        if string_as_set.issubset(self.alphabet):
            for c in string:
                self.transition(c)
            return self.current_state in self.accepting_states
        else:
            print("Unknown symbol in string!")
            return False

    def __str__(self) -> str:
        return f"({self.states}, {self.alphabet}, {self.transition_function}, {self.initial_state}, {self.accepting_states})"

    def __repr__(self) -> str:
        return self.__str__()