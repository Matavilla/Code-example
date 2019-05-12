#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <list>
#include <chrono>

#include "functional.h"

int main(int argc, char *argv[])
{
    auto begin = std::chrono::steady_clock::now(); // start timing

    //read data
    std::ifstream file_in(argv[1], std::ios_base::in);
    if(!file_in.is_open()) {
        return 1;
    }
    size_t n_process;
    file_in >> n_process;
    std::vector<int> process(n_process);
    for(size_t i = 0; i < n_process; i++) {
        file_in >> process[i];
    }
    size_t n_task;
    file_in >> n_task;
    std::vector<int> task(n_task);
    for(size_t i = 0; i < n_task; i++) {
        file_in >> task[i];
    }
    unsigned teor_load = 0; //theoretical maximum network load
    std::vector<std::vector<int>> d_exch(n_task, std::vector<int> (n_task));
    for(size_t i = 0; i < n_task; i++) {
        for(size_t j = 0; j < n_task; j++) {
            file_in >> d_exch[i][j];
            teor_load += d_exch[i][j];
        }
    }
    file_in.close();


    std::cout << "theoretical maximum network load: " << teor_load << "\n\n";
    std::vector<int> ans(n_task);
    unsigned ans_f = 0;
    for(int q = 1; q <= 3; q++) { //perform several runs and save the worst answer
        auto begin1 = std::chrono::steady_clock::now(); // start timing for generate 1 answer
        bool flag_correct = false;
        std::vector<std::list<int>> s(n_process); //task numbers in the processor
        std::vector<int> current_processor_load(n_process);
        while(!flag_correct) {
            for(auto i = s.begin(); i != s.end(); i++) {
                (*i).clear();
            }
            for(size_t i = 0; i < n_task; i++) { //generate answer
                s[getRandomInt(n_process - 1)].push_back(i);
            }
            //check answer
            flag_correct = check_answer(s, process, task, current_processor_load);
        }

        //try to improve answer
        unsigned f = compute_objective_function(teor_load, s, d_exch, n_task); //objective function
        while(true) {
            std::vector<std::list<int>> s_min;
            unsigned f_min = f;
            for(size_t i = 0; i < n_process; i++) {
                for(size_t j = 0; j < n_process; j++) {
                    if(i == j) {
                        continue;
                    }
                    //operation A
                    int index = 0;
                    for(auto i1 = s[i].begin(); i1 != s[i].end(); i1++) {
                        if(current_processor_load[j] + task[*i1] < process[j]) {
                            std::vector<std::list<int>> s_tmp(s);
                            auto l = s_tmp[i].begin();
                            std::advance(l, index);
                            s_tmp[j].push_back(*l);
                            s_tmp[i].erase(l);
                            unsigned t = compute_objective_function(teor_load, s_tmp, d_exch, n_task);
                            if(f_min > t) {
                                std::swap(f_min, t);
                                std::swap(s_tmp, s_min);
                            }
                        }
                        index++;
                    }
                    //operation B and C
                    index = 0;
                    for(auto i1 = s[i].begin(); i1 != s[i].end(); i1++) {
                        int end = 4;
                        if(task[*i1] == 10) {
                            end = 2;
                        }else if (task[*i1] == 5) {
                            end = 1;
                        }
                        for(int m = 1; m <= end; m++) {
                            std::vector<int> comb(m);
                            for(size_t r = 0; r < comb.size(); r++) {
                                comb[r] = r + 1;
                            }
                            while(next_combination(comb, s[j].size(), m)) {
                                int sum = 0;
                                for(size_t r = 0; r < comb.size(); r++) {
                                    auto i2 = s[j].begin();
                                    std::advance(i2, comb[r] - 1);
                                    sum += task[*i2];
                                }
                                if (sum != task[*i1]) {
                                    continue;
                                }
                                std::vector<std::list<int>> s_tmp(s);
                                for(int r = comb.size() - 1; r >= 0; r--) {
                                    auto i2 = s_tmp[j].begin();
                                    std::advance(i2, comb[r] - 1);
                                    s_tmp[i].push_back(*i2);
                                    s_tmp[j].erase(i2);
                                }
                                auto i2 = s_tmp[i].begin();
                                std::advance(i2, index);
                                s_tmp[j].push_back(*i2);
                                s_tmp[i].erase(i2);
                                unsigned t = compute_objective_function(teor_load, s_tmp, d_exch, n_task);
                                if(f_min > t) {
                                    std::swap(f_min, t);
                                    std::swap(s_tmp, s_min);
                                }
                            }
                        }
                        index++;
                    }
                }
            }
            if(f_min == f) {
                break;
            } else {
                std::swap(s, s_min);
                f = f_min;
                check_answer(s, process, task, current_processor_load); // compute current_processor_load
            }
        }


        std::vector<int> ans1(n_task);
        build_answer(s, ans1);
        std::cout << "network load" << q << ": " << f << '\n';
        std::cout << "Answer" << q << ":\n";
        for(auto i : ans1) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
        auto end1 = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
        std::cout << "time" << q << ": " << elapsed_ms.count() << " ms\n";
        std::cout << "\n";
        if(f > ans_f) {
            std::swap(ans_f, f);
            std::swap(ans1, ans);
        }
    }
    
    
    std::cout << "network load: " << ans_f << '\n';
    std::cout << "the worst answer: ";
    for(auto i : ans) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Total time: " << elapsed_ms.count() << " ms\n";
    return 0;
}
