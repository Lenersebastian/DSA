#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct node {
    int key;
    string value;
};

int hash_f(string value) {
    int output = 0;
    for (int i = 0; i < value.length(); i++) {
        output += value[i];
    }
    return output;
}

void insert(string n_name, node ***table, int table_size, int *places_used) {
    node *n_node = NULL;
    n_node = new node;
    n_node->value = n_name;
    n_node->key = 420;
    int counter=0;
    int position = hash_f(n_name)%table_size;
    while ((*table)[position] != NULL && (*table)[position]->value!="DELETED"){
        counter++;
        position =( hash_f(n_name)+counter)%table_size;
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
            int counter=0;
            position = hash_f((*old_table)[i]->value)%(*table_size);
            while (new_table[position] != NULL){
                counter++;
                position =( hash_f((*old_table)[i]->value)+counter)%(*table_size);
            }
            new_table[position]=(*old_table)[i];
        }
    }
    delete[] *old_table;
    return new_table;
}

void print_f(node **table, int table_size) {
    for (int i = 0; i < table_size; i++) {
        if (table[i] == NULL) {
            cout << "0\n";
        } else {
            cout << table[i]->value << endl;
        }
    }
}

node *search_f(string n_name, node ***table, int table_size) {
    int position= hash_f(n_name)%table_size;
    int counter=0;
    while((*table)[position]!=NULL){
        if ((*table)[position]->value==n_name){
            return (*table)[position];
        } else {
            counter++;
            position= (hash_f(n_name)+counter)%table_size;
        }
    }
    cout << "Neni tam taky";
    return NULL;
}

void delete_f(string n_name, node ***table,int table_size, int *places_used){
    int position= hash_f(n_name);
    int counter=0;
    while((*table)[position]!=NULL){
        if ((*table)[position]->value==n_name){
            cout << "Prvok " <<(*table)[position]->value << " je vymazany\n";
            (*table)[position]->value="DELETED";
            (*table)[position]->key=NULL;
            return;
        }
        counter++;
        position= (hash_f(n_name)+counter)%table_size;
    }
    cout << "Neni tam taky";
    return;
}
int main() {
    string input;
    int table_size = 3;
    int places_used = 0;
    node **table = new node *[table_size];
    for (int i = 0; i < table_size; i++) {
        table[i] = NULL;
    }
    while (1) { //pozri este koniec pola ked je obsadeny, prvok ma hodit na zaciatok
        cout << "insert, search abo delete?\n";
        cin >> input;
        string n_name;
        cout << "Co?\n";
        cin >> n_name;
        if (input == "i") {
            insert(n_name, &table, table_size, &places_used);
        } else if (input == "s") {
            if (search_f(n_name, &table, table_size) != NULL) {
                cout << search_f(n_name, &table, table_size)->value << " ma kluc "
                     << search_f(n_name, &table, table_size)->key << endl;
            }
        } else if (input == "d"){
            delete_f(n_name, &table, table_size, &places_used);
        }
        if (places_used <= 0.2 * table_size || places_used >= 0.75 * table_size) {
            table = resize_table(&table_size, &table,places_used);
        }
        print_f(table, table_size);
    }
}
