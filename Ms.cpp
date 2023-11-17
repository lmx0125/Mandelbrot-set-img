#include "Ms.h"

double prec;

long wigth,height;
double wight_per_pixel;
double times;
long all;
int cpu_thread_mun;
std::vector<std::vector<bool>> bool_img;
long img_line;

long cacued_pixel;

struct C {
	double r;
	double i;
};

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

bool call_next_task(double& wpp,double& i){
	if (is_task_finish())
		return false;
	wpp = wight_per_pixel;
	times++;
	i = height - times * wpp;
	return true;
}

bool is_task_finish(){
	if(img_line < 2 / wight_per_pixel)
		return false;
	return true;
}

void cacu_line(){

	double wpp;
	double i;
	double r;
	int linenum;

start_cacu:

	linenum = img_line;
	img_line++;
	r = wigth;
	
	//std::cout << "[DEBUG] [func > cacu_line() > value -> linenum = " << linenum << " | img_line = " << img_line << " | r = " << r << " ]" << std::endl;
	
	if(!call_next_task(wpp,i)){
		//std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [THREAD > " << pthread_self() << " | " << RED << "EXIT" << DEFAULT << " ]" << std::endl;
		return;
	}


	//std::cout << "[DEBUG] [func > cacu_line() > value -> wpp = " << wpp << " | i = " << i << " ]" << std::endl;
	
	//std::cout << "[DEBUG] [func > cacu_line() > call_next_task() > pass]" << std::endl;

	for (int loop_i = 0; loop_i < 3 / wight_per_pixel; ++loop_i) {
		bool_img[linenum][loop_i] = cacu(r,i);
		r += wpp;
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

	std::cout << "[" << GREEN << "PARA" << DEFAULT << "] Quality (points per unit) : ";
	std::cin >> temp;

	prec = stoi(temp);

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [prec > " << prec << " ]" << std::endl;
}

int main(){
	get_informations();

	wigth = -2;
	height = 1;
	img_line = 0;
	wight_per_pixel = 1.0 / prec;
	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [wpp > " << wight_per_pixel << " ]" << std::endl;

	struct C point;
	point.r = wigth;
	point.i = height;

	cacued_pixel = 0;
	times = 0;
	all = 6 * prec * prec;

	bool_img.resize(2 * prec, std::vector<bool>(3 * prec, 0));

	auto_cacu();
	/*
	for (int i = 0; i < 2 * prec; ++i) {
		point.r = -2.0;
		bool_img.push_back(std::vector<bool>());
		for (int j = 0; j < 3 * prec; ++j) {
			//std::cout << "[" << point.r << "," << point.i << "]";
			//std::cout << cacu(point.r,point.i);
			bool_img[i].push_back(cacu(point.r,point.i));
			point.r = point.r + wight_per_pixel;
			times++;
			std::cout << "\r[INFO] [POINT > " << times << "/" << all << " | " << static_cast<int>(times / all * 100) << "% ]";
		}
		point.i = point.i - wight_per_pixel;
	}
	std::cout << std::endl;
	*/

#ifdef __linux__
		func_sleep(100000);
#else
		func_sleep(100);
#endif

	cimg_library::CImg<unsigned char> img(3 * prec,2 * prec,1,1,0);
	unsigned char color[] = {255};

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [IMAGE > Canvas Gen > " << GREEN << "Pass" << DEFAULT << "]" << std::endl;

	img.fill(false);

	for (int i = 0; i < 2 * prec; ++i) {
		for (int j = 0; j < 3 * prec; ++j) {
			if(bool_img[i][j] != false)
				img.draw_point(j, i,color);
		}
	}

	const char* image = "set.bmp";
        img.save(image);

	std::cout << "[" << BLUE << "INFO" << DEFAULT << "] [IMAGE > Save > " << GREEN << "Pass" << DEFAULT << "]" << std::endl;

	return 0;
}

