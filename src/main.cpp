// Copyright 2022 NNTU-CS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <random>
#include <vector>
#include "tree.h"

int main() {
    std::vector<char> init = { '1', '2', '3' };
    PMTree example(init);

    auto variants = getAllPerms(example);
    for (auto& seq : variants) {
        for (char c : seq) std::cout << c;
        std::cout << "  ";
    }
    std::cout << "\n\n";

    std::cout << "getPerm1(1): ";
    auto first = getPerm1(example, 1);
    for (char c : first) std::cout << c;
    std::cout << std::endl;

    std::cout << "getPerm2(2): ";
    auto second = getPerm2(example, 2);
    for (char c : second) std::cout << c;
    std::cout << "\n\n";

    std::cout << "n;getAllPerms(s);getPerm1(s);getPerm2(s)" << std::endl;

    for (int n = 3; n <= 8; ++n) {
        std::vector<char> letters;
        for (int i = 0; i < n; ++i) letters.push_back('a' + i);
        PMTree tree(letters);

        std::mt19937 gen(42);
        std::uniform_int_distribution<> dist(1, static_cast<int>(fact(n)));
        int target = dist(gen);

        auto t0 = std::chrono::high_resolution_clock::now();
        getAllPerms(tree);
        auto t1 = std::chrono::high_resolution_clock::now();
        getPerm1(tree, target);
        auto t2 = std::chrono::high_resolution_clock::now();
        getPerm2(tree, target);
        auto t3 = std::chrono::high_resolution_clock::now();

        double d1 = std::chrono::duration<double>(t1 - t0).count();
        double d2 = std::chrono::duration<double>(t2 - t1).count();
        double d3 = std::chrono::duration<double>(t3 - t2).count();

        std::cout << n << ";"
            << std::fixed << std::setprecision(6) << d1 << ";"
            << d2 << ";"
            << d3 << std::endl;
    }

    return 0;
}
