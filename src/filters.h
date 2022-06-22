#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img);
void contrast(ppm& img, float contrast);
void brightness(ppm& img, float b);
void shades(ppm& img, unsigned char n);
void merge(ppm& img1, ppm& img2, float alpha);
void frame(ppm& img, int c, int x);
void boxBlur(ppm &img);
void zoom(ppm &img, ppm &img_zoomed, int n);
void edgeDetection(ppm &img, ppm &img_target);
void plain(ppm &img, unsigned char c);

// MULTI-THREAD FILTERS
void constrastThreadMain(ppm& img, float contrast, int n);
void blackWhiteThreadsMain(ppm& img, int n);
void boxBlurThreadsMain(ppm& img, int n);
void edgeDetectionThreadsMain(ppm &img, ppm &img_target, int n);

// LOADER
void loaderFilters(string filter, float p1, float p2, vector<ppm> imagenes, vector<string> imagenesN);
#endif