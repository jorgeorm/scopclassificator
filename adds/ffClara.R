
#library("ffbase")
#library("ff")
#library("cluster")

#options(
#    ffbatchbytes=536870912,
#    ffmaxbytes=786432000,
#    fftempdir="/datos_ntfs/tmp")

#print( getOption("ffbatchbytes") )
#print( getOption("ffmaxbytes") )
#print( getOption("fftempdir") )
#dset_ff<-read.csv.ffdf(file="dset_d1ak7__.csv", sep=";", dec=".", header=FALSE)
#options(ffmaxbytes = 3758096384, ffbatchbytes = 134217728)
#clust_ff<-ffClara( x=dset_ff, k=50, samples = 25, sampsize = 300 )

cosSimil<-function( vec1, vec2 ){

	cossTeta <- 0;

	if( is.list( vec1 ) ){
		vec1 <- unlist( vec1 );
	}

	if( is.list( vec2 ) ){
		vec2 <- unlist( vec2 );
	}

	if(is.vector( vec1 ) && is.vector( vec2 ) ) {
		cossTeta <- crossprod( vec1, vec2) / ( sqrt( sum( vec1^2 ) ) * sqrt( sum( vec2^2 ) ) );
		return ( cossTeta );
    }else{
		print("Solo se tiene para vectores");
	}

}

disClust<-function( medoides, ffDataset, metric="euclidean" ){
  numMeds=nrow(medoides)
  numObjs=nrow(ffDataset)
  
  
  
  if( !(typeof(metric) == "character" ) ){
    message("metric must be a character type indicating if dissimilarity function will be euclidean or manhattan distance messure")
    return()
  }
  
  idMedoid=1
  
  #Objetos ff
  ff_diffMat<-ff(0, dim=c(numObjs, ncol(ffDataset)))
  ff_disObj<-ff(0, length=numObjs)
  ff_disMat<-ff(0, dim=c(numObjs, numMeds))
  
  
  #Recorro los medoides para evaluar la disimilitud medida como distancias (Euclideana y manhatan)
  for( iMed in 1:numMeds ){
    #cat(paste("Empece con el objeto de referencia: ", idMedoid, "\n"))
    
    objRef<-unlist(medoides[idMedoid, ])
    #Evaluo la funci´on dree distancia sobre el conjunto de datos ff
    if( metric == "euclidean" ){
      ffrowapply(ff_diffMat[i1:i2, ]<-(objRef-unlist(ffDataset[i1:i2, ]))^2, X=ffDataset)
      ff_sumDif<-ffrowapply(rowSums(ff_diffMat[i1:i2,]), X=ff_diffMat,RETURN=TRUE, FF_RETURN=TRUE, CFUN="list")
      ff_sumDif<-unlist(ff_sumDif)
      ffvecapply(ff_disObj[i1:i2]<-sqrt(ff_sumDif[i1:i2]), X=ff_sumDif)
      ffvecapply(ff_disMat[i1:i2, iMed]<-ff_disObj[i1:i2], X=ff_disObj)
      
      
      
    }else if( metric == "manhattan" ){

      ffrowapply(ff_diffMat[i1:i2, ]<-abs(objRef-unlist(ffDataset[i1:i2, ])), X=ffDataset)
      ff_sumDif<-ffrowapply(rowSums(ff_diffMat[i1:i2,]), X=ff_diffMat,RETURN=TRUE, FF_RETURN=TRUE, CFUN="list")
      ff_sumDif<-unlist(ff_sumDif)
      ffvecapply(ff_disMat[i1:i2, iMed]<-ff_sumDif[i1:i2], X=ff_sumDif)
    }
    
    delete(ff_sumDif); rm(ff_sumDif)
    idMedoid=idMedoid+1
  }

  message("\tEvaluó disimilitudes")
  return(ff_disMat)
}

costoClust <- function(medoides, ffDataset){
	
  numMeds=nrow(medoides)
  numObjs=nrow(ffDataset)
  
	costo <- 0;
	for( i in 1:numObjs ){
		Objeto_i <- ffDataset[ i, ]

		Objeto_j <- 0;
		minDist <- Inf;
		idMedoide <- -1
		#Debo obtener medoide para el Objeto_i y asignarlo como Objeto_j
		for( j in 1:nrow( claraMedoids ) ){

			distancia <- distancias[i, j];

			if(minDist > distancia){
				minDist <- distancia;
				Objeto_j <- claraMedoids[ j, ];
				idMedoide <- j;

			}

	 	}
		#Sumo las dissimilutudes
		costo_tmp <- (1 - cosSimil( Objeto_j, Objeto_i ) );
		# cat("\tAl objeto_");
		# cat(i);
		# cat(" le corresponde el medoide_");
		# cat(idMedoide);
		# cat(" con costo ");
		# cat(costo_tmp);
		# cat("\n")
		costo <- costo + costo_tmp;
	}

	costo <- costo / nrow( ffDataset );


	return ( costo );
}

ffBCLARA <- function(x, k, metric = "euclidean", stand = FALSE, samples = 10,
      sampsize = min(nrow(x), 40 + 2 * k), trace = 0, medoids.x = TRUE,
      keep.data = medoids.x, rngR = FALSE, pamLike = FALSE){

	#Determino tamaño del dataset
    tamDset<-nrow(x);
    #Determino el tamaño de los lotes
    tamLote<-sampsize*sampsize;
    #Número de lotes a procesar
    numLotes<-ceiling(tamDset/tamLote);

    #Lote que se va a procesar
    inicioLote=1
    finLote=tamLote

    datosPreagrupados<-NULL;
    # Ejecuto el muestreo
    for( i in 1:numLotes ){
    	
    	finLote=tamLote*i

    	if(finLote > tamDset){
    		finLote=tamDset
    	}

    	cat(paste("Empecé con lote ", i, " de ", numLotes, "\n"));
    	
		#Obtengo el lote
    	lote<-x[inicioLote:finLote, ];   	
		cat("\tObtuve el lote\n");

		if(nrow(lote)<=k){

			datosPreagrupados <- rbind(datosPreagrupados, lote);
			cat( "\tNúmero de elementos en lote insuficientes, se pegaron al conjunto\n" );
			next
		}else{

			#cat( "\tEmpecé con clustering\n" );
			tPart <- system.time( 
                claraClust <- clara(
                    x=lote, 
                    k=k, 
                    metric=metric, 
                    stand=stand, 
                    samples=samples, 
                    sampsize= min( sampsize, nrow(lote) ), 
                    trace=trace,
	                medoids.x = medoids.x, 
                    keep.data = keep.data, 
                    rngR = rngR, 
                    pamLike = pamLike) );
			cat(paste("\tClustering terminado en: ", tPart[3],"\n"));
			
	        datosPreagrupados <- rbind( datosPreagrupados, claraClust$medoids );
			#cat( "\tConcatené la salida del lote\n" );
			inicioLote=finLote+1
			
			next

		}
    }

    return( clara(x=datosPreagrupados, k=k, metric=metric, stand=stand, samples=samples, sampsize=sampsize, trace=trace,
                medoids.x = medoids.x, keep.data = keep.data, rngR = rngR, pamLike = pamLike) );
}

ffClara <- function(x, k, metric = "euclidean", stand = FALSE, samples = 5,
      sampsize = min(nrow(x), 40 + 2 * k), trace = 0, medoids.x = TRUE,
      keep.data = medoids.x, rngR = FALSE, pamLike = FALSE){

    #Instancio variables necesarias
    numObjs = nrow(x)
    objective <- Inf
    bestMedoids <- NULL
    bestSample <- NULL
    bestSampleIds <- 1:sampsize
    bestDiss <- ff(1:numObjs)
    bestClust<-ff(1:numObjs)
    
    #Pongo semilla basada en la hora del sistema
    set.seed( as.integer( ( as.double(Sys.time() ) * 1000+Sys.getpid() ) %% 2^31 ) )
    for ( iSample in 1:samples ){
      message(paste(c("Empecé con muestra ", iSample)))
      #Saco una muestra
      sample_ff<-bigsample( 1:nrow(x), size=sampsize )
      dataSamp <- x[ sample_ff, ]
      
      pamClust <- pam( x=dataSamp, k=k, metric=metric )
      message("\tCalculó clustering para muestra")
      
      ff_disMat <- disClust( medoides=pamClust$medoids, ffDataset=x, metric=metric )
      
      avgDiss = 0
      
      clustering<-ffrowapply(apply(ff_disMat[i1:i2,], MARGIN=1, FUN=which.min), X=ff_disMat, RETURN=TRUE, CFUN="list")
      clustering<-unlist(clustering)
      clustering<-as.ff(clustering)
      message("Hizo clustering")
      
      clusteringDiss<-ffrowapply(apply(ff_disMat[i1:i2,], MARGIN=1, FUN=min), X=ff_disMat, RETURN=TRUE, CFUN="list")
      clusteringDiss<-unlist(clusteringDiss)
      avgDiss = sum(clusteringDiss)
      avgDiss = avgDiss/(numObjs-k)
      message("\tCalculó promedio dismilitud")
      
      message(paste("Dissimilarity: ", avgDiss))
      message(paste("Objective: ", objective))
      
      clusteringDiss<-as.ff(clusteringDiss)
      
      if(avgDiss < objective){
        objective = avgDiss
#         for(idSamp in 1:sampsize){
#           bestSample[idSamp , ] <- unlist(dataSamp[idSamp , ])
#         }
#         for(idSamp in 1:nrow(bestSample)){
#           bestSample[ idSamp, ] <- dataSamp[ idSamp, ]
#         }
        bestSamplea<-dataSamp
        bestSampleIds[] <- sample_ff[]
#         for(idMed in 1:k){
#           bestMedoids[ idMed, ] <- pamClust$medoids[ idMed, ]          
#         }
        bestMedoids<-pamClust$medoids
        ffvecapply(bestClust[i1:i2]<-clustering[i1:i2], X=clustering)
        ffvecapply(bestDiss[i1:i2]<-clusteringDiss[i1:i2], X=clusteringDiss)
        message("Copió optimo local")
      }
      
      ##Borro lo que no se usa
      delete(ff_disMat); rm(ff_disMat)
      delete(clustering); rm(clustering)
      delete(clusteringDiss); rm(clusteringDiss)
    }
    
    claraObject<-list(bestSample, bestSampleIds, bestMedoids, objective, bestClust, bestDiss)
    names(claraObject)<-c("sample", "sample_ids", "medoids", "objective", "clustering", "dissimilarity")
    return(claraObject)
}
