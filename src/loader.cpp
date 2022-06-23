#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#define ONE_OVER_BILLION 1E-9
#include <filesystem>
#include <dirent.h>
namespace fs = std::filesystem;

using namespace std;

// El siguiente es un template basico que pueden usar como base

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./loader <filtro> <n_threads> <[p1]> <[p2]> <root_dir> <out_dir>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	float p1 = atof(argv[3]);
    float p2 = atof(argv[4]);
	string root = string(argv[5]);
	string out = string(argv[6]);
    
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);
     
	DIR *dir; struct dirent *diread;
    vector<string> arch;
    if ((dir = opendir(root.c_str())) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            arch.push_back(diread->d_name);
        }
        closedir (dir);
    }
    
    vector<string> archivos;
    for(auto i : arch){
        string tipo;
        if (i.size() > 4){
            tipo = i.substr(i.size() - 4, 4);
        } 
        
        if (tipo == ".ppm"){  
            archivos.push_back(i);
        }
    }
    if (archivos.size() < n){
        n = archivos.size();
    }

	vector<thread> threads;
    
    int cant = int(archivos.size() / n);
    for(int i = 0; i < n; i++){
        vector<ppm> imagenes;
        vector<string> imagenesN;
        int inicio = i * cant;
		int fin = ((i + 1) * cant) - 1;
		if (i == n-1){
			fin = archivos.size();
		}
        for(inicio; inicio < fin+1; inicio++){
            if (inicio == fin){
                break;
            }
            ppm img(root + archivos[inicio]);
            string outfinal = out + archivos[inicio];
            imagenesN.push_back(outfinal);
            imagenes.push_back(img);
        }
        threads.push_back(thread(loaderFilters, filter, p1, p2, imagenes, imagenesN));
    }
    for (int i = 0; i < n; i++){
		threads[i].join();
	}
    

   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
    
    cout << accum;
    cout << " s" << endl;
	return 0;
}
