#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <fstream>
#include <math.h>
#include <string>
#include <cstring>
using namespace std;

const unsigned int img_width = 1920;
const unsigned int img_height = 1280;

class mandelPlot
{
	private:
		double l_lim, r_lim, u_lim, d_lim, origx, origy;
		unsigned int max_iter;
		double zoom;
		sf::Image img;
		string prevkey;
	public:
		mandelPlot()
		{
			max_iter = 150;
			l_lim = -2;
			r_lim = 1;
			u_lim = 1;
			d_lim = -1;
			zoom = 1;
			origx = -0.5;
			origy = 0;
			prevkey = "n";
			img.create(img_width, img_height, sf::Color(0,0,0));
		}
		sf::Image getImg()
		{
			return img;
		}
		int value (int x, int y)
		{
			complex<double> pt((double)(r_lim-l_lim)*x/img_width+l_lim,(double)(u_lim-d_lim)*y/img_height+d_lim);
			complex<double> z(0,0);
			unsigned int iter = 0;
			double smoothcolor = 0;
			while(abs(z)<2 && iter<max_iter)
			{
				z = z*z+pt;
				iter++;
				smoothcolor += exp(-abs(z));
			}
			if(iter<max_iter)
			{
				return (int)255*1.8*smoothcolor/max_iter;
			}
			else return 0;
		}
		void updateOrig()
		{
			origx = 0.5*(r_lim+l_lim);
			origy = 0.5*(u_lim+d_lim);
			//cout << "Updated orig!" << endl;
		}
		double getZoom()
		{
			return zoom;
		}
		void updateZoom(double zm)
		{
			zoom = zm*zoom;
			cout << "Zoom updated to " << zoom << endl;
			max_iter = 60 + (int) 50*log10(1/zoom);
			updateLims();
		}
		void zoomIn()
		{
			updateZoom(1.0/3.0);
			prevkey="n";
		}
		void zoomOut()
		{
			updateZoom(3.0);
			prevkey="n";
		}
		void updateLims()
		{
			u_lim = origy + zoom;
			d_lim = origy - zoom;
			r_lim = origx + 1.5*zoom;
			l_lim = origx - 1.5*zoom;
			//cout << "Updated zoom!" << endl;
		}
		void moveUp()
		{
			u_lim -= 0.1*(u_lim-origy);
			d_lim -= 0.1*(u_lim-origy);
			updateOrig();
			prevkey = "w";
		}
		void moveDown()
		{
			u_lim += 0.1*(u_lim-origy);
			d_lim += 0.1*(u_lim-origy);
			updateOrig();
			prevkey = "s";
		}
		void moveLeft()
		{
			l_lim -= 0.1*(r_lim-origx);
			r_lim -= 0.1*(r_lim-origx);
			updateOrig();
			prevkey = "a";
		}
		void moveRight()
		{
			l_lim += 0.1*(r_lim-origx);
			r_lim += 0.1*(r_lim-origx);
			updateOrig();
			prevkey = "d";
		}
		void updateImg()
		{

			int wid = 0.95*img_width;
			int high = 0.95*img_height;
			if (prevkey=="w")
			{
				sf::Image tmpimg = img;
#pragma omp parallel for
				for (int i = 0; i < img_width; i++){
#pragma omp parallel for
					for(int j = 0; j < high; j++) {
						tmpimg.setPixel(i,img_height-high+j,img.getPixel(i,j));
					}
#pragma omp parallel for
					for(int j = 0; j < (img_height-high); j++) {
						tmpimg.setPixel(i,j,sf::Color(value(i,j),0,0));
					}
				}
				img = tmpimg;
			}
			else if (prevkey=="s")
			{
				sf::Image tmpimg = img;
#pragma omp parallel for
				for (int i = 0; i < img_width; i++){
#pragma omp parallel for
					for(int j = 0; j < high; j++) {
						tmpimg.setPixel(i,j,img.getPixel(i,img_height-high+j));
					}
#pragma omp parallel for
					for(int j = high; j < img_height; j++) {
						tmpimg.setPixel(i,j,sf::Color(value(i,j),0,0));
					}
				}
				img = tmpimg;
			}
			else if (prevkey=="a")
			{
				sf::Image tmpimg = img;
#pragma omp parallel for
				for (int i = 0; i < img_height; i++){
#pragma omp parallel for
					for(int j = 0; j < wid; j++) {
						tmpimg.setPixel(img_width-wid+j,i,img.getPixel(j,i));
					}
#pragma omp parallel for
					for(int j = 0; j < (img_width-wid); j++) {
						tmpimg.setPixel(j,i,sf::Color(value(j,i),0,0));
					}
				}
				img = tmpimg;
			}
			else if (prevkey=="d")
			{
				sf::Image tmpimg = img;
#pragma omp parallel for
				for (int i = 0; i < img_height; i++){
#pragma omp parallel for
					for(int j = 0; j < wid; j++) {
						tmpimg.setPixel(j,i,img.getPixel(img_width-wid+j,i));
					}
#pragma omp parallel for
					for(int j = wid; j < img_width; j++) {
						tmpimg.setPixel(j,i,sf::Color(value(j,i),0,0));
					}
				}
				img = tmpimg;
			}
			else
			{
#pragma omp parallel for
				for(int i = 0; i < img_width; i++){
#pragma omp parallel for
					for(int j = 0; j < img_height; j++) 
					{
						img.setPixel(i,j,sf::Color(value(i,j),0,0));
					}
				}
			}
		}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(img_width, img_height), "Mandelbrot");
	sf::Image image;
	sf::Texture tex;
	sf::Sprite sprite;
	mandelPlot mp;
	bool update = true;
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type){
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					update = true;
					switch(event.key.code)
					{
						case sf::Keyboard::Q:
							mp.zoomIn();
							update = true;
							break;
						case sf::Keyboard::E:
							mp.zoomOut();	
							update = true;
							break;
						case sf::Keyboard::W:
							mp.moveUp();	
							update = true;
							break;
						case sf::Keyboard::S:
							mp.moveDown();	
							update = true;
							break;
						case sf::Keyboard::A:
							mp.moveLeft();	
							update = true;
							break;
						case sf::Keyboard::D:
							mp.moveRight();	
							update = true;
							break;
						default:
							update = false;
							break;
					}
				default:
					break;
			}
		}
		if (update)
		{
			mp.updateImg();	
			tex.loadFromImage(mp.getImg());
			sprite.setTexture(tex);
			update = false;
		}
		window.draw(sprite);
		window.display();
	}
	return 0;
}
