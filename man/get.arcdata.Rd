\name{get.arcdata}
\alias{get.arcdata}


\title{Imports the contents of an ARC file into R}
\description{
Reads the  contents of an arcs definition file and imports it into R.
}


\usage{get.arcdata(infodir, coverage, filename)}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This function returns a list in which every item is a list with
the next fields:

\item{Arc ID}{A number that identifies this arc.}

\item{Arc User ID}{I don't know what this exactly means. Please, remind me to fix this.}

\item{From Node}{The node where the arc begins.}

\item{To Node}{The node where the arc finishes finishes.}

\item{Left Polygon}{The number of the polygon that is to the left of the arc.}

\item{Right Polygon}{The number of the polygon that is to the right of the arc.}

\item{Number of vertices}

\item{Vertices list}{An array with all the coordinates (x,y) of the vertices.}

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}
