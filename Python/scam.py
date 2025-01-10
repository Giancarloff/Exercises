import random

'''
I had a shower thought of trying to come up with emergent behaviour via simple simulations.
This script came out of that, though I will argue it does not accomplish any emergent behaviour.
Perhaps it will serve as a base or as practice for future attempts.
'''

class Strategy:
    RAISE = 0
    CONFIRM = 1
    DENY = 2
    ALL = [RAISE, CONFIRM, DENY]
    REPR = ["RAISE", "CONFIRM", "DENY"]

class Agent:

    def __init__(self, id: int, currency: int):
        self.id = id
        self.currency = currency
        self.strategy = None
        self.last_agreement = Strategy.CONFIRM
        self.repr = ""

    def always_raise(self) -> int:
        return Strategy.RAISE

    def always_confirm(self) -> int:
        return Strategy.CONFIRM
    
    def always_deny(self) -> int:
        return Strategy.DENY
    
    def random_uniform(self) -> int:
        choice = random.sample(Strategy.ALL, 1)
        return choice[0]

    def safe_rebound(self) -> int:
        choice = Strategy.CONFIRM
        if self.last_agreement == Strategy.DENY:
            choice = Strategy.DENY

        return choice
    
    def optimist_rebound(self) -> int:
        choice = Strategy.RAISE
        if self.last_agreement == Strategy.DENY:
            choice = Strategy.DENY

        return choice
    
    def __repr__(self):
        return f"({self.id}, {self.repr}, {self.currency})"
    
if __name__ == "__main__":

    N = 6
    agents = [Agent(i, 0) for i in range(N)]

    Nsixths = int(N / 6)
    for i in range(Nsixths):
        agents[i].strategy = agents[i].always_raise
        agents[i].repr = "RAISE"
        agents[Nsixths+i].strategy = agents[i].always_confirm
        agents[Nsixths+i].repr = "CONFIRM"
        agents[2*Nsixths+i].strategy = agents[i].always_deny
        agents[2*Nsixths+i].repr = "DENY"
        agents[3*Nsixths+i].strategy = agents[i].random_uniform
        agents[3*Nsixths+i].repr = "UNIFORM"
        agents[4*Nsixths+i].strategy = agents[i].safe_rebound
        agents[4*Nsixths+i].repr = "SAFE REBOUND"
        agents[5*Nsixths+i].strategy = agents[i].optimist_rebound
        agents[5*Nsixths+i].repr = "OPTIMIST REBOUND"

    # generated_values = [i*100 + 100 for i in range(100)]
    generated_values = [200]

    available_agents = agents.copy()
    stats: dict[int, list] = dict()

    for agent in agents:
        stats[agent.id] = list()

    v = 0
    runs = 10
    for _ in range(runs):
        for _ in range(N*100):
            x, y = random.sample(available_agents, 2)
            available_agents.remove(x)
            available_agents.remove(y)

            value = generated_values[v]
            dx = x.strategy()
            dy = y.strategy()

            x.last_agreement = dy
            y.last_agreement = dx
            # print(f"{x.id} : {Strategy.REPR[dx]} <=> {Strategy.REPR[dy]} : {y.id}")
            if dx == Strategy.CONFIRM and dy == Strategy.CONFIRM:
                x.currency += value / 2
                y.currency += value / 2
            elif dx == Strategy.RAISE and dy == Strategy.RAISE:
                x.currency += value
                y.currency += value
            elif dx == Strategy.DENY and dy == Strategy.DENY: 
                x.currency -= value
                y.currency -= value
            elif dx == Strategy.DENY and dy == Strategy.RAISE:
                x.currency += value * 1.5
                y.currency = 0 if y.currency < value else y.currency - value / 2
            elif dx == Strategy.RAISE and dy == Strategy.DENY:
                x.currency = 0 if x.currency < value else x.currency - value / 2
                y.currency += value * 1.5
            elif dx == Strategy.CONFIRM and dy == Strategy.DENY:
                y.currency += value
            elif dx == Strategy.DENY and dy == Strategy.CONFIRM:
                x.currency += value
            elif dx == Strategy.RAISE and dy == Strategy.CONFIRM:
                x.currency += value / 2
                y.currency += value / 2
            elif dx == Strategy.CONFIRM and dy == Strategy.RAISE:
                x.currency += value / 2
                y.currency += value / 2

            if len(available_agents) == 0:
                available_agents += agents

        for agent in agents:
            stats[agent.id].append(int(agent.currency))
            agent.currency = 0

    for agent in agents:
        print(agent, end=" Average: ")
        print(sum(stats[agent.id])/runs)