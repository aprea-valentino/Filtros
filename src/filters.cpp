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
// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}
