\name{get.tabledata}
\alias{get.tabledata}

\title{Imports the contents of a table file into R}
\description{
Reads the  contents of a  table file and imports it into R.
}

\usage{get.arcdata(infodir, coverage, filename)}

\arguments{
\item{infodir}{Info directory where there is a file called arc.dat}
\item{coverage}{The name of the coverage we want to work with}
\item{filename}{The name of the file in the coverage directory that
stores the data}
}

\value{
This functions returns a list in which every item is a list with
the same number of elements that the number of fields.  The data
in the first field of the table is stored in the first elements
of the "sublists". Notice that the type of data can change
within the sublists.

For example, image we have a table with two fields: ID and
Name. The fist field is ot type integer and the second of type
string. So, the first element in all the sublists will
be an integer and the other of type character.

More information about this kind of data can be found at
\url{http://pages.infinit.net/danmo/e00/docs/v7_bin_cover.html}.
}
