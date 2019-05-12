#include <iostream>
#include <vector>
#include <list>
#include <random>

int getRandomInt(int b) { //generate number from [0,b)
    std::random_device device;
    std::mt19937 engine;
    engine.seed(device());
    std::uniform_int_distribution<int> distribution(0, b);
    return distribution(engine);
}

unsigned compute_objective_function(unsigned teor_load, std::vector<std::list<int>> &s, std::vector<std::vector<int>> &d_exch, size_t n_task) {
    for(size_t i = 0; i < s.size(); i++) {
        for(auto j : s[i]) {
            for(auto j1 : s[i]) {
                teor_load -= d_exch[j][j1];
            }
        }
    }
    return teor_load;
}

bool check_answer(std::vector<std::list<int>> &s, std::vector<int> &process, std::vector<int> &task, std::vector<int> &current_processor_load) {
    for(size_t i = 0; i < s.size(); i++) {
        current_processor_load[i] = 0;
        for(auto j : s[i]) {
            current_processor_load[i] += task[j];
        }
        if(current_processor_load[i] > process[i]) {
            return false;
        }
    }
    return true;
}

void build_answer(std::vector<std::list<int>> &s, std::vector<int> &ans1) {
    for(size_t i = 0; i < s.size(); i++) {
        for(auto &j : s[i]) {
            ans1[j] = i + 1;
        }
    }
}

bool next_combination(std::vector<int> &a, int n, int m)
{
    int k = m;
    for (int i = k - 1; i >= 0; --i) {
        if (a[i] < n - k + i + 1) {
            ++a[i];
            for (int j = i + 1; j < k; ++j)
                a[j] = a[j - 1] + 1;
            return true;
        }
    }
    return false;
}
