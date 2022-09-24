#include "HashTableClosed.h"
#include <iostream>
#include <string.h>
//#include <stdint.h>


HashTableClosed::HashTableClosed() {
    count = 0;
    mem_size = 8;
    array = new Pair[mem_size];
}
HashTableClosed::~HashTableClosed() {
    delete[] array;
}
void HashTableClosed::add(Login login, uint* pass_sha1_hash) {
    int index = -1, i = 0;
    int freeCount = 0;
    for (; i < mem_size; i++) {
        index = hash_func(login, i);
        if (array[index].status == enPairStatus::free) {
            freeCount++;
        }
    }

    if (freeCount == 0)
        resize();

    // берем пробы по всем i от 0 до размера массива
    index = -1, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(login, i);
        if (array[index].status == enPairStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }
    if (i >= mem_size) return; // все перебрали, нет места

    // кладем в свободную ячейку пару
    array[index] = Pair(login, pass_sha1_hash);
    count++;

}
int HashTableClosed::hash_func(Login login, int offset) {

    int sum = 0, i = 0;
    for (; i < LOGLENGTH; i++) {
        sum += login[i];
    }

    return (sum % mem_size + offset * offset) % mem_size;
}

void HashTableClosed::del(Login login) {

    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(login, i);
        if (array[index].status == enPairStatus::engaged &&
            !strcmp(array[index].login, login)) {
            array[index].status = enPairStatus::deleted;
            count--;
            return;
        }
        else if (array[index].status == enPairStatus::free) {
            return;
        }
    }
}
uint* HashTableClosed::find(Login login) {

    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(login, i);
        if (strcmp(array[index].login, login) == 0) {
            return array[index].pass_sha1_hash;
        }
    }

    return nullptr;
}

void HashTableClosed::resize() {

    Pair* save_ct = array; // запоминаем старый массив
    int oldSize = mem_size;

    mem_size *= 2;  // увеличиваем размер в два раза
    count = 0; // обнуляем количество элементов
    array = new Pair[mem_size]; // выделяем новую память

    for (int i = 0; i < oldSize; i++) {
        Pair curr = save_ct[i];
        add(curr.login, curr.pass_sha1_hash);
    }

    delete[] save_ct;
}

bool HashTableClosed::checkLogin(Login login)
{
    bool success = false;
    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(login, i);
        if (strcmp(array[index].login, login) == 0)
        {
            success = true;
            break;
        }
    }
    return success;
}