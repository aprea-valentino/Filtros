from distutils import cmd
import streamlit as st
import pandas as pd
import imageio.v2 as imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from time import sleep
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)


def show_image(inicio):
    if (inicio==True):
        img = imageio.imread("../src/imgs/ashitaka.ppm")
    else:
        img = imageio.imread("../src/out/out.ppm")
    plt.imshow(img)
    st.pyplot()

def run_filters(threads, c, b, blur, bw, frame, eDetection, s):
    filtrosAplic = ''
    parametros = ''
    #Uso: ./main <filtro> <nthreads> <[p1]> <[p2]> <img1> <img2> <custom_output>
    if c > 0:
        filtrosAplic = "contrast"
        parametros = f'{c} 1'
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)

    if b > 0:
        filtrosAplic = "brightness"
        b = b / 100
        parametros = f'{b} 1'
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)
    
    if s > 0:
        filtrosAplic = "shades"
        parametros = f'{s} 1'
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)
    
    if blur == True:
        filtrosAplic = "blur"
        parametros = "1 1"
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)

    if bw == True:
        filtrosAplic = "blackWhite"
        parametros = "1 1"
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)

    if frame == True:
        filtrosAplic = "frame"
        parametros = "30 30"
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)

    if eDetection == True:
        filtrosAplic = "edgedetection"
        parametros = "1 1"
        cmd = '../src/main ' + filtrosAplic + ' ' + str(threads) + ' ' + parametros + ' ../src/imgs/ashitaka.ppm ../src/imgs/ashitaka.ppm ../src/out/out.ppm'
        os.system(cmd)

inicio= True
aplicado = False
# Sidebar
st.sidebar.header('Parámetros')

c = st.sidebar.slider('Contraste', 0, 100, 0, 1, '%d')
b = st.sidebar.slider('Brillo', 0, 100, 0, 1, '%d')
s = st.sidebar.slider('Shades', 0, 255, 0, 1, '%d')
threads = st.sidebar.slider('Threads', 1, 32, 1, 1, '%d')

blur = st.sidebar.checkbox('Box blur',value=False)
bw = st.sidebar.checkbox('BlackWhite',value=False)
frame = st.sidebar.checkbox('Frame', value=False)
eDetection = st.sidebar.checkbox('Edge detection', value=False)


if st.sidebar.button('Aplicar'):
    run_filters(threads, c, b, blur, bw, frame, eDetection, s)
    aplicado = True

# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')
if (aplicado):
    show_image(False)
else:
    show_image(True)
