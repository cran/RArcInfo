\name{get.cntdata}
\alias{get.cntdata}

\title{Imports the contents of a CNT file into R}
\description{
Reads the  contents of a polygon centroid information file and imports it 
into R. 
}

\usage{get.labdata(infodir, coverage, filename="cnt.adf")}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This functions returns a list in which every item is also a list with
the next fields:

\item{Polygon ID}{The polygon itself.}

\item{Coordinates}{An array with the coordinates of the centroid.}

\item{Number of Labels}{The number of labels this polygon has.}

\item{Labels}{An array with the number of the label identifiers related to this polygon.} 

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}
\keyword{file}
