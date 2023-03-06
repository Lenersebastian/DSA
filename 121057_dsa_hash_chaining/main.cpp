#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct node {
    int key;
    string value;
    node *next;
};

int hash_f(string value,int table_size) {
    int output = 0;
    for (int i = 0; i < value.length(); i++) {
        output += value[i];
    }
    output=output%table_size;
    return output;
}

void insert(string n_name, node ***table, int table_size, int *places_used) {
    node *n_node = NULL;
    n_node = new node;
    n_node->value = n_name;
    n_node->key = 420;
    n_node->next = NULL;
    int position = hash_f(n_node->value,table_size);
    if ((*table)[position] == NULL) {
        (*table)[position] = n_node;
        (*places_used)++;
    } else {
        node *akt_node = (*table)[position];
        while (akt_node->next != NULL) {
            if (akt_node->next->value == n_node->value) {
                cout << "Taky prvok tam uz je\n";
                return;
            }
            akt_node = akt_node->next;
        }
        akt_node->next = n_node;
    }
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
            position = hash_f((*old_table)[i]->value,*table_size);
            new_table[position] = (*old_table)[i];
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
            node *akt_node = table[i];
            while (akt_node->next != NULL) {
                cout << akt_node->value << ", ";
                akt_node = akt_node->next;
            }
            cout << akt_node->value << endl;
        }
    }
}

node *search_f(string n_name, node ***table, int table_size) {
    for (int i = 0; i < table_size; i++) {
        node *akt_node = (*table)[i];
        if (akt_node != NULL) {
            while (akt_node != NULL) {
                if (akt_node->value == n_name) {
                    return akt_node;
                }
                akt_node = akt_node->next;
            }
        }
    }
    cout << "Take tam neni\n";
    return NULL;
}

void delete_f(string n_name, node ***table,int table_size, int *places_used){
    for (int i = 0; i < table_size; i++) {
        node *akt_node = (*table)[i];
        if (akt_node != NULL) {
            if (akt_node->value == n_name){
                node *next_node=akt_node->next;
                if (next_node==NULL){
                    (*places_used)--;
                }
                cout << "Prvok " << (*table)[i]->value << " sa vymazal\n";
                delete (*table)[i];
                (*table)[i]=next_node;
                return;
            }
            while (akt_node->next!=NULL){
                if (akt_node->next->value == n_name){
                    cout << "Prvok " << akt_node->next->value << " sa vymazal\n";
                    node *next_node = akt_node->next->next;
                    delete akt_node->next;
                    akt_node->next=next_node;
                    return;
                }
                akt_node=akt_node->next;
            }
        }
    }
    cout << "Take tam neni\n";
}
int main() {
    string input;
    int table_size = 3;
    int places_used = 0;
    node **table = new node *[table_size];
    for (int i = 0; i < table_size; i++) {
        table[i] = NULL;
    }
    while (1) {
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