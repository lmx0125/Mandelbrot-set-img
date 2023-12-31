#include "Ms.h"

//pixel per width -> 1 / width_per_pixel
double prec;
//img wigth and height
long long wigth,height;
double width_per_pixel;
double times;
long long all;
int cpu_thread_mun;
std::vector<std::vector<bool>> bool_img;
long long img_line;
long long cacued_pixel;
struct pos start_point_pos;
struct pos end_point_pos;
int wp;
long long width_pixel;
long long height_pixel;
struct argv_status_struc argv_status;
struct img_status_struc img_status;

void cacu_argv(){
	argv_status.height = argv_status.start_y_pos - argv_status.end_y_pos;
	argv_status.width = argv_status.end_x_pos - argv_status.start_x_pos;
	img_status.width_pixel_num = img_status.wp / argv_status.width * argv_status.width;
	img_status.height_pixel_num = img_status.wp / argv_status.width * argv_status.height;
	img_status.wpp = argv_status.width / img_status.wp;
}

void make_position(double *start_x,double *start_y,double *end_x,double *end_y) {
	width_pixel = wp;
	height_pixel = wp / (end_x - start_x) * (start_y - end_y);
	//std::cout << "[" << RED << "DEBU" << DEFAULT << "] [cacu pixel > "
	// 	        << width_pixel << " | " << height_pixel << "]" << std::endl << std::flush;
	argv_status.is_using_argv = true;
	argv_status.start_x_pos = *start_x;
}

void swap_double(double& first,double& second) {
	double tmp = first;
	first = second;
	second = tmp;
}

void check_informations(){
	//tidy up the start/end pos
	if (argv_status.start_x_pos > argv_status.end_x_pos)
		swap_double(argv_status.start_x_pos,argv_status.end_x_pos);
	if (argv_status.start_y_pos < argv_status.end_y_pos)
		swap_double(argv_status.start_y_pos,argv_status.end_y_pos);

	cacu_argv();

	if (argv_status.start_x &&
		argv_status.start_y &&
		argv_status.end_x &&
		argv_status.end_y &&
		argv_status.wp) {

		//std::cout << "[" << RED << "DEBU" << DEFAULT << "] [start_pos > " << start_point_pos.x << " " << start_point_pos.y << " | "
		//  		<< "end_pos > " << end_point_pos.x << " " << end_point_pos.y << "]" << std::endl << std::flush;

		std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [Generate by using argv]" << std::endl << std::flush;
		return;

	} else {
		if (!argv_status.start_x &&
			!argv_status.start_y &&
			!argv_status.end_x &&
			!argv_status.end_y &&
			!argv_status.wp) {
				get_informations();
				return;
			} else {
				std::cout << "[" << RED << "ERRO" << DEFAULT << "] [You input a invalid value or miss some argv]" << std::endl << std::flush;
				call_help();
		}
	}
}

void print_informations() {
	std::cout << std::setprecision(15);
	// std::cout << "[" << BLUE << "INFO" << DEFAULT <<"] "
	// 		  <<"[argv_status.start_x_pos > " << argv_status.start_x_pos << " "
	// 		  << "argv_status.start_y_pos > " << argv_status.start_y_pos << " "
	// 		  << "argv_status.end_x_pos > " << argv_status.end_x_pos << " "
	// 		  << "argv_status.end_y_pos > " << argv_status.end_y_pos << " "
	// 		  << "all > " << all << "]" << std::endl << std::flush;
}

void call_help() {
	std::cout << "[" << GREEN << "HELP" << DEFAULT << "] " << std::endl
			  << "[" << GREEN << "COMMAND" << DEFAULT << "] > ./set -sx <start_x_pos> -sy <start_y_pos> -ex <end_x_pos> -ey <end_y_pos> -wp <width_pixel_num>" << std::endl
			  << ">>> \"-sx\" > start_x_pos \"-sy\" > start_y_pos" << std::endl
			  << ">>> \"-ex\" > end_x_pos \"-ey\" > end_y_pos" << std::endl
			  << ">>> \"-wp\" > width_pixel_num" << std::endl
			  << "#******************************************#" << std::endl << std::flush;
			  exit(0);
}

void make_argv(int argc,char** argv){
	/*
	std::cout << "[" << RED << "DEBU" << DEFAULT << "] [argc > " << argc << " ]" << std::endl << std::flush;
	std::cout << "[" << RED << "DEBU" << DEFAULT << "] [argv > ";
	for(int i = 1; i < argc + 1; i++)
		std::cout << argv[i] << " ";
	std::cout << "]" << std::endl << std::flush;
	*/
	std::string arg;

	for (int i = 1; i < argc + 1; ++i) {
		arg = argv[i];
		try {
			if (arg == start_x_pos_tag) {
				//std::cout << " sxp > " << argv[i + 1] << " ";
				argv_status.start_x_pos = std::stod(argv[i + 1]);
				i = i + 1;
				argv_status.start_x = true;
			} else if (arg == start_y_pos_tag) {
				//std::cout << " syp > " << argv[i + 1] << " ";
				argv_status.start_y_pos = std::stod(argv[i + 1]);
				i = i + 1;
				argv_status.start_y = true;
			} else if (arg == end_x_pos_tag) {
				//std::cout << " exp > " << argv[i + 1] << " ";
				argv_status.end_x_pos = std::stod(argv[i + 1]);
				i = i + 1;
				argv_status.end_x = true;
			} else if (arg == end_y_pos_tag) {
				//std::cout << " eyp > " << argv[i + 1] << " ";
				argv_status.end_y_pos = std::stod(argv[i + 1]);
				i = i + 1;
				argv_status.end_y = true;
			} else if (arg == wp_tag) {
				img_status.wp = std::stod(argv[i + 1]);
				i++;
				argv_status.wp = true;
			} else if (arg == help_tag) {
				call_help();
			}
		} catch (const std::invalid_argument& err) {
			std::cout << "[" << RED << "DEBU" << DEFAULT << "] [err > " << err.what() << "]" << std::endl << std::flush; 
		}
	}
}

void progress_bar(){
	std::cout << std::fixed;

	while(true){
		if (is_task_finish()){
		std::cout << "\r[" << BLUE << "INFO" << DEFAULT << "] [POINT > " << all << "/" << all << " | " << "100" << "% ]" << std::endl;
			return;
		}
		std::cout << "\r[" << BLUE << "INFO" << DEFAULT << "] [POINT > " << cacued_pixel << "/" << all << " | " << static_cast<int>(static_cast<double>(cacued_pixel) / all * 100) << "% ]" << std::flush;
#ifdef __linux__
		func_sleep(50000);
#else
		func_sleep(50);
#endif
	}
}

bool call_next_task(double& i,long& task_linenum) {
	if (is_task_finish())
		return false;
	
	i = argv_status.start_y_pos - times * img_status.wpp;
	times++;
	return true;
}

bool is_task_finish(){
	if(img_status.imgline <= img_status.height_pixel_num)
		return false;
	return true;
}

void cacu_line(){
	double i;
	double r;
	long task_linenum;

start_cacu:

	task_linenum = img_status.imgline;
	img_status.imgline++;
	r = argv_status.start_x_pos;

	//std::cout << "[DEBUG] [func > cacu_line() > value -> linenum = " << linenum << " | img_status.img_line = " << img_status.img_line << " | r = " << r << " ]" << std::endl;

	if(!call_next_task(i,task_linenum)){
		//std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [THREAD > " << pthread_self() << " | " << RED << "EXIT" << DEFAULT << " ]" << std::endl;
		return;
	}


	//std::cout << "[DEBUG] [func > cacu_line() > value -> wpp = " << wpp << " | i = " << i << " ]" << std::endl;
	//std::cout << "[DEBUG] [func > cacu_line() > call_next_task() > pass]" << std::endl;

	//std::cout << "[" << RED << "DEBU" << DEFAULT << "] [task_linenum > " << task_linenum << " img_status.wpp > " << img_status.wpp << "]" << std::endl << std::flush;
	
	for (int loop_i = 0; loop_i < img_status.width_pixel_num; ++loop_i) {
		bool_img[task_linenum][loop_i] = cacu(r,i);
		//std::cout << "[" << RED << "DEBU" << DEFAULT << "] [Is cacued bool_img[" << task_linenum << "][" << loop_i << "] | value is > " << cacu(r,i) << " | (" << r << "," << i << ")]" << std::endl << std::flush;
		r += img_status.wpp;
		cacued_pixel++;
		//std::cout << "[DEBUG] [func > cacu_line() > bool_img[linenum].emplace_back(cacu(r,i)); > pass]" << std::endl;
	}

	//std::cout << "[DEBUG] [func > is_task_finish() > " << is_task_finish() << "]" << std::endl;

	if (!is_task_finish())
		goto start_cacu;

	//std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [THREAD > " << pthread_self() << " | " << RED << "EXIT" << DEFAULT << " ]" << std::endl;
	return;
}

void auto_cacu(){
	cpu_thread_mun = std::thread::hardware_concurrency();
	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [CPU > " << cpu_thread_mun << " ]" << std::endl;

	std::vector<std::thread> workers;
	for (int i = 0; i < cpu_thread_mun; ++i){
		workers.emplace_back(std::thread(cacu_line));
		std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [THREAD > " << i + 1 << " is on | " << GREEN <<"RUNNING" << DEFAULT << " ]" << std::endl;
	}

	std::thread progress_bar_thread = std::thread(progress_bar);
	progress_bar_thread.detach();

	for (std::thread &worker : workers) {
        worker.join();
    }
}

bool cacu(double r,double i){
	std::complex<double> z = 0;
	std::complex<double> c(r,i);
	for (int loop_i = 0; loop_i < iteration; ++loop_i) {
		z = z * z + c;
		if (std::abs(z) > 2) {
			return false;
		}
	}
	return true;
}

void get_informations(){
	std::string temp;
	std::cout << std::fixed;
	std::cout << "[" << GREEN << "PARA" << DEFAULT << "] Quality (points per unit) : ";
	std::cin >> temp;
	prec = stoi(temp);
	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [prec > " << prec << "]" << std::endl;
	//need to change
	//wigth = -2;
    argv_status.start_x_pos = -2;
    argv_status.start_y_pos = 1;
    argv_status.end_x_pos = 1;
    argv_status.end_y_pos = -1;
    img_status.wpp = prec;
	argv_status.height = argv_status.start_y_pos - argv_status.end_y_pos;
	argv_status.width = argv_status.end_x_pos - argv_status.start_x_pos;
    img_status.width_pixel_num = argv_status.width * img_status.wpp;
    img_status.height_pixel_num = argv_status.height * img_status.wpp;
    img_status.wpp = 1 / img_status.wpp;

    //std::cout << "[" << RED << "DEBU" << DEFAULT << "] ["
    //		  << "argv_status.height > " << argv_status.height
    //		  << " argv_status.width > " << argv_status.width
    //		  << " img_status.width_pixel_num > " << img_status.width_pixel_num
    //		  << " img_status.height_pixel_num > " << img_status.height_pixel_num 
    //		  << " img_status.wpp > " << img_status.wpp << " ]" << std::endl << std::flush;
}

int main(int argc,char** argv){
	argc--;
	argv_status.is_using_argv = false;
	img_status.imgline = 0;

	make_argv(argc, argv);
	print_informations();
	check_informations();
	//make_position(&start_point_pos.x,
	//	      &start_point_pos.y,
	//	      &end_point_pos.x,
	//	      &end_point_pos.y
	//	     );


	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [wpp > " << img_status.wpp << "]" << std::endl;

	cacued_pixel = 0;
	times = 0;
	all = img_status.width_pixel_num * img_status.height_pixel_num;

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [height pixel > " << img_status.height_pixel_num << " width pixel > " << img_status.width_pixel_num << "]" << std::endl << std::flush;
	bool_img.resize(img_status.height_pixel_num, std::vector<bool>(img_status.width_pixel_num, 0));

	auto_cacu();
	/*
	for (int i = 0; i < 2 * prec; ++i) {
		point.r = -2.0;
		bool_img.push_back(std::vector<bool>());
		for (int j = 0; j < 3 * prec; ++j) {
			//std::cout << "[" << point.r << "," << point.i << "]";
			//std::cout << cacu(point.r,point.i);
			bool_img[i].push_back(cacu(point.r,point.i));
			point.r = point.r + width_per_pixel;
			times++;
			std::cout << "\r[INFO] [POINT > " << times << "/" << all << " | " << static_cast<int>(times / all * 100) << "% ]";
		}
		point.i = point.i - width_per_pixel;
	}
	std::cout << std::endl;
	*/

#ifdef __linux__
		func_sleep(100000);
#else
		func_sleep(100);
#endif

	cimg_library::CImg<unsigned char> img(img_status.width_pixel_num,img_status.height_pixel_num,1,1,0);
	unsigned char color[] = {255};

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [IMAGE > Canvas Gen > " << GREEN << "Pass" << DEFAULT << "]" << std::endl;

	img.fill(false);

	for (int i = 0; i < img_status.height_pixel_num; ++i) {
		for (int j = 0; j < img_status.width_pixel_num; ++j) {
			if(bool_img[i][j] != false)
				img.draw_point(j,i,color);
		}
	}

	const char* image = "set.bmp";
        img.save(image);

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [IMAGE > Save > " << GREEN << "Pass" << DEFAULT << "]" << std::endl;

	for (int i = 0; i < 5; ++i)	{
		std::cout << "\r[" << BLUE << "INFO" << DEFAULT << "] [" << GREEN << "Wait " << 5 - i << " seconds to exit" << DEFAULT << "]" << std::flush;
#ifdef __linux__
		func_sleep(1000000);
#else
		func_sleep(1000);
#endif
	}

	// for (int i = 0; i < img_status.height_pixel_num; ++i) {
	// 	for (int j = 0; j < img_status.width_pixel_num; ++j){
	// 		std::cout << bool_img[i][j];
	// 	}
	// 	std::cout << std::endl;
	// }

	std::cout << std::endl;
	return 0;
}

