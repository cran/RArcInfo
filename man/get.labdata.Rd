\name{get.labdata}
\alias{get.labdata}

\title{Imports the contents of a LAB file into R}
\description{
Reads the  contents of a polygon labels definition file and imports it 
into R. 
}

\usage{get.labdata(infodir, coverage, filename="lab.adf")}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This functions returns a list in which every item is a list with
the next fields:

\item{Label Value}{The label of the polygon.}

\item{Polygon ID}{The polygon related to the label.}

\item{Coordiantes}{A vector with three pairs of (X,Y) coordinates.}

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}
\keyword{file}
