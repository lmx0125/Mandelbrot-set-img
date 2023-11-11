#include <iostream>
#include <string.h>
#include <vector>
#include <complex>
#include <cmath>
#include "CImg.h"

double prec;
#define iteration 500

long wigth,height;
double wight_per_pixel;

struct C {
	double r;
	double i;
};

class noname
{
public:
	noname() : real(0), virt(0) {}
	virtual ~noname() { }

	double real;
	double virt;

	noname& operator +=(noname &a) {
		this->real += a.real;
		this->virt += a.virt;
		return *this;
	}
	noname& operator *=(noname &a) {
		double real = this->real;
		double virt = this->virt;
		this->real = real * a.real - virt * a.virt;
		this->virt = real * a.virt + virt * a.real;
		return *this;
	}

	void print() {
		printf("real:%f, virt:%f\n", this->real, this->virt);
	}
};

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

	std::cout << "[参数]精度(points per unit) : ";
	std::cin >> temp;

	prec = stoi(temp);

	std::cout << "[INFO] prec = " << prec << std::endl;
}

int main(){
	get_informations();

	wigth = -2;
	height = 1;
	wight_per_pixel = 1.0 / prec;
	std::cout << "wpp : " << wight_per_pixel << std::endl;
	std::vector<std::vector<bool>> bool_img;

	struct C point;
	point.r = wigth;
	point.i = height;

	std::cout << std::endl;

	double times = 0;
	long all = 6 * prec * prec;

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

	std::cout << cacu(0,0) << std::endl;

	cimg_library::CImg<unsigned char> img(3 * prec,2 * prec,1,3,0);
	
	std::cout << "[INFO] : img create pass" << std::endl;
	
	unsigned char color[] = {255, 255, 255};
	
	for (int i = 0; i < 2 * prec; ++i) {
		for (int j = 0; j < 3 * prec; ++j) {
			if(bool_img[i][j] != false)
				img.draw_point(j, i, color);
		}
	}

	const char* image = "set.bmp";
        img.save(image);

	return 0;
}

