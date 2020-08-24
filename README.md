- "mandelbrot.cpp": Plots the Mandelbrot set. Use "W", "A", "S", "D" to navigate, "=" to zoom in and "-" to zoom out. Created using SFML.

	Compile with 
  > g++ mandelbrot.cpp -o mandelbrot_plotter -lsfml-graphics -lsfml-window -lsfml-system -fopenmp
  and run with 
  >./mandelbrot_plotter

Screenshots of Mandelbrot set obtained using Mandelbrot plotter (requires SFML and OpenMP).

![Initial View](https://github.com/hirsar/mandelbrot/blob/master/Screenshot%20from%202020-08-23%2021-25-35.png)

![Upper Region](https://github.com/hirsar/mandelbrot/blob/master/Screenshot%20from%202020-08-23%2021-25-58.png)

![Further Zoom on Upper Region](https://github.com/hirsar/mandelbrot/blob/master/Screenshot%20from%202020-08-23%2021-27-16.png)

- "mandelbrot_to_ppm.cpp": Creates a ppm file of the Mandelbrot set.

	Compile with 
  > g++ mandelbrot_to_ppm.cpp -o mandelbrot_ppm
  and run with 
  > ./mandelbrot_ppm
