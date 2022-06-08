#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  
#include "ppm.h"

#define BLACK 0

using namespace std;

float sumatoria(ppm& img, int i, int j, string rgb)
{
	float suma;

	if (rgb == "r")
	{
		suma = img.getPixel(i,j).r + img.getPixel(i+1,j).r + img.getPixel(i-1,j).r + img.getPixel(i,j+1).r + img.getPixel(i,j-1).r + img.getPixel(i+1,j+1).r + img.getPixel(i-1,j-1).r  + img.getPixel(i+1,j-1).r + img.getPixel(i-1,j+1).r;
	}
	else if (rgb == "g")
	{
		suma = img.getPixel(i,j).g + img.getPixel(i+1,j).g + img.getPixel(i-1,j).g + img.getPixel(i,j+1).g + img.getPixel(i,j-1).g + img.getPixel(i+1,j+1).g + img.getPixel(i-1,j-1).g + img.getPixel(i+1,j-1).g + img.getPixel(i-1,j+1).g;
	}
	else if (rgb == "b")
	{
		suma = img.getPixel(i,j).b + img.getPixel(i+1,j).b + img.getPixel(i-1,j).b + img.getPixel(i,j+1).b + img.getPixel(i,j-1).b + img.getPixel(i+1,j+1).b + img.getPixel(i-1,j-1).b + img.getPixel(i+1,j-1).b + img.getPixel(i-1,j+1).b;
	}
	return suma;	 
}
int verificar(int resultado)
{
	if (resultado > 255){
		resultado = 255;
	}
	else if (resultado < 0)
	{
		resultado = 0;
	} 
	return resultado;
}
// COMPLETAR :)
void blackWhite(ppm& img)
{
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;
			int resultado = (R+G+B)/3;
			resultado = verificar(resultado);
			img.setPixel(i,j, pixel(resultado,resultado,resultado));
		}	
	}	
}
void contrast(ppm& img, float contrast)
{
	int f = ((259*(contrast+255))/(255*(259-contrast)));

	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i,j).r;
			int resultadoR = f*(R - 128) + 128;
			resultadoR = verificar(resultadoR);
			int G = img.getPixel(i,j).g;
			int resultadoG = f*(G - 128) + 128;
			resultadoG = verificar(resultadoG);
			int B = img.getPixel(i,j).b;
			int resultadoB = f*(B - 128) + 128;
			resultadoB = verificar(resultadoB);
			img.setPixel(i,j, pixel(resultadoR, resultadoG, resultadoB));
		}
	}
}
void brightness(ppm& img, float b)
{
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i,j).r;
			int resultadoR = R + 255 * b;
			resultadoR = verificar(resultadoR);
			resultadoR = trunc(resultadoR);
			int G = img.getPixel(i,j).g;
			int resultadoG = G + 255 * b;
			resultadoG = verificar(resultadoG);
			resultadoG = trunc(resultadoG);
			int B = img.getPixel(i,j).b;
			int resultadoB = B + 255 * b;
			resultadoB = verificar(resultadoB);
			resultadoB = trunc(resultadoB);

			img.setPixel(i,j, pixel(resultadoR,resultadoG,resultadoB));
		}
	}
}

void merge(ppm& img, ppm& img2, float alpha)
{
    float p2 = 1 - alpha;
    for(int i = 0; i < img.height; i++)
    {
        for(int j = 0; j < img.width; j++)
        {
            int R1 = img.getPixel(i,j).r;
            int G1 = img.getPixel(i,j).g;
            int B1 = img.getPixel(i,j).b;
            int R2 = img2.getPixel(i,j).r;
            int G2 = img2.getPixel(i,j).g;
            int B2 = img2.getPixel(i,j).b;

            int resultadoR = (R1 * alpha) + (R2 * p2);
            resultadoR = verificar(resultadoR);
            int resultadoG = (G1 * alpha) + (G2 * p2);
            resultadoG = verificar(resultadoG);
            int resultadoB = (B1 * alpha) + (B2 * p2);
            resultadoB = verificar(resultadoB);
            
			img.setPixel(i,j, pixel(resultadoR, resultadoG, resultadoB));
        }
    }

}
void shades(ppm& img, unsigned char n){
	int rang = 255/(n-1);
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int r = img.getPixel(i,j).r;
			int g = img.getPixel(i,j).g;
			int b = img.getPixel(i,j).b;
			int g1 = (r+g+b)/3;
			int g2 = (g1/rang)*rang;
			g2 = verificar(g2);
			img.setPixel(i,j,pixel(g2,g2,g2).truncate());
			
		}
	}
}
void boxBlur(ppm &img)
{
	float rgod;
	float ggod;
	float bgod;
	for(int i = 1; i < img.height-2; i++)
	{
		for(int j = 1; j < img.width-2; j++)
		{
			rgod = sumatoria(img, i, j, "r");
			ggod = sumatoria(img, i,j,"g");
			bgod = sumatoria(img,i,j,"b");
			rgod = rgod / 9;
			rgod = verificar(rgod);
			ggod = ggod / 9;
			ggod = verificar(ggod);
			bgod = bgod / 9;
			bgod = verificar(bgod);
			img.setPixel(i,j,pixel(rgod,ggod,bgod));
		}	
	}	
}
void frame(ppm& img, pixel color, int x)
{
	for(int i = 0; i < x; i++)//altura
	{
		for(int j = 0; j < img.width; j++)//anchura
		{
			img.setPixel(i,j,color);
		}
	}
	for(int i = img.height - x; i < img.height; i++)//altura
	{
		for(int j = 0; j < img.width; j++)//anchura
		{
			img.setPixel(i,j,color);
		}
	}
	for(int i = 0; i < img.height; i++)//altura
	{
		for(int j = 0; j < x; j++)//anchura
		{
			img.setPixel(i,j,color);
		}
	}
	for(int i = 0; i < img.height; i++)//altura
	{
		for(int j = img.width - x ; j < img.width; j++)//anchura
		{
			img.setPixel(i,j,color);
		}
	}			
			
}

void edgeDetection(ppm &img, ppm &img_target)
{

	blackWhite(img);
	boxBlur(img);

	for(int i = 1; i < img.height-1; i++)
	{
		for(int j = 1; j < img.width-1; j++)
		{
			pixel p_final = pixel();
			pixel p0 = img.getPixel(i - 1, j - 1);
			pixel p1 = img.getPixel(i - 1, j);
			pixel p2 = img.getPixel(i - 1, j + 1);
			pixel p3 = img.getPixel(i, j - 1);
			pixel p4 = img.getPixel(i, j);
			pixel p5 = img.getPixel(i, j + 1);
			pixel p6 = img.getPixel(i + 1, j - 1);
			pixel p7 = img.getPixel(i + 1, j);
			pixel p8 = img.getPixel(i + 1, j + 1);

			unsigned int gxr = (p0.r - p2.r + 2 * p3.r - 2 * p5.r + p6.r - p8.r);
			unsigned int gyr = (p0.r + 2 * p1.r + p2.r - p6.r - 2 * p7.r - p8.r);
			p_final.r = sqrt(gxr * gxr + gyr * gyr);

			unsigned int gxg = (p0.g - p2.g + 2 * p3.g - 2 * p5.g + p6.g - p8.g);
			unsigned int gyg = (p0.g + 2 * p1.g + p2.g - p6.g - 2 * p7.g - p8.g);
			p_final.g = sqrt(gxg * gxg + gyg * gyg);
			
			unsigned int gxb = (p0.b - p2.b + 2 * p3.b - 2 * p5.b + p6.b - p8.b);
			unsigned int gyb = (p0.b + 2 * p1.b + p2.b - p6.b - 2 * p7.b - p8.b);
			p_final.b = sqrt(gxb * gxb + gyb * gyb);
			
			p_final.truncate();
			img_target.setPixel(i-1, j-1, p_final);
		}	
	}
	img = img_target;
}

void contrastThreads(ppm& img, float contrast, int i0, int i1)
{
	int f = ((259*(contrast+255))/(255*(259-contrast)));

	for(i0; i0 < i1; i0++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i0,j).r;
			int resultadoR = f*(R - 128) + 128;
			resultadoR = verificar(resultadoR);
			int G = img.getPixel(i0,j).g;
			int resultadoG = f*(G - 128) + 128;
			resultadoG = verificar(resultadoG);
			int B = img.getPixel(i0,j).b;
			int resultadoB = f*(B - 128) + 128;
			resultadoB = verificar(resultadoB);
			img.setPixel(i0,j, pixel(resultadoR, resultadoG, resultadoB));
		}
	}
}

void constrastThreadMain(ppm& img, float contrast, int n){
	int filas = int(img.height / n);
	vector<thread> threads;
	for (int i = 0; i < n; i++)
	{
		int inicio = i * filas;
		int fin = (i + 1) * filas;
		threads.push_back(thread(contrastThreads, ref(img), contrast, inicio, fin));
	}
	for (int i = 0; i < n; i++){
		threads[i].join();
	}
}

void blackWhiteThreads(ppm& img, int i0, int i1)
{
	for(i0; i0 < i1; i0++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i0,j).r;
			int G = img.getPixel(i0,j).g;
			int B = img.getPixel(i0,j).b;
			int resultado = (R+G+B)/3;
			resultado = verificar(resultado);
			img.setPixel(i0,j, pixel(resultado,resultado,resultado));
		}	
	}	
}

void blackWhiteThreadsMain(ppm& img, int n){
	int filas = int(img.height / n);
	vector<thread> threads;
	for (int i = 0; i < n; i++)
	{
		int inicio = i * filas;
		int fin = (i + 1) * filas;
		threads.push_back(thread(blackWhiteThreads, ref(img), inicio, fin));
	}
	for (int i = 0; i < n; i++){
		threads[i].join();
	}
}

void boxBlurThreads(ppm &img, int i0, int i1)
{
	float rgod;
	float ggod;
	float bgod;

	pixel p_final = pixel();
	
	if (i0 == 0){
		i0 += 1;
	}
	if (i1 == img.height){
		i1 -= 1;
	}

	for(i0; i0 < i1; i0++)
	{
		for(int j = 1; j < img.width-1; j++)
		{

			pixel p0 = img.getPixel(i0 - 1, j - 1);
			pixel p1 = img.getPixel(i0 - 1, j);
			pixel p2 = img.getPixel(i0 - 1, j + 1);
			pixel p3 = img.getPixel(i0, j - 1);
			pixel p4 = img.getPixel(i0, j);
			pixel p5 = img.getPixel(i0, j + 1);
			pixel p6 = img.getPixel(i0 + 1, j - 1);
			pixel p7 = img.getPixel(i0 + 1, j);
			pixel p8 = img.getPixel(i0 + 1, j + 1);

			rgod = p0.r + p1.r + p2.r +
				   p3.r + p4.r + p5.r +
				   p6.r + p7.r + p8.r;

			ggod = p0.g + p1.g + p2.g +
				   p3.g + p4.g + p5.g +
				   p6.g + p7.g + p8.g;
			
			bgod = p0.b + p1.b + p2.b +
				   p3.b + p4.b + p5.b +
				   p6.b + p7.b + p8.b;

			p_final.r = int(rgod / 9);
			p_final.g = int(ggod / 9);
			p_final.b = int(bgod / 9);
			p_final.truncate();

			img.setPixel(i0,j, p_final);
		}	
	}	
}

void boxBlurThreadsMain(ppm& img, int n){
	int filas = int(img.height / n);
	vector<thread> threads;
	for (int i = 0; i < n; i++)
	{
		int inicio = i * filas;
		int fin = (i + 1) * filas;
		threads.push_back(thread(boxBlurThreads, ref(img), inicio, fin));
	}
	for (int i = 0; i < n; i++){
		threads[i].join();
	}
}

void edgeDetectionThreads(ppm &img, ppm &img_target, int i0, int i1)
{

	if (i0 == 0){
		i0 += 1;
	}
	if (i1 == img.height){
		i1 -= 1;
	}
	
	for(i0; i0 < i1; i0++)
	{
		for(int j = 1; j < img.width-1; j++)
		{
			pixel p_final = pixel();
			pixel p0 = img.getPixel(i0 - 1, j - 1);
			pixel p1 = img.getPixel(i0 - 1, j);
			pixel p2 = img.getPixel(i0 - 1, j + 1);
			pixel p3 = img.getPixel(i0, j - 1);
			//pixel p4 = img.getPixel(i0, j);
			pixel p5 = img.getPixel(i0, j + 1);
			pixel p6 = img.getPixel(i0 + 1, j - 1);
			pixel p7 = img.getPixel(i0 + 1, j);
			pixel p8 = img.getPixel(i0 + 1, j + 1);

			unsigned int gxr = (p0.r - p2.r + 2 * p3.r - 2 * p5.r + p6.r - p8.r);
			unsigned int gyr = (p0.r + 2 * p1.r + p2.r - p6.r - 2 * p7.r - p8.r);
			p_final.r = sqrt(gxr * gxr + gyr * gyr);

			unsigned int gxg = (p0.g - p2.g + 2 * p3.g - 2 * p5.g + p6.g - p8.g);
			unsigned int gyg = (p0.g + 2 * p1.g + p2.g - p6.g - 2 * p7.g - p8.g);
			p_final.g = sqrt(gxg * gxg + gyg * gyg);
			
			unsigned int gxb = (p0.b - p2.b + 2 * p3.b - 2 * p5.b + p6.b - p8.b);
			unsigned int gyb = (p0.b + 2 * p1.b + p2.b - p6.b - 2 * p7.b - p8.b);
			p_final.b = sqrt(gxb * gxb + gyb * gyb);
			
			p_final.truncate();
			img_target.setPixel(i0-1, j-1, p_final);
		}	
	}
	if (i1 == img.height-1){
		img = img_target;
	}
}

void edgeDetectionThreadsMain(ppm &img, ppm &img_target, int n){
	int filas = int(img.height / n);
	vector<thread> threads;
	for (int i = 0; i < n; i++)
	{
		int inicio = i * filas;
		int fin = (i + 1) * filas;

		threads.push_back(thread(edgeDetectionThreads, ref(img), ref(img_target),  inicio, fin));
	}
	for (int i = 0; i < n; i++){
		threads[i].join();
	}
}

// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}
