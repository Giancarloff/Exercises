#include <iostream>
#include <string>
#include <ctime>

int main() {

    const char newl = '\n';

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Get the user's choice
    std::cout << "Choose rock, paper, or scissors: ";
    std::string user_choice;
    std::cin >> user_choice;

    std::string cpu_choices[3] = {"rock", "paper", "scissors"};

    int cpu_rand = std::rand() % 2;
    std::string cpu_play = cpu_choices[cpu_rand];

    std::cout << "CPU: " << cpu_play << newl;

    if (cpu_play == user_choice) {
        std::cout << "DRAW" << newl;
    } else {

        // Feels like there is a better way
        if (cpu_play == "rock") {
            if (user_choice == "paper") {
                std::cout << "PLAYER WINS" << newl;
            } else {
                std::cout << "CPU WINS" << newl;
            }
        }

        if (cpu_play == "paper") {
            if (user_choice == "scissors") {
                std::cout << "PLAYER WINS" << newl;
            } else {
                std::cout << "CPU WINS" << newl;
            }
        }

        if (cpu_play == "scissors") {
            if (user_choice == "rock") {
                std::cout << "PLAYER WINS" << newl;
            } else {
                std::cout << "CPU WINS" << newl;
            }
        }
    }
    return 0;
}
