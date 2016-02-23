#include <stdlib.h>
#include <string.h>
#include "libpnm.h"

int x,y;                 //loop variables
int color;	             
float slope;			 //slope of the image (y/x) used to calculate diagonal
float colorChange;       //change of color per line	
float curColor = 255.0;	 //current color of each line
char file_name[1024];				
char *colors[] = {"R", "G", "B"};
							
struct PBM_Image pbmImage;	
struct PGM_Image pgmImage;
struct PPM_Image ppmImage;

/***********************************************
This functions draws the PBM image in question 1
************************************************/
int DrawPBMImage(int imageWidth, int imageHeight){
	slope = (float)(imageHeight) / (float)(imageWidth);
		
	//First, fill the image with black
	for  (x = 0; x < imageWidth; x++)			
		for (y = 0; y < imageHeight; y++)
			pbmImage.image[y][x] = 1;	
	
	//Then fill the white rectangle in the middle
	for  (x = imageWidth/4; x < (imageWidth*3/4); x++)	
		for (y = imageHeight/4; y < (imageHeight*3/4); y++)	
			pbmImage.image[y][x] = 0;		
	
	//draw the diagonals of white rectangle
	if (slope <= 1.0){		                              					
		for (x = imageWidth/4; x <= (imageWidth/2); x++)   //loop through x if width is greater	
		{
			y = slope * x;                                 //corresponding y of x
			
			pbmImage.image[  y][  x] = 1;				   //4 segments of diagonal
			pbmImage.image[imageHeight-y][  x] = 1;
			pbmImage.image[imageHeight-y][imageWidth-x] = 1;			
			pbmImage.image[  y][imageWidth-x] = 1;			
		}
	}
	else{
		for (y = imageHeight/4; y <= (imageHeight/2); y++)	//loop through y if height is greater			
		{
			x =  y/slope;                                   //corresponding x of y 
			
			pbmImage.image[  y][  x] = 1;                   //4 segments of diagonal
			pbmImage.image[imageHeight-y][  x] = 1;
			pbmImage.image[imageHeight-y][imageWidth-x] = 1;			
			pbmImage.image[  y][imageWidth-x] = 1;			
		}
	}	
	return 0;
}

/**********************************************
This function draws the PGM image in question 2
***********************************************/
int DrawPGMImage(int imageWidth, int imageHeight){
	int i;         //loop variable
	slope = (float)(imageHeight) / (float)(imageWidth);   //slope to calculate x & y of diagonal

	//First, fill the image with black
	for  (x = 0; x < imageWidth; x++)			
		for (y = 0; y < imageHeight; y++)
			pgmImage.image[y][x] = 0;	
	
	//Then fill the white rectangle in the middle
	for  (x = imageWidth/4; x < (imageWidth*3/4); x++)	
		for (y = imageHeight/4; y < (imageHeight*3/4); y++)	
			pgmImage.image[y][x] = 255;

	if(slope <= 1.0){                                       //loop through x if width is greater
		colorChange = 255.0/(imageWidth/4.0);               //how much color changes per line

		for(x = imageWidth/4; x<= (imageWidth/2); x++){     //loop through x
			y = x * slope;

			for(i = y; i <= imageHeight-y; i++){            //draw the top and bottom triangles
				pgmImage.image[i][x] = (unsigned char)curColor;   //they are symmetric
				pgmImage.image[i][imageWidth-x] = (unsigned char)curColor;
			}

			for(i = x; i <= imageWidth-x; i++){             //draw the left and right triangle
				pgmImage.image[y][i] = (unsigned char)curColor;   //they are symmetric
				pgmImage.image[imageHeight-y][i] = (unsigned char)curColor;
			}

			curColor = curColor - colorChange;
			if (curColor<0.0)
				curColor=0.0;
		}
	}

	else{                                                   //loop through y if height is greater
		colorChange = 255.0/(imageHeight/4.0);              //how much color changes per line

		for(y = imageHeight/4; y<= (imageHeight/2); y++){   //loop through y
			x = y / slope;

			for(i = y; i <= imageHeight-y; i++){            //draw the top and bottom triangles 
				pgmImage.image[i][x] = (unsigned char)curColor;  //they are symmetric
				pgmImage.image[i][imageWidth-x] = (unsigned char)curColor;
			}

			for(i = x; i <= imageWidth-x; i++){             //draw the left and right triangles
				pgmImage.image[y][i] = (unsigned char)curColor;  //they are symmetric
				pgmImage.image[imageHeight-y][i] = (unsigned char)curColor;
			}

			curColor = curColor - colorChange;              //the current color
			if (curColor<0.0)                               //in case of getting negative value
				curColor=0.0;
		}
	}	
	return 0;
}

/**********************************************
This function draws the PPM image in question 3
***********************************************/
int DrawPPMImage(int imageWidth, int imageHeight){
	curColor = 0.0;  

	//First, fill the image with white
	for  (x = 0; x < imageWidth; x++)			
		for (y = 0; y < imageHeight; y++)
			for (color = RED; color <= BLUE; color++)
			ppmImage.image[y][x][color] = 255;

	//the upper part
	for (y = 0; y < (imageHeight/2); y++){
		curColor = 255.0/(imageHeight/2-1) * y;         //the current color
		//the middle part
		for (x = 0; x < (imageWidth/3); x++){
			ppmImage.image[y][x][GREEN] = (unsigned char)curColor;  //increase GREEN gradually
			ppmImage.image[y][x][BLUE] = (unsigned char)curColor;   //increase BLUE gradually
			
		}
		//the middle part
		for (x = imageWidth/3; x < (2*imageWidth/3); x++){
			ppmImage.image[y][x][RED] = (unsigned char)(255 - curColor);  //decrease RED gradually
			ppmImage.image[y][x][BLUE] = (unsigned char)(255 - curColor); //decrease BLUE gradually
		} 
		//the right part
		for (x = (2*imageWidth/3); x < imageWidth; x++){
			ppmImage.image[y][x][GREEN] = (unsigned char)curColor;   //increase GREEN gradually
			ppmImage.image[y][x][RED] = (unsigned char)curColor;     //increase RED gradually
		}

		
	}

	//the lower part
	curColor = 0.0; int i = 0;
	for(y = (imageHeight/2); y < imageHeight; y++)
	{
		curColor = 255.0/(imageHeight/2-1) * i; 
		//the left part
		for (x = 0; x < (imageWidth/2); x++){
			//gray is RED = GREEN = BLUE, increase them gradually
			ppmImage.image[y][x][RED] = (unsigned char)curColor;
			ppmImage.image[y][x][GREEN] = (unsigned char)curColor;
			ppmImage.image[y][x][BLUE] = (unsigned char)curColor;
		}
		//the right part
		for (x = (imageWidth/2); x < imageWidth; x++){
			//gray is RED = GREEN = BLUE, decrease them gradually
			ppmImage.image[y][x][RED] = (unsigned char)(255- curColor);
			ppmImage.image[y][x][GREEN] = (unsigned char)(255 - curColor);
			ppmImage.image[y][x][BLUE] = (unsigned char)(255 - curColor);
		}

		i += 1;
	}

	return 0;
}


int main(int argc, char** argv)
{    
	int w, h;                            //declare width and height of iamge
	bool format;                         //false for ASCII, otherwise raw	
	//char file_name[1024];                //hold the file name of the file which stores the image
		
	if (argc != 6)                       //check the arguments
	{
		printf("Usage: %s image_type image_width image_height output_image_name image_format_code\n", argv[0]);
		exit(EXIT_FAILURE);              //exit if error occurs
	}
	
	if(strcmp(argv[5], "0") == 0)        //check image format. "0" for ASCII, "1" for raw 
		format = false;
	else
		format = true;
	

	if(strcmp(argv[1], "pbm")==0)        //check if the image type is "pbm"
	{	
		w = atoi(argv[2]);		         //store width from argument						
		if ((w < 4) || (w % 4 != 0)){    //width resgrition
			printf("ERROR: Width must be divisible by 4 and greater than or equal to 4\n");
			exit(EXIT_FAILURE);          //exit if error occurs
		}
		
		h = atoi(argv[3]);			     //store height from argument					
		if ((h < 4) || (h % 4 != 0))     //height restriction
		{
			printf("ERROR: Height must be divisible by 4 and greater than or equal to 4\n");
			exit(EXIT_FAILURE);          //exit if error occurs
		}
		
		
		if (create_PBM_Image(&pbmImage, w, h) != 0)	       //create a PBM image	
		{
			printf("ERROR: Fail to create a PBM image\n"); 
			exit(EXIT_FAILURE);                            //exit if error occurs
		}
		
		DrawPBMImage(w, h);                              //draw the required image
		
		strcpy(file_name, argv[4]);	                       //store the filename form argument
		strcat(file_name, ".pbm");                         //append suffix to filename
		
		if (save_PBM_Image(&pbmImage, file_name, format) != 0){	  //save the image
	 		printf("Error: Fail to save a PBM image to file.\n");
	 		exit(EXIT_FAILURE);                                   //exit if error occurs
	 	}
		
		free_PBM_Image(&pbmImage);	                              //free memory						
    } 


    if(strcmp(argv[1], "pgm")==0)        //check if the image type is "pgm"	         
	{	
		w = atoi(argv[2]);	           	 //store width from argument			
		if ((w < 4) || (w % 4 != 0)){    //width restriction
			printf("ERROR: Width must be divisible by 4 and greater than or equal to 4\n");
			exit(EXIT_FAILURE);          //exit if error occurs
		}
		
		h = atoi(argv[3]);			     //store height from argument					
		if ((h < 4) || (h % 4 != 0))     //height restriction
		{
			printf("ERROR: Height must be divisible by 4 and greater than or equal to 4\n");
			exit(EXIT_FAILURE);          //exit if error occurs
		}
		
		
		if (create_PGM_Image(&pgmImage, w, h, 255) != 0)	//create a PGM image	
		{
			printf("ERROR: Fail to create a PGM image\n");
			exit(EXIT_FAILURE);                              //exit if error occurs
		}
		
		DrawPGMImage(w, h);                                //draw the required image 
		
		strcpy(file_name, argv[4]);	                         //store the file name
		strcat(file_name, ".pgm");                           //append suffix to file name
		
		if (save_PGM_Image(&pgmImage, file_name, format) != 0){	  //save image
	 		printf("Error: Fail to save a PGM image to file.\n");
	 		exit(EXIT_FAILURE);                                   //exit if error occurs
	 	}
		
		free_PGM_Image(&pgmImage);		                          //free memory					
    } 


    if(strcmp(argv[1], "ppm")==0)                //check if the image type is "ppm"	
	{	

		w = atoi(argv[2]);				         //store width from argument				
		if ((w < 6) || (w % 6 != 0)){            //width restriction
			printf("ERROR: Width must be divisible by 6 and greater than or equal to 6\n");
			exit(EXIT_FAILURE);                  //exit if error occurs
		}
		
		h = atoi(argv[3]);					     //store height from argument
		if ((h < 4) || (h % 4 != 0))             //height restriction
		{
			printf("ERROR: Height must be divisible by 4 and greater than or equal to 4\n");
			exit(EXIT_FAILURE);                  //exit if error occurs
		}
		
		
		if (create_PPM_Image(&ppmImage, w, h, 255) != 0)   //create a PPM image		
		{
			printf("ERROR: Fail to create a PPM image\n");
			exit(EXIT_FAILURE);                            //exit if error occurs
		}
		
		DrawPPMImage(w, h);                                 //draw the required image

		char name[1024];
		strcpy(name, argv[4]);	                            //store the file name 
		strcat(name, ".ppm");                               //append suffix to file name
		
		if (save_PPM_Image(&ppmImage, name, format) != 0){	 //save PPM image
	 		printf("Error: Fail to save a PPM image to file.\n");   
	 	}


		if (create_PGM_Image(&pgmImage, w, h, 255) != 0)   	 //create a PGM image	
		{
			printf("ERROR: Fail to create a PGM image\n");
			free_PPM_Image(&ppmImage);                       //free memory
			exit(EXIT_FAILURE);                              //exit if error occurs       
		}

		//copies a PPM to 3 gray images using copy_PPM_to_PGM
		for(color = RED; color <= BLUE; color++){
			if(copy_PPM_to_PGM(&ppmImage, &pgmImage, color) != 0)  //check if copy is successful
				printf("Error: Failure in conversion\n");          //print error message if not

			else{                                                  //if copy is successful
				strcpy(file_name, argv[4]);                        //name the file
				strcat(file_name, ".");
				strcat(file_name, colors[color]);
				strcat(file_name, ".pgm");

				if (save_PGM_Image(&pgmImage, file_name, format) != 0){	    //save converted image
	 				printf("Error: Fail to save a conversion image to file.\n");   
	 				//print error message if fail to save iamge
	 			}
			}
		}
		
		free_PPM_Image(&ppmImage);	          //free memory	
		free_PGM_Image(&pgmImage);	          //free memory						
    } 
	
	return 0;
}