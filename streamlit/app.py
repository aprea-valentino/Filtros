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
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)


def show_image():
    img = imageio.imread("../src/imgs/ashitaka.ppm")
    plt.imshow(img)
    st.pyplot()

def run_filters(c, b, blur, bw, frame, eDetection, m, s):
    if c == True:
        filtrosAplic += "contrast"
        parametros += f'{c} '

    if b == True:
        filtrosAplic += "brightness"
        parametros += f'{b} '

    if blur == True:
        filtrosAplic += "blur"
        parametros += " "

    if bw == True:
        filtrosAplic += "blackWhite"
        parametros += " "
        
    if frame == True:
        filtrosAplic += "frame"
        parametros +=


    cmd = '../src/main '  + '"'+ filtrosAplic + '" '+ "1" + '"'+ parametros + '" ' + '../src/imgs/ashitaka.ppm ../src/out/out.ppm ' + '0'
    os.system(cmd)
   
# Sidebar
st.sidebar.header('Parámetros')

c = st.sidebar.slider('Contraste',-100, 100, 0, 1, '%d')
b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')
m = st.sidebar.slider('Merge', -100, 100, 0, 1, '%d')
s = st.sidebar.slider('Shades', +100, 100, 0, 1, '%d')

blur = st.sidebar.checkbox('Box blur',value=False)
bw = st.sidebar.checkbox('BlackWhite',value=False)
frame = st.sidebar.checkbox('Frame', value=False)
eDetection = st.sidear.checkbox('Edge detection', value=False)


if st.sidebar.button('Aplicar'):
    run_filters(c, b, blur, bw)


# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

show_image()
