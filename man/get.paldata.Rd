\name{get.paldata}
\alias{get.paldata}

\title{Imports the contents of a PAL file into R}
\description{
Reads the  contents of a polygon definitions file and imports it into R.
}

\usage{get.paldata(infodir, coverage, filename)}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This functions returns a list in which every item is a list with
the next fields:

\item{Polygon ID}{A number that identifies this polygon.}

\item{Minimum Values}{
An array with the minimum values for the X and Y coordinates
of the nodes in the polygon boundary.
}

\item{Maximum Values}{
An array with the maximum values for the X and Y coordinates
of the nodes in the polygon boundary.
}

\item{Number of arcs}

\item{Arcs list}{
 An array with information about the polygons that are in the polygon boundary:
 Arc ID, From Node and Adjadcent Polygon.From Node and Adjadcent Polygon Id.
}

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}
