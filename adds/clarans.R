library("ffbase");
library("ff");
library("fields");
library("pnmath");

#Maxima distancia en amstrongs
#maxDistance=20

#Modifica el valor de la distancia máxima
#setMaxDist<- function( maxVdist ){
#	maxDistance <- maxVdist
#}

clarans <- function (x, k,numLocal=2, maxNeighbor=ceiling(nrow(x)*0.015) ){

    set.seed( as.integer((as.double(Sys.time())*1000+Sys.getpid()) %% 2^31) );

    # Pongo las variables en sus valores iniciales
   	dsetSize <- nrow(x)
   	bestNode = rep(-1, k)
	minCost = Inf
	distMat <- NULL
	clustAsign = rep(-1, dsetSize)
	
	clustMat <- matrix(, nrow=dsetSize, ncol=2)
	#maxFeatDistance=sqrt(maxDistance*ncol(x))


    for(nlocalMin in 1: numLocal){

    	# Selecciono un nodo arbitrario y coloco el j-vecino en 1
		currentNode <- sample(dsetSize, k)
		medoids <- x[currentNode, ]
		distMat <- rdist(x, medoids)

		# Calculo matriz de distancias entre los medoides y los datos si es la primera vez
		currentCost = 0

		for(jObjectId in 1:dsetSize){
			idClust = which.min(distMat[jObjectId,])
			clustAsign[jObjectId] = idClust
			currentCost = currentCost + distMat[jObjectId, idClust]
		}

		currentCost = currentCost/(dsetSize - k)
		
		jNeighbor = 1

		while( jNeighbor <= maxNeighbor ){
			# Obtengo un vecino alealtorio del nodo actual
			positionReplace = sample(1:k, 1)
			newMedoidId = -1

			# Obtengo un medoide que no haga parte del nodo actual
			repeat{

				newMedoidId = sample(1:dsetSize, 1)

				if( ! (newMedoidId %in% currentNode) ){
					break;
				}
			}
			
			##Calculo el costo diferencial
			oldMedoidId <- currentNode[positionReplace]

			newMedoid <- x[newMedoidId,]
			newColDmat <- rdist(x, newMedoid)
			replacingCost=0
			costoNuevo=0
			asignacionTemporal<-rep(-1, dsetSize)
			asignacionTemporal[newMedoidId]=positionReplace

			#Costo diferencial
			#Asignación Cluster Temporal
			for(jObjectId in 1:dsetSize){
				##No analizo el medoide nuevo
				if(jObjectId == newMedoidId){
					next
				}
				##No analizo medoide viejo
				#if(jObjectId == oldMedoidId){
				#	next
				#}
				##Detecto si Oj pertenece a Om
				if(clustAsign[jObjectId] == positionReplace){
					distsOj<-distMat[jObjectId, ]
					sortedDists<-sort(distsOj, index.return=TRUE)
					j2ObjectId <- sortedDists$ix[2]

					##Más similiar a Oj2 que a Op
					if(newColDmat[jObjectId,1]>=distMat[jObjectId, j2ObjectId]){
						asignacionTemporal[jObjectId]=j2ObjectId
						costoNuevo = costoNuevo+distMat[jObjectId, j2ObjectId]
						#d(Oj, Oj2)-d(Oj, Om)
						replacingCost = replacingCost + distMat[jObjectId, j2ObjectId] - distMat[jObjectId, positionReplace]
					}else if(distMat[jObjectId, j2ObjectId] > newColDmat[jObjectId,1]) {
					## Más similiar a Op que a Oj2
						#d(Oj, Op)-d(Oj, Om)
						asignacionTemporal[jObjectId]=positionReplace
						costoNuevo = costoNuevo+newColDmat[jObjectId,1]

						replacingCost = replacingCost + newColDmat[jObjectId,1] - distMat[jObjectId, positionReplace]
					}
				}else{
					# Más similiar a Oj2 que a Op
					if(newColDmat[jObjectId,1] >= distMat[jObjectId, clustAsign[jObjectId]]){
						#Cjmp = 0
						asignacionTemporal[jObjectId]=clustAsign[jObjectId]
						costoNuevo = costoNuevo+distMat[jObjectId, clustAsign[jObjectId]]
						replacingCost = replacingCost+0
					}else {
					# Más similar a Op que a Oj2
						#d(Oj, Op)-d(Oj, Oj2)
						asignacionTemporal[jObjectId]=positionReplace
						costoNuevo = costoNuevo+newColDmat[jObjectId,1]
						replacingCost = replacingCost + newColDmat[jObjectId] - distMat[jObjectId, clustAsign[jObjectId]]
					}
					
				}
			}

			#Si vecino aleatorio tiene costo negativo
			if( replacingCost < 0 ){
				#cat("Sustitución: \n");
				#print(currentNode);

				currentNode[positionReplace] = newMedoidId;
				
				#TODO: Reemplazar la columna en la matriz de distancia
				distMat[,positionReplace] = newColDmat[,1];

				##TODO: Asignar los clusters al current 
				clustAsign[1:dsetSize] = asignacionTemporal[1:dsetSize];

				currentCost = costoNuevo/(dsetSize - k);
				#print(currentNode);
				#cat(paste("\nCon costo: ", currentCost,"\n"));
				
				jNeighbor = 1;
			} else{
				jNeighbor = jNeighbor+1
			}

		}

		if(minCost > currentCost){
			bestNode[1:k] = currentNode[1:k];
			minCost = currentCost
		}
	
    }

    claransObj = list( bestNode, x[bestNode, ], minCost, clustAsign )
    names(claransObj) = c("idsClust", "medoids", "cost", "clustering")

    return (claransObj)

}
