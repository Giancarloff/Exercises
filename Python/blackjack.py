from random import randint as RAND_INT

class Card:

    def __init__(self, value: int, ace: bool) -> None:
        self.__value = value
        self.__ace = ace

    @property
    def value(self) -> int:
        return self.__value
    
    @property
    def is_ace(self) -> bool:
        return self.__ace
    
    def __str__(self) -> str:
        return f"{self.__value}"

def generate_deck() -> list[Card]:
    deck = list()
    i = 0
    while i < 53:
        value = i % 13
        deck.append(Card(value + 1, value == 1))
        i += 1

    return deck

def shuffle_deck(deck: list[Card]) -> list[Card]:
    deck_to_return = list()

    while 1 < len(deck):
        index = RAND_INT(1, len(deck)-1)
        card = deck.pop(index)
        deck_to_return.append(card)

    deck_to_return.append(deck.pop(0))

    return deck_to_return

def blackjack(deck: list[Card]) -> None:

    if len(deck) < 52:
        raise Exception(f"Expected 52 cards on the deck, got {len(deck)}")
    
    deck = shuffle_deck(deck)

    dealer_hand = list()
    player_hand = list()

    # Starting hands
    player_hand.append(deck.pop(0))
    dealer_hand.append(deck.pop(0))
    player_hand.append(deck.pop(0))
    dealer_hand.append(deck.pop(0))

    game_over = False
    flip_card = False

    player_score = 0
    dealer_score = 0

    while not game_over:
        player_score = sum([card.value for card in player_hand])
        dealer_score = sum([card.value for card in dealer_hand])

        if flip_card:
            print(f"DEALER: {[str(card) for card in dealer_hand]} => Sum: {dealer_score}")
        else:
            print(f"DEALER: {dealer_hand[0]} and a hidden card")
        print(f"YOU: {[str(card) for card in player_hand]} => Sum: {player_score}")

        if 21 <= player_score or 21 <= dealer_score:
            game_over = True

        if not game_over:
            print("Hit or stand?")
            option = input().strip().lower()
            if option == "hit":
                player_hand.append(deck.pop(0))
            elif option == "stand":
                flip_card = True
                while dealer_score <= 17:
                    card = deck.pop(0)
                    dealer_hand.append(card)
                    dealer_score += card.value
                game_over = True

    print("GAME OVER! CHECK SCORES BELOW")
    print(f"DEALER: {[str(card) for card in dealer_hand]} => Sum: {dealer_score}")
    print(f"YOU: {[str(card) for card in player_hand]} => Sum: {player_score}")

    if 21 < player_score:
        print("DEALER WINS")
    elif 21 < dealer_score:
        print("PLAYER WINS")
    else:
        if player_score < dealer_score:
            print("DEALER WINS")
        else:
            print("PLAYER WINS")


blackjack(generate_deck())

# GPT's feeback:

'''
Your code looks good to me. Here are some suggestions to improve it:

You could use a for loop instead of a while loop in the generate_deck() function to make it more concise.

[REDACTED]

You could add a __repr__() method to the Card class to make it easier to debug.

You could use an enumeration to define the suits of the cards instead of relying on integer division in the generate_deck() function.

You could add some input validation in the blackjack() function to handle invalid input from the user.

Overall, your code is well-organized and easy to understand. Good job!
'''




