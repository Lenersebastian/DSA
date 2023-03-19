#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>

using namespace std::chrono;
using namespace std;

struct node {
    int key;
    string value;
};

int hash_f(string value) {
    int output = 37;
    for (int i = 0; i < value.length(); i++) {
        output = output*54059 ^ value[i]*76963; //prime numbers
    }
    if(output<0){
        output*=-1;
    }
    return output;
}

void insert(string n_name, node ***table, int table_size, int *places_used, int key) {
    node *n_node = NULL;
    n_node = new node;
    n_node->value = n_name;
    n_node->key = key;
    int counter=0;
    int hash=hash_f(n_name);
    int position = hash%table_size;
    while (((*table)[position] != NULL) && ((*table)[position]->value!="DELETED")){
        counter++;
        position =( hash+(counter*counter))%table_size;
    }
    (*table)[position]=n_node;
    (*places_used)++;
}

node **resize_table(int *table_size, node ***old_table, int places_used) {
    int old_table_size = *table_size;
    int position;
    if (places_used >= 0.75*old_table_size){
        *table_size = ((*table_size) << 1);
    } else {
        *table_size = ((*table_size) >> 1);
    }
    node **new_table = new node *[*table_size];
    for (int i = 0; i < *table_size; i++) {
        new_table[i] = NULL;
    }
    for (int i = 0; i < old_table_size; i++) {
        if ((*old_table)[i] != NULL) {
            if ((*old_table)[i]->value != "DELETED"){
                int hash=hash_f((*old_table)[i]->value);
                int counter=0;
                position = hash%(*table_size);
                while (new_table[position] != NULL){
                    counter++;
                    position =(hash+(counter*counter))%(*table_size);
                }
                new_table[position]=(*old_table)[i];
            }
        }
    }
    delete[] *old_table;
    return new_table;
}

node *search_f(string n_name, node ***table, int table_size) {
    int hash = hash_f(n_name);
    int position= hash%table_size;
    int counter=0;
    while((*table)[position]!=NULL){
        if ((*table)[position]->value==n_name){
            return (*table)[position];
        } else {
            counter++;
            position= (hash+(counter*counter))%table_size;
        }
    }
    cout << "Prvok sa v tabulke nenachadza\n";
    return NULL;
}

void delete_f(string n_name, node ***table,int table_size, int *places_used){ 
    int hash=hash_f(n_name);
    int position = hash%table_size;
    int counter=0;
    while((*table)[position]!=NULL){
        if ((*table)[position]->value==n_name){
            // cout << "Prvok " <<(*table)[position]->value << " je vymazany\n";
            (*table)[position]->value="DELETED";
            (*table)[position]->key=0;
            (*places_used)--;
            return;
        }
        counter++;
        position= (hash+(counter*counter))%table_size;
    }
    cout << "Prvok sa v tabulke nenachadzal\n";
    return;
}

int main() {
    int table_size = 1000000;
    int key;
    string n_name;
    int places_used = 0;
    node **table = new node *[table_size];
    for (int i = 0; i < table_size; i++)
    {
        table[i] = NULL;
    }

    ifstream cisel_10000000("random_numbers_1000000.txt");
    ifstream strings_10000000("random_strings_1000000.txt");
    auto zaciatok = high_resolution_clock ::now();

    for (int i = 0; i < 1000000; i++)
    {
        cisel_10000000 >> key;
        strings_10000000 >> n_name;
        insert(n_name, &table, table_size, &places_used, key);
        if (places_used >= 0.75 * table_size)
        {
            table = resize_table(&table_size, &table, places_used);
        }
    }
    // auto koniec = high_resolution_clock ::now();
    // auto dlzka = duration_cast<seconds>(koniec - zaciatok).count();
    // cout << "Insert trval: " << dlzka << " sekund." << endl;

    cisel_10000000.clear();
    cisel_10000000.seekg(0);
    strings_10000000.clear();
    strings_10000000.seekg(0);

    // zaciatok=high_resolution_clock ::now();
    for (int i = 0; i < 1000000; i++)
    {
        cisel_10000000 >> key;
        strings_10000000 >> n_name;
        search_f(n_name, &table, table_size);
    }
    // koniec=high_resolution_clock ::now();
    // dlzka = duration_cast<seconds>(koniec - zaciatok).count();
    // cout << "Search trval: "<< dlzka << " sekund." << endl;

    cisel_10000000.clear();
    cisel_10000000.seekg(0);
    strings_10000000.clear();
    strings_10000000.seekg(0);

    // zaciatok = high_resolution_clock ::now();
    for (int i = 0; i < 1000000; i++)
    {
        cisel_10000000 >> key;
        strings_10000000 >> n_name;
        delete_f(n_name, &table, table_size, &places_used);
        if (places_used <= 0.2 * table_size && table_size>2)
        {
            table = resize_table(&table_size, &table, places_used);
        }
    }
    auto koniec = high_resolution_clock ::now();
    auto dlzka = duration_cast<milliseconds>(koniec - zaciatok).count();
    cout << "Delete trval: " << dlzka << " millisekund." << endl;
    return 0;

}   
