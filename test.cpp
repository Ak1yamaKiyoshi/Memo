#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

enum ability {
    ab_add, ab_remove, ab_update, ab_search, ab_all, ab_repetitions, ab_asciiart
};

int get_value(ability action);

struct gamestate {
    int credits;
    std::vector<enum ability> av_abilities;
};

void evaluate_state(ability action, gamestate &state); 
bool evaluate_buy(ability action, gamestate &state);

void gamestate_to_file(const gamestate &state);
gamestate gamestate_from_file();

// - - - 

int main() {
    return 0;
}
