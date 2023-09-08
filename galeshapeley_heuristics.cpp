#improved gale shapeley using heuristics
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::unordered_map<std::string, std::vector<std::string>> men_preferences;
std::unordered_map<std::string, std::vector<std::string>> women_preferences;

std::unordered_map<std::string, std::string> gale_shapley_with_imbalance() {
    std::vector<std::string> men;
    std::vector<std::string> women;
    std::unordered_map<std::string, std::string> women_partner;
    std::vector<std::string> men_free;

    // Initialize data structures
    for (const auto& pair : men_preferences)
        men.push_back(pair.first);
    for (const auto& pair : women_preferences)
        women.push_back(pair.first);
    men_free = men;

    // Introduce imbalance handling
    // Sort women's preference lists based on the length of the men's proposal history
    for (auto& pair : women_preferences)
        std::sort(pair.second.begin(), pair.second.end(),
                  [&](const std::string& m1, const std::string& m2) {
                      return std::find(men_preferences[m1].begin(), men_preferences[m1].end(), pair.first) < std::find(men_preferences[m2].begin(), men_preferences[m2].end(), pair.first);
                  });

    while (!men_free.empty()) {
        std::string man = men_free.front();
        men_free.erase(men_free.begin());
        std::vector<std::string>& preferences = men_preferences[man];

        for (const std::string& woman : preferences) {
            std::string current_partner = women_partner[woman];
            if (current_partner.empty()) {
                // Woman is free, they become partners
                women_partner[woman] = man;
                break;
            } else {
                // Woman is already engaged, check if she prefers the new man
                const std::vector<std::string>& woman_preferences = women_preferences[woman];
                auto it_man = std::find(woman_preferences.begin(), woman_preferences.end(), man);
                auto it_current_partner = std::find(woman_preferences.begin(), woman_preferences.end(), current_partner);
                if (it_man < it_current_partner) {
                    // Woman prefers the new man, update partners
                    women_partner[woman] = man;
                    men_free.push_back(current_partner);
                    break;
                }
            }
        }
    }

    return women_partner;
}

int main() {
    // Example usage
    men_preferences = {
        {"John", {"Mary", "Jane", "Alice"}},
        {"Mike", {"Jane", "Alice", "Mary"}},
        {"Tom", {"Mary", "Alice", "Jane"}}
    };

    women_preferences = {
        {"Mary", {"Tom", "John"}},
        {"Jane", {"Mike", "John", "Tom"}},
        {"Alice", {"Tom", "Mike", "John"}}
    };

    std::unordered_map<std::string, std::string> result = gale_shapley_with_imbalance();
    for (const auto& pair : result)
        std::cout << pair.second << " is paired with " << pair.first << std::endl;

    return 0;
}
