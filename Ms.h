#include <iostream>
#include <string.h>
#include <vector>
#include <complex>
#include <cmath>
#include <thread>
#include "CImg.h"

#ifdef __linux__
#include <unistd.h>

#define func_sleep usleep

#define iteration 800
#define DEFAULT "\033[m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"

#else
#include <Windows.h>

#define func_sleep Sleep

#define iteration 800
#define DEFAULT "\0"
#define GREEN "\0"
#define RED "\0"
#define BLUE "\0"
#define YELLOW "\0"

#endif


bool cacu(double r,double i);
void get_informations();
void auto_cacu();
void cacu_line();
bool call_next_task(double& wpp,double& i);
bool is_task_finish();
void progress_bar();