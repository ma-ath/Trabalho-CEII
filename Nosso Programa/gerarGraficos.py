import matplotlib.pyplot as plt
import sys
import numpy as np
import tkinter as tk
from tkinter import filedialog

def zoom_factory(ax,base_scale = 2.):       #FUNCAO QUE DA ZOOM, AINDA NAO USO PRA NADA PQ ACHEI RUIM MAS TO DEIXANDO AQUI
    def zoom_fun(event):
        # get the current x and y limits
        cur_xlim = ax.get_xlim()
        cur_ylim = ax.get_ylim()
        cur_xrange = (cur_xlim[1] - cur_xlim[0])*.5
        cur_yrange = (cur_ylim[1] - cur_ylim[0])*.5
        xdata = event.xdata # get event x location
        ydata = event.ydata # get event y location
        if event.button == 'up':
            # deal with zoom in
            scale_factor = 1/base_scale
        elif event.button == 'down':
            # deal with zoom out
            scale_factor = base_scale
        else:
            # deal with something that should never happen
            scale_factor = 1
            print (event.button)
        # set new limits
        ax.set_xlim([xdata - cur_xrange*scale_factor,
                     xdata + cur_xrange*scale_factor])
        ax.set_ylim([ydata - cur_yrange*scale_factor,
                     ydata + cur_yrange*scale_factor])
        plt.draw() # force re-draw

    fig = ax.get_figure() # get the figure of interest
    # attach the call back
    fig.canvas.mpl_connect('scroll_event',zoom_fun)

    #return the function
    return zoom_fun


root = tk.Tk()  #ISSO AQUI É PRA ABRIR A JANELINHA DE ESCOLHER ARQUIVO
root.withdraw() #

if len(sys.argv) > 1:       #PEGA NOME DO ARQUIVO
    arquivo = sys.argv[1] #nome do arquivo
else:
    #arquivo = "resposta.tab"
    arquivo = filedialog.askopenfilename()  #ABRE JANELINHA DE ESCOLHER ARQUIVO
    
tempo = []
resposta = []
count = 0
valores = []

with open(arquivo, 'r') as fp:      #SALVA DADOS DO ARQUIVO NA LISTA VALORES, E NOME DAS VARIAVEIS NA LISTA VARIAVEIS
    for linha in fp.readlines():
        if count == 0:
            variaveis = linha.strip().split(' ')
        if count == 2:
            passo = float(linha.strip().split(' ')[0])
        count = count + 1
        if count > 1:
            valores.append(linha.strip().split(' '))

if len(sys.argv) > 2:               #PEGA PASSO TOTAL DE PONTOS PARA PLOTAR
    pontos = int(sys.argv[3]) #pontos para plotar
else:
    pontos = len(valores)-1

tempoIndice = variaveis.index('t')#EIXOX
valores = np.array(valores, dtype=float)#TRANSFORMA VALORES NOME PARA VALORES FLOAT

print('Variaveis: ' + str(variaveis))
print('Plotando Variaveis...')
plt.figure(figsize=(12,6))

for i in variaveis: #PLOTA AS VARIAVEIS TODAS MENOS 't'
    if i != 't':
        plt.plot(valores[:pontos,tempoIndice], valores[:pontos,variaveis.index(i)])

plt.legend(variaveis[1:])       #COLOCA LEGENDA NAS LINHAS
    
axes = plt.gca()                #OBJETO EIXO
axes.set_xlim([0,np.max(valores[:pontos,tempoIndice])])     #AJUSTA O EIXO X
    
plt.title('Resposta')
plt.ylabel('Amplitude')
plt.xlabel('Tempo (s)')

#scale = 1.1     #NIVEL DE ZOOM COM MOUSE
#f = zoom_factory(axes,base_scale = scale)   #CHAMA FUNCAO DE ZOOM

plt.show()
#plt.savefig('%sxtempo.png'%arquivo)   #SALVA IMAGEM
