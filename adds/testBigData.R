x <- data.frame(log=rep(c(FALSE, TRUE), length.out=26), int=1:26, dbl=1:26 + 0.1,
                        fac=factor(letters), ord=ordered(LETTERS), dct=Sys.time()+1:26,
                        dat=seq(as.Date("1910/1/1"), length.out=26, by=1))
    x <- x[c(13:1, 13:1),]
    csvfile <- tempPathFile(path=getOption("fftempdir"), extension="csv")
    write.csv(x, file=csvfile, row.names=FALSE)

    y <- read.csv(file=csvfile, header=TRUE)
    y
    cat("Read csv with header\n")
    ffx <- read.csv.ffdf(file=csvfile, header=TRUE)
    ffx
    sapply(ffx[,], class)

    message("NOTE that read.table fails for ordered factors, this is fixed in read.table.ffdf")
    try(read.csv(file=csvfile, header=TRUE, colClasses=c(ord="ordered")))
    # could fix this with the following two lines (Gabor Grothendieck) but does not know what bad side-effects this could have
     #setOldClass("ordered")
     #setAs("character", "ordered", function(from) ordered(from))
    y <- read.csv(file=csvfile, header=TRUE, colClasses=c(dct="POSIXct", dat="Date"))  # "ordered" gives an error
    y

    ffx <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered", dct="POSIXct", dat="Date"))
    ffx
    sapply(ffx[,], class)

    message("NOTE that reading in chunks can change the sequence of levels and thus the coding")
    message("(Sorting levels during chunked reading can be too expensive)")
    ffx <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered",
                                dct="POSIXct", dat="Date"), first.rows=6, next.rows=10,
                                VERBOSE=TRUE)
    y <- ffx$fac[]
    print(levels(y))
    data.frame(values=as.character(y), codes=as.integer(y))

    message("If we don't know the levels we can sort then after reading")
    message("(Will rewrite all factor codes)")
    message("NOTE that you MUST assign the return value of sortLevels()")
    ffx <- sortLevels(ffx)
    y <- ffx$fac[]
    print(levels(y))
    data.frame(values=as.character(y), codes=as.integer(y))

    message("If we KNOW the levels we can fix levels upfront")
    ffx <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered", dct="POSIXct", dat="Date"),
                                first.rows=6, next.rows=10, levels=list(fac=letters, ord=LETTERS))
    y <- ffx$fac[]
    print(levels(y))
    data.frame(values=as.character(y), codes=as.integer(y))

    message("Or we inspect a sufficiently large chunk of data and use those")
    ffx1 <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered",
                                dct="POSIXct", dat="Date"), nrows=13)
    ffx <- read.csv.ffdf(x=ffx1, file=csvfile, header=FALSE, skip=1+nrow(ffx1),
                                VERBOSE=TRUE)

    message("We can check for unexpected factor levels, say we only allowed a:l")
    ffx <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered", dct="POSIXct",
                                dat="Date"), levels=list(fac=letters[1:12], ord=LETTERS[1:12]),
                                appendLevels=FALSE)
    sapply(colnames(ffx), function(i)sum(is.na(ffx[[i]][])))

    message("We can fine-tune the creation of the ffdf:")
    message("- let's create the ff files outside of fftempdir")
    message("- let's reduce required disk space and thus file.system cache RAM")
    vmode(ffx)
    message("By default we had record size")
    sum(.ffbytes[vmode(ffx)])

    ffy <- read.csv.ffdf(file=csvfile, header=TRUE, colClasses=c(ord="ordered", dct="POSIXct", dat="Date")
    , asffdf_args=list(
        vmode = c(log="boolean", int="byte", dbl="single", fac="nibble", ord="nibble", dct="single", dat="single")
      , col_args=list(pattern = "./csv")  # create in getwd() with prefix csv
      )
    )
    vmode(ffy)
    message("This recordsize is more than 50% reduced")
    sum(.ffbytes[vmode(ffy)])

    message("Don't forget to wrap-up files that are not in fftempdir")
    delete(ffy); rm(ffy)
    message("It's a good habit to also wrap-up temporary stuff (or at least know how this is done)")
    rm(ffx); gc()

    fwffile <- tempfile()

    cat(file=fwffile, "123456", "987654", sep="\n")
    x <- read.fwf(fwffile, widths=c(1,2,3))    #> 1 23 456 \ 9 87 654
    y <- read.table.ffdf(file=fwffile, FUN="read.fwf", widths=c(1,2,3))
    stopifnot(identical(x, y[,]))
    x <- read.fwf(fwffile, widths=c(1,-2,3))   #> 1 456 \ 9 654
    y <- read.table.ffdf(file=fwffile, FUN="read.fwf", widths=c(1,-2,3))
    stopifnot(identical(x, y[,]))
    unlink(fwffile)

    cat(file=fwffile, "123", "987654", sep="\n")
    x <- read.fwf(fwffile, widths=c(1,0, 2,3))    #> 1 NA 23 NA \ 9 NA 87 654
    y <- read.table.ffdf(file=fwffile, FUN="read.fwf", widths=c(1,0, 2,3))
    stopifnot(identical(x, y[,]))
    unlink(fwffile)

    cat(file=fwffile, "123456", "987654", sep="\n")
    x <- read.fwf(fwffile, widths=list(c(1,0, 2,3), c(2,2,2))) #> 1 NA 23 456 98 76 54
    y <- read.table.ffdf(file=fwffile, FUN="read.fwf", widths=list(c(1,0, 2,3), c(2,2,2)))
    stopifnot(identical(x, y[,]))
    unlink(fwffile)



    unlink(csvfile)
