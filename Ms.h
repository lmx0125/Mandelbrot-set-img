#include <iostream>
#include <string.h>
#include <vector>
#include <complex>
#include <cmath>
#include <thread>
#include <iomanip>
#include "CImg.h"

#ifdef __linux__
#include <unistd.h>
#define func_sleep usleep
#define DEFAULT "\033[m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#else
#include <Windows.h>
#define func_sleep Sleep
#define DEFAULT "\0"
#define GREEN "\0"
#define RED "\0"
#define BLUE "\0"
#define YELLOW "\0"
#endif

#define iteration 1000
#define start_x_pos_tag "-sx"
#define start_y_pos_tag "-sy"
#define end_x_pos_tag "-ex"
#define end_y_pos_tag "-ey"
#define help_tag "-help"
#define iteration_tag "-i"
#define wp_tag "-wp"

struct pos {
	double x;
	double y;
};

struct C {
	double r;
	double i;
};

struct argv_status_struc {
	bool start_x;
	bool start_y;
	bool end_x;
	bool end_y;
	bool wp;
	bool is_using_argv;
	double start_x_pos;
	double start_y_pos;
	double end_x_pos;
	double end_y_pos;
	double width;
	double height;
};

struct img_status_struc {
	long long imgline;
	double wpp;
	double wp;
	long long width_pixel_num;
	long long height_pixel_num;
};

bool cacu(double r,double i);
void get_informations();
void auto_cacu();
void cacu_line();
bool call_next_task(double& i,long& task_linenum);
bool is_task_finish();
void progress_bar();
void make_argv(int argc,char** argv);
void call_help();
void print_informations();
void check_informations();
void swap_double(double& first,double& second);
void make_position(double *start_x,double *start_y,double *end_x,double *end_y);
void cacu_argv();
