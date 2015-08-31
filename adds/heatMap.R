#librerias
#library(gplots)

#paramsConfig
size<-768
numInterfalos <- 20
min<-0
maxDist<-20
divColoresDist <- maxDist/numInterfalos
divMarcasDist <- 4
maxAng<-3.1386
divColoresAngs <- 0.31386
divMarcasAmgs <- 0.31386

require(graphics); require(grDevices)
archivos <- list.files("~/Documentos/Proteinas", full=T, pattern=".csv");
for (archivoPDB in archivos){
    # Leo el archivo
    #matrizcsv <- read.csv(file="~/Documentos/Proteinas/3S66_ANGS.csv", head=TRUE, sep=";", dec=",")
    cat(paste("Graficando : "),archivoPDB,"\n")
    matrizcsv <- read.csv(file=archivoPDB, head=TRUE, sep=";", dec=",")

    # Armo la matriz y pongo nombres a las filas
    matriz <- data.matrix(matrizcsv)
    rownames(matriz) <-rownames(matriz, do.NULL= FALSE, prefix="Ca")

    # Determino colores a utilizar
    # hmcol<-colorRampPalette(c("orangered3 ",'darkorange ',"yellow","green4 ","lightblue3"))(16)
    # hmcol<-colorRampPalette(c("orangered3",'yellow ',"green4","cyan","blue "))(numInterfalos)
    hmcol <- topo.colors(numInterfalos)

    # Determino el tipo de imagen a guardars
    # svg("r_plot.svg", width=5, height=5)
    png(paste(gsub(pattern=".csv", replacement="", x=archivoPDB),"png", sep="."), width=size, height=size)
    #png("~/Documentos/Proteinas/3S66_ANGS.png", width=size, height=size)

    #Creo los gr´aficos
    # layout(matrix(1:2,ncol=2), width = c(2,1),height = c(1,1))
    # intervalos <- seq(min,max, length.out=numInterfalos)
    # leyenda <- legend("bottom", fill=hmcol, legend=intervalos)

    #Meto el mapa de Calor
    par(fig=c(0,1,0,1), new=TRUE)
    image(matriz, Rowv=NA, Colv=NA,  col=hmcol,sym=TRUE, margins=c(4,4),axes=FALSE)

if(grep(archivoPDB, 'DIST')){
    numAtomos <- paste("Ca",nrow(matriz), sep="")
    axis(2, at = seq(0,1,1), labels=c("Ca1", numAtomos))
    axis(1, at = seq(0,1,1), labels=c("Ca1", numAtomos))
} else{
    numResiduos <- paste("R",nrow(matriz), sep="")
    axis(2, at = seq(0,1,1), labels=c("R1", numResiduos))
    axis(1, at = seq(0,1,1), labels=c("R1", numResiduos))
}
    # heatmap( matriz, Rowv=NA, Colv = NA,  col = hmcol,
    #                  sym=TRUE, margins=c(4,4),
    #                  revC=FALSE, scale="none", distfun= function (c) as.dist(max-c),
    #                  xlab = "Carbono Alpha", ylab =  "Carbono Alpha")


    #rango <- seq( min, maxAng, divColoresAngs)



    #  image(m, col=hmcol, ylab="", axes=FALSE)
    dev.off()
}
