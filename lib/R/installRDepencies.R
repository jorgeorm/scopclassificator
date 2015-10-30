initial.dir <- getwd()

installed <- rownames(installed.packages(lib.loc = initial.dir))
.libPaths( c( .libPaths(), initial.dir) )

if(! ("RInside" %in% installed)){
    install.packages ("RInside", lib=initial.dir)
}

if(! ("Rserve" %in% installed)){
    install.packages ("Rserve", lib=initial.dir)
}

if(! ("plotly" %in% installed)){
   install.packages("viridis") # dependency
   install.packages("devtools")
   devtools::install_github("ropensci/plotly")
}


Sys.setenv("plotly_username" = "scopclassificator")
Sys.setenv("plotly_api_key" = "sg6xi2ozow")

library("plotly")
library("viridis")

data <- matrix(
    c(2, 4, 3, 1, 5, 7, 3, 1, 5),
    nrow=3,
    byrow=TRUE)

graphicUrl <- plot_ly(
    z = data,
    colors=viridis(256),
    type = "heatmap",
    filename="r-docs/basic-heatmap");

print(graphicUrl)
