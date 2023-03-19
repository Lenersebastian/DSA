#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <fstream>

using namespace std::chrono;
using namespace std;

struct node
{
    int key;
    string value;
    node *next;
};

int hash_f(string value)
{
    int output = 37;
    for (int i = 0; i < value.length(); i++)
    {
        output = output * 54059 ^ value[i] * 76963; // prime numbers
    }
    if (output < 0)
    {
        output *= -1;
    }
    return output;
}

void insert(string n_name, node ***table, int table_size, int *places_used, int key)
{
    node *n_node = NULL;
    n_node = new node;
    n_node->value = n_name;
    n_node->key = key;
    n_node->next = NULL;
    int position = hash_f(n_node->value) % table_size;
    if ((*table)[position] == NULL)
    {
        (*table)[position] = n_node;
        (*places_used)++;
    }
    else
    {
        node *akt_node = (*table)[position];
        while (akt_node->next != NULL)
        {
            akt_node = akt_node->next;
        }
        akt_node->next = n_node;
        (*places_used)++;
    }
}

node **resize_table(int *table_size, node ***old_table, int places_used)
{
    int old_table_size = *table_size;
    int position;
    int hash;
    if (places_used >= 0.75 * old_table_size)
    {
        *table_size = ((*table_size) << 1);
    }
    else
    {
        *table_size = ((*table_size) >> 1);
    }
    node **new_table = new node *[*table_size];
    for (int i = 0; i < *table_size; i++)
    {
        new_table[i] = NULL;
    }
    for (int i = 0; i < old_table_size; i++)
    {
        if ((*old_table)[i] != NULL)
        {
            node *akt_node = (*old_table)[i];
            while (akt_node != NULL)
            {
                insert(akt_node->value, &new_table, *table_size, &places_used, akt_node->key);
                akt_node = akt_node->next;
            }
        }
    }
    delete[] * old_table;
    return new_table;
}

node *search_f(string n_name, node ***table, int table_size)
{
    int position = hash_f(n_name) % table_size;
    node *akt_node = (*table)[position];
    while (akt_node != NULL)
    {
        if (akt_node->value == n_name)
        {
            return akt_node;
        }
        akt_node = akt_node->next;
    }
    cout << "Prvok sa v tabulke nenachadza\n";
    return NULL;
}

void delete_f(string n_name, node ***table, int table_size, int *places_used)
{
    int position = (hash_f(n_name)) % table_size;
    if ((*table)[position]->value == n_name)
    {
        (*table)[position] = (*table)[position]->next;
        (*places_used)--;
        return;
    }
    node *akt_node = (*table)[position];
    node *before_node;
    while (akt_node != NULL)
    {
        if (akt_node->value == n_name)
        {
            before_node->next = akt_node->next;
            delete akt_node;
            (*places_used)--;
            return;
        }
        before_node = akt_node;
        akt_node = akt_node->next;
    }
    cout << "Prvok sa v tabulke nenachadzal\n";
}

int main()
{
    int table_size = 1000000;
    int key;
    string n_name;
    int places_used = 0;
    node **table = new node *[table_size];
    for (int i = 0; i < table_size; i++)
    {
        table[i] = NULL;
    }

    ifstream cisel_1000000("random_numbers_1000000.txt");
    ifstream strings_1000000("random_strings_1000000.txt");
    auto zaciatok = high_resolution_clock ::now();

    for (int i = 0; i < 1000000; i++)
    {
        cisel_1000000 >> key;
        strings_1000000 >> n_name;
        insert(n_name, &table, table_size, &places_used, key);
        if (places_used >= 0.75 * table_size)
        {
            table = resize_table(&table_size, &table, places_used);
        }
    }
    // auto koniec = high_resolution_clock ::now();
    // auto dlzka = duration_cast<milliseconds>(koniec - zaciatok).count();
    // cout << "Insert trval: " << dlzka << " milisekund." << endl;

    cisel_1000000.clear();
    cisel_1000000.seekg(0);
    strings_1000000.clear();
    strings_1000000.seekg(0);

    // zaciatok = high_resolution_clock ::now();
    for (int i = 0; i < 1000000; i++)
    {
        cisel_1000000 >> key;
        strings_1000000 >> n_name;
        search_f(n_name, &table, table_size);
    }
    // koniec = high_resolution_clock ::now();
    // dlzka = duration_cast<seconds>(koniec - zaciatok).count();
    // cout << "Search trval: " << dlzka << " sekund." << endl;

    cisel_1000000.clear();
    cisel_1000000.seekg(0);
    strings_1000000.clear();
    strings_1000000.seekg(0);

    // zaciatok = high_resolution_clock ::now();
    for (int i = 0; i < 1000000; i++)
    {
        cisel_1000000 >> key;
        strings_1000000 >> n_name;
        delete_f(n_name, &table, table_size, &places_used);
        if (places_used <= 0.15 * table_size && table_size>2)
        {
            table = resize_table(&table_size, &table, places_used);
        }
    }
    auto koniec = high_resolution_clock ::now();
    auto dlzka = duration_cast<milliseconds>(koniec - zaciatok).count();
    cout << "Delete trval: " << dlzka << " milisekund." << endl;
    return 0;
}
