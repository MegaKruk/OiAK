#include <stdio.h>
#include <stdlib.h>

float a[4] = {11,2,3,4};
float b[4] = {5,16,7,8};
float c[4] = {9,1,2,3};
float d[4] = {4,5,61,7};
float sum[4] ={0,0,0,0};
float wyznacznik=0;

void Wyznacznik()
{   
   
    asm (                                               
        "movups (%0), %%xmm0     \n\t"			//A
        "movups (%1), %%xmm1 \n\t" 				//B
       
        "shufps $0x48 , %%xmm0, %%xmm0 \n\t"	//A'(1, 0, 2, 0)
        "shufps $0xb1 , %%xmm1, %%xmm1 \n\t"	//B'(2, 3, 0, 1)
        "mulps %%xmm0,%%xmm1 \n\t"				//A'*B'
       
        "movups (%0), %%xmm0 \n\t"          	//A     
        "movups (%1), %%xmm2 \n\t"              //B
       
        "shufps $0xb1,%%xmm0,%%xmm0 \n\t"		//A"(2, 3, 0, 1)
        "shufps $0x48,%%xmm2,%%xmm2 \n\t"		//B"(1, 0, 2, 0)
        "mulps  %%xmm0,%%xmm2 \n\t"				//A"*B"
       
        "subps %%xmm1, %%xmm2 \n\t"				//A'B'-A"B"
       
        "movups (%2), %%xmm0     \n\t"        	//C
        "movups (%3), %%xmm1 \n\t"       		//D
       
        "shufps $0x16 , %%xmm0, %%xmm0 \n\t"	//C'(0, 1, 1, 2)	
        "shufps $0xef , %%xmm1, %%xmm1 \n\t"	//D'(3, 2, 3, 3)
        "mulps %%xmm0,%%xmm1 \n\t"  			//C'*D'

        "movups (%2), %%xmm0 \n\t"              //C   
        "movups (%3), %%xmm3 \n\t"              //D    
       
        "shufps $0xef,%%xmm0 ,%%xmm0 \n\t"   	//C"(3, 2, 3, 3)
        "shufps $0x16, %%xmm3, %%xmm3 \n\t"     //D"(0, 1, 1, 2)       
        "mulps  %%xmm0,%%xmm3 \n\t"             //C"*D"
       
        "subps %%xmm1, %%xmm3 \n\t"             //C'D'-C"D" 

        "mulps %%xmm2,%%xmm3 \n\t" 				//(A'B'-A"B")(C'D'-C"D")
   

//Kolejne wektory posiadaja tylko po 2 wartosci w sobie, dlatego
//skorzystam z mozliwosci sprowadzenia ostatnich dwoch wartosci wektora
//do wartosci 0
//(a2*b3-a3*b2)*(c0*d1-c1*d0) +
//(a3*b1-a1*b3)*(c0*d2-c2*d0) +
//(a0*b0-a0*b0)*(c0*d0-c0*d0) +
//(a0*b0-a0*b0)*(c0*d0-c0*d0)
//zabieg ten spowoduje ze w wektorach beda nastepujace wartosci (x,y,0,0)
        "movups (%0), %%xmm0     \n\t"      	//A        
        "movups (%1), %%xmm2 \n\t"              //B
	    "shufps $0x0e , %%xmm0, %%xmm0  \n\t"   //A'(0, 0, 3, 2) 
        "shufps $0x07 , %%xmm2, %%xmm2 \n\t"    //B'(0, 0, 1, 2)       
       
        "mulps %%xmm0, %%xmm2 \n\t"             //A'*B'  
       
        "movups (%0), %%xmm0     \n\t"          //A                     
        "movups (%1), %%xmm4     \n\t"			//B
		"shufps $0x07 , %%xmm0, %%xmm0 \n\t" 	//A"(0, 0, 1, 3)
        "shufps $0x0e , %%xmm4, %%xmm4 \n\t"    //B"(0, 0, 3, 2)      
       
        "mulps %%xmm0,%%xmm4 \n\t"              //A"*B"  
       
        "subps %%xmm4, %%xmm2 \n\t"             //A'B'-A"B"    
       
        "movups (%2), %%xmm0 \n\t"             	//C          
        "movups (%3), %%xmm4 \n\t"				//D
	    "shufps $0x00 , %%xmm0, %%xmm0 \n\t"   	//C'(0, 0, 0, 0)
        "shufps $0x09 , %%xmm4, %%xmm4 \n\t"    //D'(0, 0, 2, 1)      
       
        "mulps %%xmm0, %%xmm4 \n\t"             //(A'B'-A"B")(C'D'-C"D")
       
        "movups (%2), %%xmm0 \n\t"              //C       
        "movups (%3), %%xmm5 \n\t"				//D
		"shufps $0x09 , %%xmm0, %%xmm0 \n\t"  	//C"
        "shufps $0x00 , %%xmm5, %%xmm5 \n\t"    //D"      
       
        "mulps %%xmm5,%%xmm0 \n\t"              //C"*D"  
       
        "subps %%xmm4, %%xmm0 \n\t"             //C'D'-C"D"    
       
        "mulps %%xmm0,%%xmm2 \n\t"              //(A'B'-A"B")(C'D'-C"D")
      
        "subps %%xmm2,%%xmm3 \n\t"              
        "movups %%xmm3,(%4) \n\t"
        :
        : "r" (a),"r" (b) , "r" (c), "r"(d), "r" (sum)
    );
   
    for(int i=0; i<4; i++){
        wyznacznik+=sum[i];
		printf("%f \n",sum[i]);
		}
//		printf("%f \n",wyznacznik);

};


int main()
{
    Wyznacznik();
    printf("Wyznacznik: \n");
    printf("%f \n",wyznacznik);
    return 0;
}
