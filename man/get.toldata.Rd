\name{get.toldata}
\alias{get.toldata}

\title{Imports the contents of a TOL file into R}
\description{
Reads the  contents of a tolerance definition file and imports it into R.
}

\usage{get.arcdata(infodir, coverage, filename="tol.adf")}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This function returns a list in which every item is a list with
the next fields:

\item{Tolerance Type}{A number from 1 to 10 showing the tolerance type.} 

\item{Tolerance Status}

\item{Tolerance Value}

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}

\keyword{file}
