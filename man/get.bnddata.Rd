\name{get.bnddata}
\alias{get.bnddata}

\title{Imports the contents of a BND file into R}
\description{
Reads the  contents of a BND file and imports it into R. This kind
of files store the bounds of the given coverage (the one we are working
with). That is, the upper and lower bounds for the x and y coordiantes.
}

\usage{get.arcdata(infodir, coverage, filename="bnd.adf")}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}


\value{
A vector with the  x min, y min, x max and y max values.

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}

\keyword{file}
