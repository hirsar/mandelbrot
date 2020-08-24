#include <iostream>
#include <complex>
#include <fstream>
#include <math.h>
using namespace std;

const unsigned int img_width = 1920;
const unsigned int img_height = 1080;
unsigned int max_iter = 300;
float l_lim = -2;
float r_lim = 1;
float u_lim = 1;
float d_lim = -1;

int value (int x, int y) {
	complex<float> point((float) (r_lim-l_lim)*y/img_width+l_lim, (float) (u_lim-d_lim)*x/img_height+d_lim);
	complex<float> z(0,0);
	unsigned int iter = 0;
	while(abs(z)<2 && iter < max_iter) {
		z = z*z+point;
		iter++;
	}
	if(iter < max_iter){
		return 255*iter/max_iter;
	}
	else return 0;
}

int main()
{
	ofstream File("mandel.ppm", ios_base::out);
	if(File.is_open())
	{
		File << "P3\n" << img_width << " " << img_height << " 255\n";
		for (int i = 0; i < img_height; i++){
			for(int j = 0; j < img_width; j++) {
				int val = value(i,j);
				File << val << " " << 0 << " " << 0 << "\n";
			}
		}
		File.close();
	}
	else cout << "File cannot be opened!";
	return 0;
}
