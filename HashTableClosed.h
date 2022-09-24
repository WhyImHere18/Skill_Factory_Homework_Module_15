#pragma once

#include <string.h>
#include "sha1.h"

#define LOGLENGTH 10  // Зафиксируем длину логина

typedef char Login[LOGLENGTH]; // тип логин

class HashTableClosed { // хеш-таблица

public:

    HashTableClosed();
    ~HashTableClosed();
    void add(Login login, uint* pass_sha1_hash);
    void del(Login login);
    uint* find(Login login);
    bool checkLogin(Login login);

private:

    enum enPairStatus {
        free, // свободен
        engaged, //занят
        deleted //удален
    };

    struct Pair { // пара ключ-значение

        Pair() : // конструктор по умолчанию
            login(""),
            pass_sha1_hash(nullptr),
            status(enPairStatus::free) {

        }
        Pair(Login _login, uint* _pass_sha1_hash)
        {
            status = enPairStatus::engaged;
            strcpy(login, _login);
            pass_sha1_hash = _pass_sha1_hash;

        }
        Pair(const Pair& other)
        {
            strcpy(login, other.login);
            pass_sha1_hash = other.pass_sha1_hash;
        }
        Pair& operator = (const Pair& other) { //  оператор присваивания
            strcpy(login, other.login);
            status = other.status;

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            return *this;
        }

        uint* pass_sha1_hash;
        Login login;
        enPairStatus status; // состояние ячейки
    };

    int hash_func(Login login, int offset);
    void resize();


    Pair* array;
    int mem_size;
    int count;

};