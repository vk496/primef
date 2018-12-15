/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vk496
 *
 * Created on 10 de diciembre de 2018, 21:07
 */

#include <iostream>
#include <gmp.h>
#include <fstream>
#include <vector>
#include <mutex>
#include <thread>
#include <unistd.h>

using namespace std;


mutex _mutex, mtx_print, mtx_final;
bool cont = true, printres = false;
mpz_t p;
int i = 0;

void nextNum(mpz_t a) {
    unique_lock<mutex> lock(_mutex);
    //    mpz_add_ui(p, p, ((i == 1) ? 4 : 2)); //Avoid mul2 and mul5
    mpz_add_ui(p, p, 1);

    //    if (i == 4) {
    //        i = 0;
    //    }

    mpz_inits(a, NULL);

    mpz_set(a, p);
}

void myPrint(mpz_t extra) {
    mtx_print.lock();
    if (cont) {
        mpz_out_str(stdout, 16, extra);
        cout << endl;
    }
    mtx_print.unlock();
}

void *PrintHello(string d) {

    mpz_t extra;
    mpz_inits(extra, NULL);

    mpz_set_ui(extra, 1);

    string fnum;

    mpz_t n;
    mpz_inits(n, NULL);


    while (cont) {


        //get extra_byte in hex
        unsigned long int z = mpz_sizeinbase(extra, 16) + 2;
        char *cstr = new char[z];
        mpz_get_str(cstr, 16, extra);

        fnum.assign(d);
        fnum.append(cstr);

        delete cstr;

        mpz_set_str(n, fnum.c_str(), 16);
        myPrint(extra);

        if (mpz_probab_prime_p(n, 0) != 0) {

            mtx_print.lock(); //lock all other threads
            if (mpz_probab_prime_p(n, 45) != 0) {
                cont = false;
            }
            mtx_print.unlock();

        }

        nextNum(extra);

    }

    mtx_final.lock();
    if (!printres) {
        cout << "Bingo (hex): ";
        mpz_out_str(stdout, 16, extra);
        cout << endl;
        printres = true;
    }
    mtx_final.unlock();

    pthread_exit(NULL);
}

int main(int argc, char** argv) {


    const string inputFile = "myfile.txt";
    vector<char> buffer;

    ifstream infile(inputFile);
    
    if(!infile.good()) {
        cout << "Problems with input file: " << inputFile << endl;
        return 1;
    }

    //get length of file
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);

    //read file
    if (length > 0) {
        buffer.resize(length);
        infile.read(&buffer[0], length);
    }

    char* pBuffer = new char[buffer.size()*2];

    for (unsigned int i = 0; i < buffer.size(); ++i) {
        sprintf(&pBuffer[i * 2], "%02x", (unsigned char) buffer[i]);
    }

    string j(pBuffer, buffer.size()*2);

    mpz_inits(p, NULL);

    mpz_set_ui(p, 1);

    std::vector<std::thread> threads;

    unsigned int NUM_THREADS = std::thread::hardware_concurrency();

    if (NUM_THREADS == 0) {
        NUM_THREADS = 1;
    }

    cout << "Start with " << NUM_THREADS << " threads" << endl;
    sleep(4);

    for (unsigned int i = 0; i < NUM_THREADS; i++)
        threads.push_back(std::thread(PrintHello, j));



    delete pBuffer;

    for (auto& th : threads)
        th.join();



    return 0;

}

