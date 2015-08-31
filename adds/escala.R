#paramsConfig
size<-600
numInterfalos <- 20
numMarcas <- 10
min<-0
maxDist<-20
divColoresDist <- maxDist/numInterfalos
divMarcasDist <- maxDist/numMarcas
maxAng<-1
divColoresAngs <- maxAng/numInterfalos
divMarcasAmgs <-  round(maxAng/numMarcas, digits=2)
require(graphics); require(grDevices)


cat("Graficando la barra de colores\n")

hmcol <- topo.colors(numInterfalos)

png("/home/jorgeorm/Documentos/Proteinas/escalaAngs.png", width=size, height=size)
#png("/home/jorgeorm/Documentos/Proteinas/escalaDist.png", width=size, height=size)

#Creo los gr´aficos
# layout(matrix(1:2,ncol=2), width = c(2,1),height = c(1,1))
# intervalos <- seq(min,max, length.out=numInterfalos)
# leyenda <- legend("bottom", fill=hmcol, legend=intervalos)

#Meto el mapa de Calor
par(fig=c(0,1,0.75,1), new=TRUE)
rango <- seq( min, maxAng, divColoresAngs)
#rango <- seq( min, maxDist, divMarcasDist)


matrizRango <-  as.matrix(rango)

image(matrizRango, col=hmcol, ylab="", axes=FALSE, xlab='Angulo θ (π rad) entre residuos')
#   image(matrizRango, col=hmcol, ylab="", axes=FALSE, xlab='Distancia (Å) entre residuos')

# axis(2)
#axis(1, at=seq(0,1,0.2), labels=seq(min, maxDist, divMarcasDist))
axis(1, at=seq(0,1,0.1), labels=seq(min, maxAng, divMarcasAmgs))



#  image(m, col=hmcol, ylab="", axes=FALSE)
dev.off()
