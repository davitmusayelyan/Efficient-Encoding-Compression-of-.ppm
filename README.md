# Better-Encoding-Compression-of-.ppm
File format P152 offers efficient storing of .ppm files through a better encoding and image-compression option with the index-color technique using the ‘k-means method.’

## What is PPM?
PPM (Portable Pix Map) is an image file format which is essentially a text file containing the dimensions of the image and the rgb colors of the pixels. This format is highly inefficient because it lists every single pixel one by one, and contains almost no other information about the image.

## What does P152 have?
P152 is an original file format that improves on PPM by:\
**(i)** avoiding the full enumeration of the pixels through a lossless compression of the file, using **run-length encoding**\
**(ii)** offering an option of a lossy compression of the ppm file through **color-indexing** with the k-meaans algorithm\
**(iii)** containing information about the image other than the pixels such as the date and time of the creation, whether the image is grayscale, run-length encoded, or has a color-table

## Color-indexing & the K-means Algorithm
When converting from a ppm file to a p152 file, the user has the option of using a color-table with the desired number of colors to further compress the image. Although the color pallette of the image is reduced, it also significantly decreases the size of the image, saving storage space. To do this, the ppm-to-p152 converter uses k-means clustering to identify the colors that should be in the color table to best resemble the image. 

If the user wants the image to be saved with only 10 colors, the program starts off by assigning 10 different colors to the color table defining 10 clusters. Then, each pixel on the image is assigned to the color (cluster) in the color table that it is closest to, after which the mean of the cluster is recalculated and that color-table color is updated. The process ends when there is no more improvement in the chosen colors in the color table. 

The picture on the left is a ppm image which has been converted to a p152 file format with run-length encoding and a color-table with 20 colors. The picture on the right is the color-table compressed version of th same image. The P152 format of this image is **65%** smaller in its size than the .ppm file due to the compression.


![PPM vs Compressed](https://user-images.githubusercontent.com/52777539/89043008-ea514980-d358-11ea-96e9-1b74df2993ff.png)
