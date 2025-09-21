/* 
 * lotto_combinations.c
 * --------------------
 * Generate all possible 6-number combinations from a user-supplied set of numbers,
 * filter them by two user-defined constraints:
 *   1) number of even numbers in the combination (X1..X2)
 *   2) sum of the 6 numbers (Y1..Y2)
 *
 * Outputs:
 *   - prints valid combinations
 *   - counts how many combinations were rejected by each filter
 *   - counts how many times each input number appears in valid combinations
 *
 * Notes:
 *   - Implemented in plain C (malloc, qsort, simple combinatorics)
 *   - Input numbers must be unique integers between 1 and 49
 */
#include <stdio.h>
#include <stdlib.h>

int plnumbers(void); // Ask for total count of numbers to input
int zygous(void); //Get lower/upper limit for even numbers
int afroisma(void); // Get lower/upper limit for sum of numbers
void input(int N, int *mat);
int linear_search(int *mat, int number, int N); //Check if number already exists
int compare(const void *elem1, const void *elem2);
int *compinations(int *mat, int X1, int X2, int Y1, int Y2, int N, int allc, int *freq); // Generate and check combinations
int check_one(int *mat, int one, int two, int three, int four, int five, int six, int X1, int X2); //Check "even numbers" condition
int check_two(int *mat, int one, int two, int three, int four, int five, int six, int Y1, int Y2);// Check "sum of numbers" condition
int plhfos(int a); // Calculate number of possible combinations 
int parag(int a); // Factorial helper
int *frequency(int N); // Allocate and init frequency array

int main()
{	
  
    int *mat, *p, *freq;
    int allcases;
	int N, X1, X2, Y1, Y2; 
	int i;
	
/* Read configuration */
	N  = plnumbers();
	
	X1 = zygous();
	X2 = zygous();
	
	Y1 = afroisma();
	Y2 = afroisma();
	
/* Allocate and read numbers */

	mat = (int*) malloc(N*sizeof(int));
	if(mat == NULL)
	{
		printf("no memory sorry");
		exit(1);
	}

	input( N, mat);
	
	qsort( mat, N, sizeof(int), compare); //ταξινομιση πινακα κατα αυξουσα σειρα

 /* Frequency counters */
	freq = frequency(N);
	
/* Generate combinations and collect stats */
	p = compinations( mat, X1, X2, Y1, Y2, N, allcases, freq);

 /* Compute total combinations for reporting */
	allcases = plhfos(N);
	
	printf("\nTotal possible combinations: %d", allcases);
	printf("\n Rejected by even-number constraint: %d", *p);
	printf("\n Rejected by sum constraint: %d", *(p+1));
	printf("\n Valid combinations found: %d", *(p+2));	
	
	for(i=0;i<N;i++)
	  printf("Number %d appeared %d times\n", *(mat + i), *(freq + i));
	
	/* Clean up */
    free(mat);
    free(freq);
    free(p);
	
	return 0;		
}


/* ---- Input / validation functions ---- */
int plnumbers(void)
{
	int N;
	do
	{
	   printf("Enter how many numbers you will provide (7..49): ");
	   scanf("%d", &N);
	}while(N<=6 || N>49);
	return N;
}

int zygous(void) 
{
    static int x1=7, x2;
	                               

	  if(x1==7)
	  {
	       do
		  {
		    printf("\n Enter minimum number of even numbers (0..6): ");
	        scanf("%d", &x1);
	      }while(x1<0 || x1>6);
	      return x1;
      }
      
      else 
	  {   
	     do
	     {
	       printf(" Enter maximum number of even numbers (must be >= previous and <=6): ");
	       scanf("%d", &x2);
	     }while(x2<x1 || x2>6);
	     return x2;
	  }
    
    
   
}

int afroisma(void) 
{
	static int y1=20, y2;
	
	   if(y1==20)
	   {
		 do
		 {
		   printf("\nEnter minimum sum for the 6-number combination (21..279): ");
		   scanf("%d", &y1);
	     }while(y1<21 || y1>279);
	     return y1;
	   }
	   else 
	   {
	   	 do
	   	 {
	   	 	printf("\nEnter maximum sum for the 6-number combination (>= min, <=279): ");
	   	 	scanf("%d", &y2);
		 }while(y2>279 || y2<y1);
		 return y2;
	   }

}
void input(int N, int *mat)
{
	int mpla, a, found, i;	
	
	for(i=0;i<N;i++)
	*(mat+i)=0;
	
	for(a=0; a<N; a++)
	{  
		do
		{
		  printf("\n Enter number: ");
	      scanf("%d", &mpla);
	    }while(mpla<1 || mpla>49);
	    
		found = linear_search( mat, mpla, a);
		
		if(found==0)
		*(mat+a) = mpla;
		else 
		{
			printf("\nDuplicate found. Enter a different number: ");
			scanf("%d", &mpla);
			*(mat + a) = mpla;
		}
    }
}



int linear_search(int *mat, int number, int N)
{
	int i, find=0;
	for(i=0; i<N; i++)
	{
		if(*(mat + i) == number)
		find++;
	}
	return find;
}

int compare(const void *elem1, const void *elem2) 
{
	return *(int*)elem1 - *(int*)elem2;
}

/* ---- Combination generation and checks ---- */
int *compinations(int *mat, int X1, int X2, int Y1, int Y2, int N, int allc, int *freq)
{
	int one, two, three, four, five, six;
	int check1=0, check2=0, truecount=0;
	int *pl;
	
	pl = (int*) malloc(((allc*6)+3)*sizeof(int));
	if(pl == NULL)
	exit(1);
	
	for(one=0; one<(N-5); one++)
	    for(two = one+1; two<(N-4); two++)
	        for(three = two+1; three<(N-3); three++)
	            for(four = three+1; four<(N-2); four++)
	        	    for(five = four+1; five<(N-1); five++)
	        	        for(six = five+1; six<N; six++)
				            {	
							   if( check_one( mat, one, two, three, four, five, six, X1, X2) == 0)
							   {
							   	    if( check_two( mat, one, two, three, four, five, six, Y1, Y2) == 0)
							   	      {
							   	     	 printf("\n %d %d %d %d %d %d", *(mat+one), *(mat+two), *(mat+three), *(mat+four), *(mat+five), *(mat+six));
				                         truecount++;
				                         (*(freq+one))++;
				                         (*(freq+two))++;
				                         (*(freq+three))++;
				                         (*(freq+four))++;
				                         (*(freq+five))++;
				                         (*(freq+six))++;
										 	                         
									  }
								    else
								        check2++;
							   }
							   
							   else
							       check1++;
	
                            }
    *pl = check1;
    *(pl+1) = check2;
    *(pl+2) = truecount;
    return pl;	
}


int check_one(int *mat, int one, int two, int three, int four, int five, int six, int X1, int X2) 
{
    int countz;
	countz=0;
    if(((*(mat+one)) % 2) == 0)
	countz++;
	if(((*(mat+two)) % 2) == 0)
    countz++;
	if(((*(mat+three)) % 2) == 0)
    countz++;
	if(((*(mat+four)) % 2) == 0)
	countz++;
    if(((*(mat+five)) % 2) == 0)
    countz++;
	if(((*(mat+six)) % 2) == 0)
	countz++;
	
	if( countz < X1 || countz > X2)						
		return 1;
	else 
	    return 0;		
}


int check_two(int *mat, int one, int two, int three, int four, int five, int six, int Y1, int Y2)
{
	int total;
	total = *(mat+one) + *(mat+two) + *(mat+three) + *(mat+four) + *(mat+five) + *(mat+six);
	
	if( total < Y1 || total > Y2)
	   return 1;
	else 
	   return 0;
}
/* ---- Combinatorics helpers ---- */

int plhfos(int a) 
{
	int ap, bp, df, pl;
	ap = parag(a);
	bp = parag(6);
	df = a-6;
	df = parag(df);
	pl = ap/(bp*df);
	return pl;
}

int parag(int a)  
{
	int i, ap;
    for(i=1, ap=1; i<=a; i++)
    ap = ap*i;
    return ap;
}


/* ---- Frequency allocation ---- */
int *frequency(int N)
{
	int i;
	int *freq;
	freq = (int*) malloc(N*sizeof(int));
	if(freq == NULL)
	    exit(1);
	for(i=0;i<N;i++)
	    *(freq+i) = 0; 
	return freq;
}





















