#include "random.h"

#include <iostream>
#include <array>
#include <algorithm>

//#define DEBUG_MODE

namespace Potion{
    enum Type{
        healing,
        mana,
        speed,
        invisibility,
        big_cock,
        big_titty_goth_gf_summoning,
        max_potions,
    };
    constexpr std::array<int, max_potions> potion_price{20, 20, 15, 60, 100, 1000};
    constexpr std::array<std::string_view, max_potions> potion_name{"healing", "mana", "speed", "invisibility", "big cock", "big titty goth gf summoning"};

    constexpr std::array<Potion::Type, max_potions> types{healing, mana, speed, invisibility, big_cock, big_titty_goth_gf_summoning};

};

class Player{
    std::string name {"Balls"};
    std::array <int, Potion::max_potions> potion_inventory {};

    int inventory_size {0};
    int max_inventory_size {15};

    int gold {0};


public:
    // debug feature
#ifdef DEBUG_MODE
    explicit Player(std::string_view givenName):
            name {givenName},
            gold {10000}
    {
    }
#endif
#ifndef DEBUG_MODE
    explicit Player(std::string_view givenName):
        name {givenName},
        gold {Random::get(1000, 1200)}
    {
    }
#endif
    std::string_view getName(){return name;}

    int getGold() const {return gold;}
    void setGold(int newGold ){gold = newGold;}

    std::array<int, Potion::max_potions> getInventory() const {return potion_inventory;};
    void setInventory( Potion:: Type t ) {
        potion_inventory[t] += 1;
    }
    
    void inventoryDefaultValueSetter(){ // fills the inventory array with zeroes
        std::fill_n(potion_inventory.begin(), Potion::max_potions, 0);
    }

    int getInventorySize() const {return inventory_size;}
    void setInventorySize(){
        // i don't know how I would set the size without setting it to 0 first
        inventory_size = 0;

        for (auto i {0}; i < Potion::max_potions; i++){
            if (potion_inventory[i] != 0)
                inventory_size += potion_inventory[i];
        }
    }

    int getMaxInventorySize() const {return max_inventory_size;}

};

void printInventory(const Player& player){
    const std::array<int, Potion::max_potions> numericInventory {player.getInventory()};

    for (auto i {0}; i < Potion::max_potions; i++){
        if (numericInventory[i] != 0)
            std::cout << numericInventory[i] << "x " << Potion::potion_name[i] << " potion. \n";
    }
    std::cout << '\n';
};

void purchase(Player& player){
    while(true){
        player.setInventorySize();
        char selection{};
        int gold = player.getGold();
        int inventorySize = player.getInventorySize();

        if (inventorySize < player.getMaxInventorySize() && gold > 0) {
            std::cout << "What do you want to purchase? (or say q to quit)";
            while (true) {
                std::cin >> selection;

                if (!std::cin)
                {
                    //  handle the failure
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // And remove the bad input
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }

            // convert char to int unless the selection is q
            if (selection != 'q') selection -= '0';

            if (selection >= Potion::max_potions || selection < 0 && selection != 'q'){ // index out of range
                std::cout << "There's no such potion.\n";
                continue;
            }  else if (selection == 'q') { // quit
                break;
            } else if (Potion::potion_price[selection] <= gold && selection != -1 && inventorySize < player.getMaxInventorySize()) { // purchase
                player.setInventory(Potion::types[selection]);

                gold -= Potion::potion_price[selection];
                player.setGold(gold);
                std::cout << "You got a " << Potion::potion_name[selection] << " potion. \n" ;
                std::cout << "Thanks for the purchase. You now have " << gold << " gold.\n\n";
                continue;
            } else if (gold < Potion::potion_price.at(selection)){ // too poor for that item
                std::cout << "You don't have enough money to buy that. \n\n";
                continue;
            } else if (gold <= 15){ // too poor to buy the cheapest item
                std::cout << "You don't have enough money to buy anything.\n\n";
                break;
            }

        } else if (gold <= 15){ // too poor to buy the cheapest item
            std::cout << "You don't have enough money to buy anything.\n\n";
            break;
        } else if (inventorySize == player.getMaxInventorySize()){ // inventory full
            std::cout << "Your inventory is full. \n\n";
            break;
        } else{ // this should never happen, something is very wrong if it does
            std::cerr << "Error.\n\n";
            continue;
        }
    }

}


void shop(Player& player){
    player.inventoryDefaultValueSetter();

    std::cout << "Welcome to Roscoe's Potion Emporium! \n";
    std::cout << "Hello, " << player.getName() << ", you have " << player.getGold() << " gold. \n";
    std::cout << "\nHere's our selection today: \n";

    for (auto i: Potion::types){
        std::cout << i << ") " << Potion::potion_name[i] << " costs " << Potion::potion_price[i] << '\n';
    }
    purchase(player);
    std::cout << "\nThanks for shopping at Roscoe's potion emporium! \n";
    std::cout << "Your inventory now has: \n";
    printInventory(player);

}

int main() {

#ifndef DEBUG_MODE
    std::string name {};
    std::cout << "Enter your name:";
    std::getline(std::cin >> std::ws, name);
    Player player { name };
#endif

    // debug feature
#ifdef DEBUG_MODE
    Player player {"debugman"};
#endif
    shop(player);

    return 0;
}
