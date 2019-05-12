#include <iostream>
#include <fstream>
#include <random>
#include <vector>

double getRandomDouble(std::mt19937 &engine) { //generate number from [0,1)
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(engine);
}

int main(int argc, char *argv[])
{
    int number_process;
    int probability_load_task[3];
    int percent_load_task[] = {5, 10, 20};
    int probability_load_processor[4];
    int percent_load_processor[] = {40, 60, 80, 100};
    int Q;
    int probability_exchange_of_data[4];
    int percent_exchange_of_data[] = {0, 10, 50, 100};


    std::ifstream file_in(argv[1], std::ios_base::in); //read data
    if(!file_in.is_open()) {
        return 1;
    }
    file_in >> number_process;
    for(int i = 0; i < 3; i++) {
        file_in >> probability_load_task[i];
    }
    for(int i = 0; i < 4; i++) {
        file_in >> probability_load_processor[i];
    }
    file_in >> Q;
    for(int i = 0; i < 4; i++) {
        file_in >> probability_exchange_of_data[i];
    }
    
    
    for(int q = 1; q <= 10; q++) { //generate 10 pocket of data
        char s[256];
        sprintf(s, "gen_data_%d_%d_%d", number_process, Q, q);
        std::ofstream file_out(s, std::ios_base::out | std::ios_base::trunc);
        
        
        std::random_device device;
        std::mt19937 engine;
        engine.seed(device());
        
        
        file_out << number_process << std::endl;
        int R = 0;
        for(int i = 0; i < number_process; i++) {
            double p = getRandomDouble(engine);
            for(int j = 0; j < 4; j++) {
                p -= probability_load_processor[j] / 100.0;
                if(p <= 0) {
                    R += percent_load_processor[j];
                    file_out << percent_load_processor[j] << ' ';
                    break;
                }
            }
        }
        file_out << std::endl;

        R = (R * Q) / 100;
        int count = 3;
        int tmp = 0;
        std::vector<int> task;
        int probability_load_task_copy[3];
        probability_load_task_copy[0] = probability_load_task[0]; 
        probability_load_task_copy[1] = probability_load_task[1];
        probability_load_task_copy[2] = probability_load_task[2];
        while(count != 0) {
            double p = getRandomDouble(engine);
            for(int j = 0; j < 3; j++) {
                p -= probability_load_task_copy[j] / 100.0;
                if(p <= 0) {
                    if(tmp + probability_load_task_copy[j] < R) {
                        tmp += probability_load_task_copy[j];
                        task.push_back(percent_load_task[j]);
                    }else {
                        int t = probability_load_task_copy[j];
                        probability_load_task_copy[j] = 0; //proxibit generation of j task
                        for(int j1 = 0; j1 < 3; j1++) { 
                            if(probability_load_task_copy[j1] != 0) {
                                probability_load_task_copy[j1] += t / 2;
                                break;
                            }
                        }   
                        for(int j1 = 2; j1 > -1; j1--) {
                            if(probability_load_task_copy[j1] != 0) {
                                probability_load_task_copy[j1] += t / 2;
                                probability_load_task_copy[j1] += t % 2;
                                break;
                            }
                        }
                        count--;                
                    }
                    break;
                }
            }
        }
        file_out << task.size() << std::endl;
        for(auto i : task) {
            file_out << i << ' ';
        }
        file_out << std::endl;
        
        
        std::vector<std::vector<int>> a(task.size(), std::vector<int> (task.size()));
        for(size_t i = 0; i < task.size(); i++) {
            for(size_t j = 0; j < task.size(); j++) {
                if(i == j) {
                    continue;
                }
                double p = getRandomDouble(engine);
                for(int j1 = 0; j1 < 4; j1++) {
                    p -= probability_exchange_of_data[j1] / 100.0;
                    if(p <= 0) {
                        a[i][j] = percent_exchange_of_data[j1];
                        a[j][i] = a[i][j];
                        break;
                    }
                }
            }
        }
        for(size_t i = 0; i < task.size(); i++) {
            for(size_t j = 0; j < task.size(); j++) {
                file_out << a[i][j] << ' ';
            }
            file_out << std::endl;
        }
        
        
        file_out.close();
    }
    return 0;
}
