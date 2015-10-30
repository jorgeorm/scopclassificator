#!/bin/bash
# Proper header for a Bash script.
export R_LIBS_USER=$1

case "$2" in
    -rcpp.cxx )
        Rscript -e "Rcpp:::CxxFlags()"
        ;;
    -rcpp.ld )
        Rscript -e "Rcpp:::LdFlags()"
        ;;
    -rinside.cxx )
        Rscript -e "RInside:::CxxFlags()"
        ;;
    -rinside.ld )
        Rscript -e "RInside:::LdFlags()"
        ;;
esac
