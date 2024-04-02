#include "database.h"
#include <iostream>

std::string enum_name(Type type)
{
    std::string typeStr;
    switch (type)
    {
        case INT:
            typeStr = "INT";
            break;
        case DOUBLE:
            typeStr = "DOUBLE";
            break;
        case STRING:
            typeStr = "STRING";
            break;
        case ARRAY:
            typeStr = "ARRAY";
            break;
    }
    return typeStr;
}

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value)
{
    Entry *newEntry = new Entry;
    newEntry->type = type;
    newEntry->key = key;
    switch (type)
    {
        case INT:
            newEntry->value = new int(*(int*)(value));
            break;
        case DOUBLE:
            newEntry->value = new double(*(double*)(value));
            break;
        case STRING:
            newEntry->value = new std::string(*(std::string*)(value));
            break;
        case ARRAY:
            newEntry->value = new std::string(*(std::string*)(value));
            break;
    }
    return newEntry;
}

// 데이터베이스를 초기화한다.
void init(Database &database)
{
    database.d_size = 1;
    database.d_used = 0;
    database.db = new Entry*[database.d_size];
}

void list(Database &database)
{
    for(int i = 0; i < database.d_used; i++)
    {
        std::string s = enum_name(database.db[i]->type);
        if(s == "INT")
        {
            std::cout << database.db[i]->key << ": " << *((int*)database.db[i]->value);
        }
        else if(s == "DOUBLE")
        {
            std::cout << database.db[i]->key << ": " << *((double*)database.db[i]->value);
        }
        else if(s == "STRING")
        {
            std::cout << database.db[i]->key << ": " << *((std::string*)database.db[i]->value);
        }
        else if(s == "ARRAY")
        {
            std::cout << database.db[i]->key << ": " << *((std::string*)database.db[i]->value);
        }
        std::cout << std::endl;
    }
}

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry)
{
    Entry *isExist = get(database, entry->key);
    if(isExist != nullptr)
    {
        isExist->type = entry->type;
        isExist->value = entry->value;
        return;
    }

    if(database.d_used == database.d_size)
    {
        database.d_size *= 2;
        Entry ** newDb = new Entry *[database.d_size];
        for (int i = 0; i < database.d_used; i++)
        {
            newDb[i] = database.db[i];
        }
        delete[] database.db;
        database.db = newDb;
    }
    database.db[database.d_used++] = entry;
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry *get(Database &database, std::string &key)
{
    for(int i = 0; i < database.d_used; i++)
    {
        if(database.db[i]->key == key)
        {
            return database.db[i];
        }
    }
    return nullptr;
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key)
{
    int index = -1;
    for (int i = 0; i < database.d_used; i++)
    {
        if (database.db[i]->key == key)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        std::cout << "Not found\n";
        return;
    }
    if(database.db[index]->value == "STRING" || database.db[index]->value == "ARRAY")
    {
        delete[] database.db[index];
    }
    else delete database.db[index];

    for (int i = index; i < database.d_used; i++)
    {
        database.db[i] = database.db[i + 1];
    }
    database.d_used--;
}

// 데이터베이스를 해제한다.
void destroy(Database &database)
{
    for(int i = 0; i < database.d_used; i++)
    {
    if(database.db[i]->value == "STRING")
    {
        delete[] database.db[i];
    }
    else delete database.db[i];
    }
}

void makeArray(std::string value, std::string& array)
{
    if (value != "ARRAY" && value != "array")
    {
        std::cout << "size: ";
        int size;
        std::cin >> size;
        array = "[";
        for (int i = 0; i < size; i++)
        {
            std::cout << "item[" << i << "]: ";
            std::string temp;
            std::cin >> temp;
            if (value == "STRING" || value == "string")
            {
                array += "\"" + temp + "\"";
            }
            else
            {
                array += temp;
            }
            if (i != size - 1)
            {
                array += ", ";
            }
        }
        array += "]";
    }
    else
    {
        std::cout << "size: ";
        int size;
        std::cin >> size;
        array = "[";
        for (int i = 0; i < size; i++)
        {
            std::cout << "item[" << i << "]: type (int, double, string, array): ";
            std::string t;
            std::cin >> t;
            std::string nested;
            makeArray(t, nested);
            array += nested;
            if (i != size - 1)
            {
                array += ", ";
            }
        }
        array += "]";
    }
}

int main()
{
    Database myDatabase;
    init(myDatabase);
    Type t;
    std::string key;
    std::string typeStr;
    std::string command = "";

    while(true)
    {
        std::cout << "command (list, add, get, del, exit): ";
        std::cin >> command;
        if(command == "list")
        {
            list(myDatabase);
        }
        else if(command == "add")
        {
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "type (int, double, string, array): ";
            std::cin >> typeStr;

            if(typeStr == "INT" || typeStr == "int")
            {
                t = INT;
                std::cout << "value: ";
                int num;
                std::cin >> num;
                Entry* entry = create(t, key, &num);
                add(myDatabase, entry);
            }
            else if(typeStr == "DOUBLE" || typeStr == "double")
            {
                t = DOUBLE;
                std::cout << "value: ";
                double num;
                std::cin >> num;
                Entry* entry = create(t, key, &num);
                add(myDatabase, entry);
            }
            else if(typeStr == "STRING" || typeStr == "string")
            {
                t = STRING;
                std::cout << "value: ";
                std::string str;
                std::cin >> str;
                Entry* entry = create(t, key, &str);
                add(myDatabase, entry);
            }
            else if(typeStr == "array" || typeStr == "ARRAY")
            {
                t = ARRAY;
                std::cout << "value (int, double, string, array): ";
                std::string value;
                std::cin >> value;
                std::string array = "[";
                makeArray(value, array);
                Entry* entry = create(t, key, &array);
                add(myDatabase, entry);
            }
            else
            {
                std::cout << "invalid type\n";
            }
        }
        else if(command == "get")
        {
            std::cout << "key: ";
            std::cin >> key;
            Entry* component = get(myDatabase, key);
            if(component == nullptr)
            {
                std::cout << "not found\n";
                continue;
            }
            std::string s = enum_name(component->type);
            if(s == "INT")
            {
                std::cout << component->key << ": " << *((int*)component->value);
            }
            else if(s == "DOUBLE")
            {
                std::cout << component->key << ": " << *((double*)component->value);
            }
            else if(s == "STRING")
            {
                std::cout << component->key << ": " << *((std::string*)component->value);
            }
            else if(s == "ARRAY")
            {
                std::cout << component->key << ": " << *((std::string*)component->value);
            }
            std::cout << std::endl;
        }
        else if(command == "del")
        {
            std::cout << "key: ";
            std::cin >> key;
            remove(myDatabase, key);
        }
        else if(command == "exit")
        {
            destroy(myDatabase);
            break;
        }
        else
        {
            std::cout << "invalid command\n";
        }
    }
}