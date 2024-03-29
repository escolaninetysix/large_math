//  Note that de = Phi*f +1.  This requires de - Phif=1.
//   This requires de > Phif
//  

#include<iostream.h>
#include<stdlib.h>
#include<fstream.h>
#include<math.h>

struct numbernode
{
	int digit;
	struct numbernode* next;
};

struct signednumber
{
	struct numbernode* magnitude;
	int sign;   // sign =-1 or 1
};

struct divout
{
	struct numbernode* quotient;
	struct numbernode* remainder;
};

struct leadingstruct
{
	int leadingdivisor;
	int leadingmet;
};

struct gcmout
{
	struct numbernode* number1_multiple_gcm;
	struct numbernode* number2_multiple_gcm;
	struct numbernode* gcm;
};

struct pair
{
	struct numbernode *encoder;
	struct numbernode *decoder;
};

struct numbernode* deletenumber(struct numbernode* number);
void printrev (struct numbernode* number);
struct numbernode* inttonumber (int integer);
struct numbernode* multiply(struct numbernode* number1, struct numbernode* number2);
struct signednumber* opposite (struct signednumber* number);
struct numbernode* copy (struct numbernode* number);
numbernode* reverse( struct numbernode* list);
char *stringnum(numbernode* number);
struct numbernode* greater(struct numbernode* number1, struct numbernode* number2);
struct numbernode* zeroclean (struct numbernode* number);
struct signednumber* addsigned(struct signednumber* number1, struct signednumber* number2);
leadingstruct leading (struct numbernode* divisor, struct numbernode* met);
struct divout* divide(struct numbernode* dividend, struct numbernode* divisor);
struct numbernode* apowerbmodc (struct numbernode* a, struct numbernode *b,
								struct numbernode *c);
struct gcmout *gcm(numbernode* number1, numbernode* number2);
struct numbernode *stringtonumber(char *string);

struct pair *board_pair(int board_serial_number, struct numbernode *N,
								 struct numbernode* prime, 
								 struct numbernode* another_prime,int number_boards);
void listpairs(struct numbernode *N, struct numbernode* prime,
			   struct numbernode* another_prime, int number_boards);
void encryptmsg(struct numbernode* message, struct numbernode *N, int number_boards);
struct numbernode* decrypt(struct numbernode *encrypted_message, struct numbernode*N,
						   int board_serial_number, struct numbernode* prime, 
								 struct numbernode* another_prime,int number_boards);
void verify_encrypted(struct numbernode* message, struct numbernode *N, int number_boards);

int main()
{
	// N is the modulus used in encrypting messages,
	//  and the 2 primes composing N are needed.
	struct numbernode* prime1 = inttonumber(10086767);
	struct numbernode* prime2 = inttonumber(17344123); // bigger
	struct numbernode* N = multiply(prime1, prime2);

	// Useful functions to call:
	//   decrypt()			- This will decrypt a message for a particular board serial #.
	//   listpairs()		- Outputs decryptor & encryptor values for all boards.
	//   encryptmsg()		- Lists a message encrypted specially for each board.
	//   verify_encrypted() - Checks the encryptedmsg list with the decoderlist- does is work?

	int number_boards = 25000;
	listpairs(N, prime1, prime2, number_boards);
	char msgstring[16]="120458293847728";
	struct numbernode* message = stringtonumber(msgstring);
	encryptmsg(message, N, number_boards);
	verify_encrypted(message, N, number_boards);

	return(0);
}

struct numbernode* deletenumber(struct numbernode* number)
{
	struct numbernode* temp;
	while (number!=NULL)
	{
		temp = number->next;
		delete number;
		number = temp;
		temp=NULL;
	}

	return (NULL);

}

void printrev (struct numbernode* number)
{
	struct numbernode* current = number;
	while (current!=NULL)
	{
		cout << current->digit << " ";
		current=current->next;
	}
	cout << endl;
	return;
}

struct numbernode* inttonumber (int integer)
{
	struct numbernode* numberout=NULL;
	struct numbernode* current;
	int reduced = integer;
	if (reduced>0)
	{
		numberout=new struct numbernode;
		numberout->digit=reduced%10;
		numberout->next=NULL;
		reduced=reduced/10;
		current = numberout;
		while (reduced>0)
		{
			current->next = new struct numbernode;
			current = current->next;
			current->digit = reduced%10;
			current->next = NULL;
			reduced=reduced/10;
		}
	}

	return(numberout);
}
  
struct numbernode* multiply(struct numbernode* number1, struct numbernode* number2)
{
	// accomodate apowerbmodc:
	//    - doubler in apowerbmodc can be zero, as the second argument.
	//			- it can be divide's remainder which can be zero
				//  which means: NULL
	struct numbernode* t1;
	if (number1==NULL || number2==NULL)
	{
		return NULL;
	}
	else
	{
		struct numbernode* answer;
		struct numbernode* runner1;
		struct numbernode* runner2;
		int ones;
		int carry;
		struct numbernode* addzeroposition;
		struct numbernode* addposition;

		// Deal with row1 * row2,1st digit
		if (number2->digit==0)
		{
			t1=NULL;
			t1=new struct numbernode;
			answer =t1;
			answer->digit=0;
			answer->next=NULL;
		}
		else
		{
			runner1=number1;
			t1=NULL;
			t1=new struct numbernode;
			answer=t1;
			ones = (runner1->digit*number2->digit)%10;
			carry = (runner1->digit*number2->digit)/10;
			answer->digit=ones;
			answer->next=NULL;
			addposition=answer;
			while (runner1->next!=NULL)
			{
				runner1=runner1->next;
				t1=NULL;
				t1=new struct numbernode;
				addposition->next=t1;
				addposition=addposition->next;
				ones = (carry + runner1->digit*number2->digit)%10;
				carry = (carry + runner1->digit*number2->digit)/10;
				addposition->digit=ones;
				addposition->next = NULL;
				//carry is carry.
			}
			if (carry!=0)
			{
				t1=NULL;
				t1 = new struct numbernode;
				addposition->next=t1;
				addposition = addposition->next;
				addposition->digit=carry;
				addposition->next = NULL;
			}
		}
			// Now answer contains number1 * number2->digit.

		runner2=number2->next;
		if (runner2!=NULL)
		{
			if (answer->next==NULL)
			{
				t1=NULL;
				t1=new struct numbernode;
				answer->next=t1;
				answer->next->digit=0;
				answer->next->next=NULL;
			}
		}
		addzeroposition=answer->next;
		while (runner2!=NULL)
		{
			while (runner2->digit==0)
			{
				runner2=runner2->next; // has to be non-zero eventually
				
				//advance addzero position:
				if (addzeroposition->next==NULL)
				{
					t1=NULL;
					t1= new struct numbernode;
					addzeroposition->next=t1; 
					addzeroposition=addzeroposition->next;
					addzeroposition->digit=0;
					addzeroposition->next=NULL;
				}
				else
					addzeroposition= addzeroposition->next;
			}

			// do multiply and augment:
			addposition = addzeroposition;
			// do first digit:
			runner1=number1;
			ones = (addposition->digit+runner1->digit*runner2->digit)%10;
			carry = (addposition->digit+runner1->digit*runner2->digit)/10;
			addposition->digit=ones;
			//carry has carry.
			while (runner1->next!=NULL)
			{
				runner1=runner1->next;
				if (addposition->next == NULL)
				{
					t1=NULL;
					t1=new struct numbernode;
					addposition->next=t1;
					addposition->next->digit=0;
					addposition->next->next=NULL;
				}
				addposition=addposition->next;
				ones = (carry + addposition->digit+runner1->digit*runner2->digit)%10;
				carry = (carry + addposition->digit+ runner1->digit*runner2->digit)/10;
				addposition->digit=ones;
				// carry is carry.
			}
			if (carry!=0)
			{
				if (addposition->next==NULL) // if ->next is unset to NULL after a new struct,
											// then this gets missed and a crash occurs below.
				{
					t1=NULL;
					t1 = new struct numbernode;
					addposition->next =t1;
					addposition->next->digit=0;
					addposition->next->next = NULL;
				}
				addposition=addposition->next;
				ones = (carry + addposition->digit)%10;
				carry = (carry + addposition->digit)/10;
				addposition->digit = ones;
				// carry has carry.
				if (carry!=0)
				{
					t1=NULL;
					t1 = new struct numbernode;
					addposition->next =t1;
					addposition=addposition->next;
					addposition->digit=carry;
					addposition->next = NULL;
				}
			}

			runner2=runner2->next;
			if (runner2!=NULL)
			{
				//advance addzero position:
				if (addzeroposition->next==NULL)
				{
 					t1=NULL;
					t1= new struct numbernode;
					addzeroposition->next=t1; 
					addzeroposition=addzeroposition->next;
					addzeroposition->digit=0;
					addzeroposition->next=NULL;
				}
				else
					addzeroposition= addzeroposition->next;
			}
		}
		return(answer);
	}
}

struct signednumber* opposite (struct signednumber* number)
{
	number->sign=-number->sign;
	return number;
}

struct numbernode* copy (struct numbernode* number)
{
	struct numbernode* t1;
	struct numbernode* replica=NULL;
	struct numbernode* current;
	struct numbernode* currentcopy;
	if (number!=NULL)
	{
		current=number;
		replica = new struct numbernode;
		replica->digit=number->digit;
		replica->next=NULL;
		currentcopy=replica;
		while (current->next!=NULL)
		{
			current=current->next;
			t1=NULL;
			t1= new struct numbernode;
			currentcopy->next=t1;
			currentcopy = currentcopy->next;
			currentcopy->digit=current->digit;
			currentcopy->next=NULL;
		}
	}
	return replica;
}

struct numbernode* reverse( struct numbernode* list)
{
	if (list==NULL) return NULL;
	if (list->next==NULL)
		return(list);
	struct numbernode* newhead;
	struct numbernode* temp;
	struct numbernode* newheadtemp;
	newhead = list->next;
	list->next = NULL;
	temp = newhead->next;
	newhead->next = list;
	while (temp != NULL)
	{
		newheadtemp = newhead;
		newhead = temp;
		temp = newhead->next;
		newhead->next = newheadtemp;
	}
	return(newhead);
}

char *stringnum(numbernode* number)
{
	char* string= new char[200];
	if (number==NULL) string[0]=NULL;
	else
	{
		struct numbernode* number2 = copy(number);
		number2=reverse(number2);
		int count=0;
		struct numbernode* current = number2;
		while(current!=NULL)
		{
			string[count]=48+current->digit;
			count++;
			current=current->next;
		}
		string[count]='\0';
		number2=deletenumber(number2);
	}
	return (string);
}

struct numbernode* greater(struct numbernode* number1, struct numbernode* number2)
{
	struct numbernode* greatermag;
	if (number1==NULL&&number2==NULL)
		greatermag=NULL;
	else if (number1==NULL)
		greatermag=number2;
	else if (number2==NULL)
		greatermag=number1;
	else //Need to compare two non-zero magnitudes:
	{
		struct numbernode* current1=number1;
		struct numbernode* current2=number2;

		while (current1!=NULL && current2!=NULL)
		{
			current1=current1->next;
			current2=current2->next;
		}
		if (current1==NULL && current2!=NULL)
			greatermag=number2;
		else if (current1!=NULL&&current2==NULL)
			greatermag=number1;
		else // Same # of digits so must compare:
		{
			// if only one digit long:

			struct numbernode* copy1 = copy(number1);
			struct numbernode* copy2 = copy(number2);
			copy1=reverse (copy1);
			copy2=reverse (copy2);
			//check digits from greatest to least
			current1=copy1;
			current2=copy2;
			if (current1->digit==current2->digit&&current1->next!=NULL)
			{
				while (current1->digit==current2->digit&&current1->next!=NULL)
				{
					current1=current1->next;
					current2=current2->next;
				}
				if (current1->digit > current2->digit)
					greatermag=number1;
				else if (current2->digit>current1->digit)
					greatermag=number2;
				else
					greatermag=NULL; // they're equal.
			}
			else
			{
				if (current1->digit > current2->digit)
					greatermag=number1;
				else if (current2->digit>current1->digit)
					greatermag=number2;
				else
					greatermag=NULL; // they're equal.
			}
			
			copy1=deletenumber(copy1);	   // this should delete them properly.
			copy2=deletenumber(copy2);
		}
	}
	return(greatermag);
}

struct numbernode* zeroclean (struct numbernode* number)
{
	// Helper function for subtraction;
	//  for use with nonzero number.

	// Remove leading zeros:
	struct numbernode* t1;
	struct numbernode* newnumber=NULL;
	if (number!=NULL)
	{
		t1=NULL;
		t1= new struct numbernode;
		newnumber=t1;
		newnumber->digit=number->digit;
		newnumber->next = NULL;
		struct numbernode * greatestnonzero=number;
		struct numbernode* currentold = number;
		while (currentold->next!=NULL)
		{
			currentold=currentold->next;
			if (currentold->digit!=0)
				greatestnonzero=currentold;
		}

		currentold=number;
		struct numbernode* currentnew = newnumber;
		if (greatestnonzero!=number)
		{
			// Copy all digits up to & including currentold=greatestnonzero
			do
			{
				currentold=currentold->next;
				t1=NULL;
				t1 =new struct numbernode;
				currentnew->next=t1;
				currentnew=currentnew->next;
				currentnew->digit=currentold->digit;
				currentnew->next=NULL;
			} while (currentold!=greatestnonzero);
		}
	}
	number=deletenumber(number);
	return newnumber;
}

struct signednumber* addsigned(struct signednumber* number1, struct signednumber* number2)
{
	struct numbernode *t1;
	int carry, ones;
	struct signednumber * answer=new struct signednumber;

	if (number1->magnitude==NULL && number2->magnitude==NULL)
	{
		answer->magnitude=NULL;
		answer->sign = 1;
	}
	else if (number1->magnitude==NULL)
	{
		answer->magnitude=copy(number2->magnitude);
		answer->sign=number2->sign;
	}
	else if (number2->magnitude==NULL)
	{
		answer->magnitude=copy(number1->magnitude);
		answer->sign=number1->sign;
	}
	else
	{
		//assume numbers aren't null magnitude.
		if (number1->sign==number2->sign)
		{
			//add magnitudes, keep sign.
			answer->sign=number1->sign;
			struct numbernode* carryextra;
			struct numbernode* current1=number1->magnitude;
			struct numbernode* current2=number2->magnitude;
			carry=0;
			struct numbernode* carryanswer;
			//Do first digit:
			ones = (current1->digit+current2->digit)%10;
			carry= (current1->digit+current2->digit)/10;
			t1=NULL;
			t1=new struct numbernode;
			answer->magnitude =t1;
			answer->magnitude->digit=ones;
			answer->magnitude->next=NULL;
			current1=current1->next;
			current2=current2->next;
			carryanswer=answer->magnitude;
			//deal with multiple digit terms
			if (current1!=NULL||current2!=NULL)
			{
				while (current1!=NULL&&current2!=NULL)
				{
					t1=NULL;
					t1= new struct numbernode;
					carryanswer->next=t1; 
					carryanswer=carryanswer->next;
					ones=(carry+current1->digit+current2->digit)%10;
					carry = (carry+current1->digit+current2->digit)/10;
					carryanswer->digit=ones;
					carryanswer->next=NULL;
					current1=current1->next;
					current2=current2->next;
				}
				if (current1!=NULL||current2!=NULL)
				{
					if (current1==NULL)
						carryextra=current2;
					else
						carryextra=current1;
					//carry has carry

					//do first digit:
					ones = (carry+carryextra->digit)%10;
					carry = (carry+carryextra->digit)/10;
					t1=new struct numbernode;
					carryanswer->next=t1;
					carryanswer=carryanswer->next;
					carryanswer->digit=ones;
					carryanswer->next=NULL;
					//carry has carry
					carryextra=carryextra->next;
					while (carryextra!=NULL)
					{
						ones = (carry+carryextra->digit)%10;
						carry = (carry+carryextra->digit)/10;
						t1=NULL;
						t1= new struct numbernode;
						carryanswer->next=t1; 
						carryanswer=carryanswer->next;
						carryanswer->digit=ones;
						//carry has carry
						carryanswer->next=NULL;
						carryextra=carryextra->next;
					}
				}
				if (carry>0)
				{
					//digit+carry (carry <=1) = 9+1 max implies carry<=1.
					t1=NULL;
					t1=new struct numbernode;
					carryanswer->next=t1;
					carryanswer=carryanswer->next;
					carryanswer->digit=1; // (carry must = 1)
					carryanswer->next = NULL;
				}
			}

			else  // deal with carry for single digit sum:
			{
				if (carry>0)
				{
					t1=NULL;
					t1=new struct numbernode;
					carryanswer->next=t1;
					carryanswer=carryanswer->next;
					carryanswer->digit=1; // (carry must = 1)
					carryanswer->next = NULL;
				}
			}
		}
		
		else // number1->sign is opposite number2->sign:
		{
			struct numbernode* smallnumber;
			struct numbernode * bignumber=greater(number1->magnitude, number2->magnitude);
			if (bignumber==NULL) // if equal in magnitude
			{
				answer->magnitude=NULL;
			}
			else
			{
				int borrow, major, smallerterm, oldborrow;
				if (bignumber==number1->magnitude)
				{
					smallnumber=number2->magnitude;
					answer->sign=number1->sign;
				}
				else
				{
					smallnumber=number1->magnitude;
					answer->sign=number2->sign;
				}
				
				//Compute answer magnitude:
				//Do first digit.
				t1=NULL;
				t1=new struct numbernode;
				answer->magnitude =t1;
				borrow=0; 
				major=bignumber->digit;
				if (major<smallnumber->digit)
				{
					major = 10+major;
					borrow=1;
				}
				answer->magnitude->digit=major-smallnumber->digit;
				answer->magnitude->next=NULL;
				//do remaining digits.
				//     note that if there was a borrow there will be another digit since
				//		bignumber>smallnumber.
				struct numbernode* currentbig=bignumber;
				struct numbernode* currentsmall=smallnumber;
				struct numbernode* currentanswer=answer->magnitude;
				while (currentbig->next!=NULL)
				{
					currentbig=currentbig->next;
					if (currentsmall->next==NULL)
					{
						smallerterm=0; 
					}
					else
					{
						currentsmall=currentsmall->next;
						smallerterm=currentsmall->digit;
					}

					oldborrow=borrow;
					borrow=0;
					if ((currentbig->digit-oldborrow)<smallerterm)
					{
						borrow=1;
						major=10+currentbig->digit-oldborrow;
					}
					else
					{
						major=currentbig->digit-oldborrow;
					}
					t1=NULL;
					t1=new struct numbernode;
					currentanswer->next=t1;
					currentanswer=currentanswer->next;
					currentanswer->digit=major-smallerterm;
					currentanswer->next=NULL;
				}
			}
		answer->magnitude = zeroclean(answer->magnitude);
		}
	}

	return(answer);
}

leadingstruct leading (struct numbernode* divisor, struct numbernode* met)
{
	// note this gives the leading three digits of the divisor,
	//  and the leading 3 or 4 digits of met,
	// or it gives one or two digits of divisor and gives all of met.

	struct leadingstruct answer;
	int divcount = 0;
	struct numbernode* currentdivisor=divisor;
	struct numbernode* currentmet;
	divcount++;
	int magnitude;
	int chop=0;
	int waschopped=0;
	while(currentdivisor->next!=NULL)
	{
		divcount++;
		currentdivisor=currentdivisor->next;
	}
	currentdivisor=divisor;
	currentmet=met;
	if (divcount>3) chop = divcount-3;
	if (chop>0) waschopped=1;
	while (chop>0)		// if met input to fn is NULL, this crashed.
	{
		currentdivisor=currentdivisor->next;
		if (currentmet!=NULL)
			if (currentmet->next!=NULL) currentmet=currentmet->next;
		chop--;
	}
	// build answer values.
	magnitude=1;
	answer.leadingdivisor=0;
	while (currentdivisor!=NULL)
	{
		answer.leadingdivisor+=magnitude*currentdivisor->digit;
		magnitude*=10;
		currentdivisor=currentdivisor->next;
	}

	magnitude=1;
	answer.leadingmet=0;
	while (currentmet!=NULL)
	{
		answer.leadingmet+=magnitude*currentmet->digit;
		magnitude*=10;
		currentmet=currentmet->next;
	}
	// round up leadingdivisor
	if (waschopped) answer.leadingdivisor+=1;
	// round down leadingmet
	if (answer.leadingmet>0) answer.leadingmet-=1; // Cop out... this will make k conservatively
		// work, but could be inefficient.  The real question is, did any digits get chopped
		// off of met to get leadingmet.  I didn't keep track of this.
	return answer;
}

struct divout* divide(struct numbernode* dividend, struct numbernode* divisor)
{
	struct numbernode* t1;
	// Note EX: if 230/23 or 0/23 then remainder is NULL.

	struct divout* answer = new struct divout;
	// Assumes that divisor > 0.


	struct numbernode* product;
	struct numbernode* temp1;
  	struct numbernode* temp2;

	// create met if quotient>0
	if (greater(divisor,dividend)==divisor)
	{
		answer->quotient=NULL;
		answer->remainder=copy(dividend);
	}
	else
	{
		// Here create met, then do the division.

		dividend = reverse(dividend);
		
		struct leadingstruct leaders;
		struct numbernode* untoucheddividend=dividend; // Note destroy dividend, by
		// moving its digits into met.
		struct numbernode* met=NULL; // Met holds the digits of the dividend which
		     // the algorithm has met so far, taking the digits away from the remaining
		     // digits of the dividend; the pointer to the untouched digits of the dividend
			// is then called untoucheddividend.
		struct numbernode* numtemp;

		int k;
		struct signednumber* subresult, *sub1, *sub2;
		struct numbernode* quotientcurrent;

		// build met until it is >= divisor	 aka while divisor > met keep building met.
		
		while (greater(divisor, met)==divisor)
		{	// build met by inserting before its first digit.
			if (met==NULL)
			{
				  met= new struct numbernode;
				  met->digit = untoucheddividend->digit;
				  met->next=NULL;
				  untoucheddividend=untoucheddividend->next;
			}
			else
			{
				numtemp = met;
				met = new struct numbernode;
				met->digit = untoucheddividend->digit;
				met->next = numtemp;
				untoucheddividend=untoucheddividend->next;
			}
		}

		// Get first digit of quotient.
		// determine k=met/divisor and met=met%divisor:
		leaders=leading(divisor,met);
		k = leaders.leadingmet/leaders.leadingdivisor; // met rounded down / divisor rounded up
		temp1= inttonumber(k+1);
		product = multiply(temp1, divisor);
		temp1=deletenumber(temp1);
		temp2= greater(product, met);
		product=deletenumber(product);
		while (temp2==met||// if (K+1)*divisor <=met
				temp2==NULL)
		{
				k++;	// while K+1*divisor <=met

				temp1= inttonumber(k+1);
				product = multiply(temp1, divisor);
				temp1=deletenumber(temp1);
				temp2= greater(product, met);
				product=deletenumber(product);
		}
		sub1 = new struct signednumber;
		sub1->sign=1;
		sub1->magnitude=met;
		sub2 = new struct signednumber;
		sub2->sign=-1;
		temp1=inttonumber(k);
		sub2->magnitude=multiply(temp1,divisor);
		temp1=deletenumber(temp1);
		subresult=addsigned(sub1, sub2);
		sub1->magnitude=deletenumber(sub1->magnitude);
		sub2->magnitude=deletenumber(sub2->magnitude);
		delete sub1;
		delete sub2;
		met = subresult->magnitude;
		delete subresult;

		t1=NULL;
		t1= new struct numbernode;
		answer->quotient=t1; 
		answer->quotient->digit = k;
		answer->quotient->next = NULL;


		quotientcurrent=answer->quotient;
		// Get remaining digits of quotient.

		while (untoucheddividend!=NULL)
		{
			// Augment met:
			if (met==NULL)
			{
				  if (untoucheddividend->digit!=0)
				  {
					  met = new struct numbernode;
					  met->digit = untoucheddividend->digit;
					  met->next=NULL;
					  untoucheddividend=untoucheddividend->next;
				  }
				  else
					  untoucheddividend=untoucheddividend->next;
			}
			else
			{
				numtemp = met;
				met = new struct numbernode;
				met->digit = untoucheddividend->digit;
				met->next = numtemp;
				untoucheddividend=untoucheddividend->next;
			}


			// Get quotient another digit: met/divisor.; get resulting met.
			
			// determine k=met/divisor and met=met%divisor:
			leaders=leading(divisor,met);
			k = leaders.leadingmet/leaders.leadingdivisor; // met rounded down / divisor rounded up		
			//cout << "lead met was " <<leaders.leadingmet << endl;
			//cout << "Lead divisor was " <<leaders.leadingdivisor << endl;
			//cout << "k estimate was " << k << endl;
		
			temp1= inttonumber(k+1);
			product = multiply(temp1, divisor);
			temp1=deletenumber(temp1);
			temp2= greater(product, met);
			product=deletenumber(product);
			while (temp2==met||// if (K+1)*divisor <=met
					temp2==NULL)
			{
					k++;	// while K+1*divisor <=met

					temp1= inttonumber(k+1);
					product = multiply(temp1, divisor);
					temp1=deletenumber(temp1);
					temp2= greater(product, met);
					product=deletenumber(product);
			}
						
			if (k==0) // avoid subtraction:
			{
				//leave met the way it is.
			}
			else
			{
				sub1 = new struct signednumber;
				sub1->sign=1;
				sub1->magnitude=met;
				sub2 = new struct signednumber;
				sub2->sign=-1;
				temp1=inttonumber(k);				
				sub2->magnitude=multiply(temp1,divisor);
				temp1=deletenumber(temp1);
				subresult=addsigned(sub1, sub2);
				sub1->magnitude=sub1->magnitude=deletenumber(sub1->magnitude);
				deletenumber(sub2->magnitude);
				if (subresult->sign == - 1) cout << "Bad sign in subtract" << endl;
				delete sub1;
				delete sub2;

				met = subresult->magnitude;
				delete subresult; 
			}

			t1=NULL;
			t1 = new struct numbernode;
			quotientcurrent->next=t1;
			quotientcurrent=quotientcurrent->next;
			quotientcurrent->digit = k;
			quotientcurrent->next = NULL;
			 
		}	
		

		// Put answer quotient in final form.
		answer->quotient = reverse(answer->quotient);
		// Put met in as the remainder.
		answer->remainder = met;

		dividend = reverse(dividend);
	}

	return(answer);
}

struct numbernode* apowerbmodc (struct numbernode* a, struct numbernode *b,
								struct numbernode *c)
{
	// doesn't handle case where a=b=c or where a=c since a mod c = 0, since multiply can't take 0 argument.
	struct numbernode *product= inttonumber(1);
	struct numbernode* two = inttonumber(2);
	struct numbernode* temp;
	struct divout *answer;
	struct divout *answer2;
	struct numbernode *bdissolve = copy(b);
	struct numbernode *doubler= copy(a);
	while (bdissolve!=NULL)
	{
		// include doubler as a factor if in b's binary representation:
		temp = bdissolve;
		answer = divide(bdissolve, two);
		temp=deletenumber(temp);
		bdissolve=answer->quotient;
		if (answer->remainder!=NULL)   // if remainder is 1, that is to say.
		{
			if (answer->remainder->digit==1)
			{
				//let product = product * doubler mod c:

				// get product
				temp = product;
				product = multiply (product, doubler);
				temp=deletenumber(temp);
				// get product mod c
				answer2 = divide(product, c);
				product=deletenumber(product);
				product=answer2->remainder;
				answer2->quotient=deletenumber(answer2->quotient);
				delete answer2;
			}
		}
		answer->remainder=deletenumber(answer->remainder);
		delete answer;
		// bdissolve is NULL if all b's digits are gone.

		//square doubler
		temp = doubler;
		doubler=multiply(doubler,doubler);
		temp=deletenumber(temp);
		answer=divide(doubler, c);
		doubler=deletenumber(doubler);
		doubler=answer->remainder;
		answer->quotient=deletenumber(answer->quotient);
		delete answer;
		// doubler holds doubler*doubler mod c.
	}
	two=deletenumber(two);
	doubler=deletenumber(doubler);
	return(product);
}

struct gcmout *gcm(numbernode* number1, numbernode* number2)
{
	struct gcmout * gcmreturn  = new struct gcmout;

	struct numbernode* a, *b;
	int flip;

	struct signednumber* atemp1, *atemp2, *atemp3;
	struct numbernode* temp1;

	if (greater(number1, number2)==number2)
	{
		a=number2;
		b=number1;
		flip = 1;
	}
	else
	{
		a=number1;
		b=number2;
		flip = 0;
	}

	struct signednumber *an1, *bn1, *an2, *bn2, *an3, *bn3;
	struct divout *divtemp;

	int notthrough = (greater(a,b)!=NULL);
	an1 = new struct signednumber;
	an1->magnitude=inttonumber(1);
	an1->sign = 1;
	bn1 = new struct signednumber;
	bn1->magnitude=NULL;
	bn1->sign = 1;
	an2 = new struct signednumber;
	an2->magnitude = NULL;
	an2->sign=1;
	bn2 = new struct signednumber;
	bn2->magnitude = inttonumber(1);
	bn2->sign=1;
	struct numbernode* v1, *v2;
	struct numbernode * oldremainder = NULL;
	while(notthrough)
	{
		atemp1 = new struct signednumber;
		atemp1->sign = an1->sign;
		atemp1->magnitude= multiply(an1->magnitude, a);
		atemp2 = new struct signednumber;
		atemp2->sign = bn1->sign;
		atemp2->magnitude = multiply(bn1->magnitude, b);
		atemp3 = addsigned(atemp1, atemp2);	// need addsigned to add 0 if needed.
		atemp1->magnitude=deletenumber(atemp1->magnitude);
		delete atemp1;
		atemp2->magnitude=deletenumber(atemp2->magnitude);
		delete atemp2;
		v1 = atemp3->magnitude; // Assume that v1 is positive, because it must be.
		delete atemp3;

		atemp1 = new struct signednumber;
		atemp1->sign=an2->sign;
		atemp1->magnitude= multiply(an2->magnitude, a);
		atemp2 = new struct signednumber;
		atemp2->sign = bn2->sign;
		atemp2->magnitude = multiply(bn2->magnitude, b);
		atemp3 = addsigned(atemp1, atemp2);
		atemp1->magnitude=deletenumber(atemp1->magnitude);
		delete atemp1;
		atemp2->magnitude=deletenumber(atemp2->magnitude);
		delete atemp2;
		v2 = atemp3->magnitude; // Assume that v2 is positive, because it must be.
		delete atemp3;

		divtemp = divide(v1, v2);
		v1=deletenumber(v1);
		v2=deletenumber(v2);

		if (divtemp->remainder==NULL)
		{
			// Done with Euclid's algorithm.
			
			notthrough=0;
			//output of function:
			gcmreturn->gcm= copy(oldremainder); // this is the greatest common multiple (gcm).
			if (flip==1)
			{
				gcmreturn->number1_multiple_gcm=copy(bn2->magnitude);
				gcmreturn->number2_multiple_gcm=copy(an2->magnitude);
			}
			else
			{
				gcmreturn->number1_multiple_gcm=copy(an2->magnitude);
				gcmreturn->number2_multiple_gcm=copy(bn2->magnitude);
			}
		}
		else
		{
			// an3 = an1-k*an2:
			atemp1 = new struct signednumber;
			atemp1->sign = an1->sign;
			atemp1->magnitude=an1->magnitude;
			atemp2 = new struct signednumber;
			atemp2->sign = -1*(an2->sign);
			temp1 = multiply(divtemp->quotient,an2->magnitude);
			atemp2->magnitude=temp1;
			atemp3 = addsigned(atemp1, atemp2);
			an3 = new struct signednumber;
			an3->magnitude = atemp3->magnitude;
			an3->sign=atemp3->sign;
			temp1=deletenumber(temp1);
			delete atemp1;
			delete atemp2;
			delete atemp3;

			// bn3 = bn1-k*bn2;
			atemp1 = new struct signednumber;
			atemp1->sign = bn1->sign;
			atemp1->magnitude=bn1->magnitude;
			atemp2 = new struct signednumber;
			atemp2->sign = -1*(bn2->sign);
			temp1 = multiply(divtemp->quotient, bn2->magnitude);
			atemp2->magnitude=temp1;
			atemp3 = addsigned(atemp1, atemp2);
			bn3 = new struct signednumber;
			bn3->magnitude= atemp3->magnitude;   
			bn3->sign = atemp3->sign;
			temp1=deletenumber(temp1);
			delete atemp1;
			delete atemp2;
			delete atemp3;


			// Shift it down
			an1->magnitude=deletenumber(an1->magnitude);
			bn1->magnitude=deletenumber(bn1->magnitude);
			an1->sign = an2->sign;
			an1->magnitude=	an2->magnitude;
			bn1->sign = bn2->sign;
			bn1->magnitude = bn2->magnitude;
			an2->sign = an3->sign;
			an2->magnitude= an3->magnitude;
			bn2->sign=bn3->sign;
			bn2->magnitude = bn3->magnitude;
			delete an3;
			delete bn3;

		}
		oldremainder=deletenumber(oldremainder);
		oldremainder=copy(divtemp->remainder);
		divtemp->quotient=deletenumber(divtemp->quotient);
		divtemp->remainder=deletenumber(divtemp->remainder);
		delete divtemp;		 
	
	}

	an2->magnitude=deletenumber(an2->magnitude);
	delete an2;
	bn2->magnitude=deletenumber(bn2->magnitude);
	delete bn2;
	an1->magnitude=deletenumber(an1->magnitude);
	delete an1;
	bn1->magnitude=deletenumber(bn1->magnitude);
	delete bn1;

	return(gcmreturn);
}

struct numbernode *stringtonumber(char *string)
{
	struct numbernode *t1;
	int i;
	numbernode* newnumber = NULL;
	numbernode* current;
	if (string[0]!='\0')
	{
		t1= new struct numbernode;
		newnumber=t1; 
		newnumber->digit=string[0]-48;
		newnumber->next=NULL;
		current = newnumber;
		i=1;
		while (string[i]!='\0')
		{
			t1 = new struct numbernode;
			current->next=t1;
			current=current->next;
			current->digit=string[i]-48;
			current->next = NULL;
			i++;
		}
	}
	newnumber = reverse(newnumber);
	return newnumber;
}

struct pair *board_pair(int board_serial_number, struct numbernode *N,
								 struct numbernode* prime, 
								 struct numbernode* another_prime,int number_boards)
{
	// There are number_boards evenly distributed points < PhiN.
	// This function guesses along a line of evenly distributed points.
	// Then it finds the next available relativeprime to PhiN, counting
	// upwards until one is found.  This way the list of decoders need not
	// be stored in the installer.  Rather, this very same algorithm will
	// be used in the installer.
	//
	// The guess for the encoder is= .1 PhiN + .8 PhiN * (board_serial_number/number_boards).

	struct pair *the_pair;  // the encoder/decoder pair for this board.
	struct numbernode* number_boardsl=inttonumber(number_boards);
	struct numbernode* one = inttonumber(1);

	//produce Phi of N, the # of numbers less than N which are relatively prime to N:
	struct numbernode* temp1;
	struct signednumber* atemp1, *atemp2, *atemp3;
	atemp1 = new struct signednumber;
	atemp1->sign = 1;
	atemp1->magnitude = N;
	atemp2 = new struct signednumber;
	atemp2->sign = -1;
	atemp2->magnitude = prime;
	atemp3 = addsigned(atemp1, atemp2);
	delete atemp1;
	delete atemp2;
	// assume that N>prime1 so sign is +.
	// Now subtract prime2 from atemp3:
	atemp1= new struct signednumber;
	atemp1->sign=-1;
	atemp1->magnitude=another_prime;
	atemp2=addsigned(atemp3, atemp1);
	delete atemp1;
	delete atemp3;
	temp1 = inttonumber(1);
	// Now add 1,  ; assume N-prime1-prime2>=0.
	atemp1= new struct signednumber;
	atemp1->sign = 1;
	atemp1->magnitude= temp1;
	atemp3 = addsigned(atemp2, atemp1);
	struct numbernode* PhiN = atemp3->magnitude;
	delete atemp1;
	temp1=deletenumber(temp1);
	delete atemp3;
	//  Now PhiN holds N-prime1-prime2+1.
	
	struct divout *dftemp1;
	struct signednumber *aftemp1, *aftemp2, *aftemp3;
	struct numbernode* ftemp1, *ftemp2, *ftemp3, *ftemp4, *ftemp5;
	struct numbernode* encoder_candidate;
	struct numbernode *decoder;
	struct gcmout *PhiNRelativePrimeCK;
	
	// Produce 0.1PhiN and 0.8PhiN.
	ftemp5=inttonumber(4);
	ftemp1 = multiply(PhiN, ftemp5); // use PhiN.
	ftemp5=deletenumber(ftemp5);
	ftemp5=inttonumber(5);
	dftemp1 = divide(ftemp1, ftemp5);
	ftemp5=deletenumber(ftemp5);
	ftemp1=deletenumber(ftemp1);
	ftemp1 = dftemp1->quotient;
	dftemp1->remainder=deletenumber(dftemp1->remainder);
	delete dftemp1;
	ftemp5= inttonumber(10);
	dftemp1 = divide(PhiN, ftemp5);
	ftemp5=deletenumber(ftemp5);
	ftemp2 = dftemp1->quotient;
	dftemp1->remainder=deletenumber (dftemp1->remainder);
	delete dftemp1;
	// ftemp1 holds 0.8*PhiN
	// ftemp2 holds 0.1*PhiN

	int f = board_serial_number;
	int is_valid;
	struct signednumber *ss1, *ss2, *ss3;
	
	{
		// Guess at #f by 0.1*PhiN+(f/number_boards)*(0.8*PhiN),
		//   then count upward until a number is found which is relatively prime to PhiN.
		ftemp3 = inttonumber(f);
		ftemp4 = multiply(ftemp3,ftemp1);
		ftemp3=deletenumber(ftemp3);
		dftemp1=divide(ftemp4,number_boardsl);
		ftemp3=dftemp1->quotient;
		dftemp1->remainder=deletenumber(dftemp1->remainder);
		delete dftemp1;
		ftemp4=deletenumber(ftemp4);
		// ftemp3 holds f/number_boards*0.8*PhiN

		aftemp1 = new struct signednumber;
		aftemp1->sign = 1;
		aftemp1->magnitude = ftemp2;
		aftemp2 = new struct signednumber;
		aftemp2->sign = 1;
		aftemp2->magnitude = ftemp3;
		aftemp3 = addsigned(aftemp1, aftemp2);
		encoder_candidate=aftemp3->magnitude;
		delete aftemp1;
		delete aftemp2;
		ftemp3=deletenumber(ftemp3);

		delete aftemp3;

		// Check encoder candidate
		is_valid=0;
		PhiNRelativePrimeCK = gcm(PhiN, encoder_candidate);
		if (greater(PhiNRelativePrimeCK->gcm, one)==NULL)
		{
			ss1 = new struct signednumber;
			ss1->sign=1;
			ss1->magnitude=multiply(PhiNRelativePrimeCK->number1_multiple_gcm, PhiN);
			ss2 = new struct signednumber;
			ss2->sign=-1;
			ss2->magnitude=multiply(PhiNRelativePrimeCK->number2_multiple_gcm, encoder_candidate);
			ss3 = addsigned(ss1, ss2);
			if (ss3->sign==-1) // the sign of the sum 
			{
				is_valid=1;
				decoder = PhiNRelativePrimeCK->number2_multiple_gcm;
			}
			ss1->magnitude=deletenumber(ss1->magnitude);
			ss2->magnitude=deletenumber(ss2->magnitude);
			ss3->magnitude=deletenumber(ss3->magnitude);
			delete ss1;
			delete ss2;
			delete ss3;
		}
		PhiNRelativePrimeCK->gcm=deletenumber(PhiNRelativePrimeCK->gcm);
		PhiNRelativePrimeCK->number1_multiple_gcm=deletenumber(PhiNRelativePrimeCK->number1_multiple_gcm);
		if (!is_valid) PhiNRelativePrimeCK->number2_multiple_gcm=deletenumber(PhiNRelativePrimeCK->number2_multiple_gcm);
		delete PhiNRelativePrimeCK;

		while (!is_valid)
		{
			// Increment candidate, check again.
			aftemp1= new struct signednumber;
			aftemp1->sign = 1;
			aftemp1->magnitude = encoder_candidate;
			aftemp2 = new struct signednumber;
			aftemp2->sign = 1;
			aftemp2->magnitude = one;
			aftemp3 = addsigned(aftemp1, aftemp2);
			encoder_candidate=deletenumber(encoder_candidate);
			encoder_candidate=aftemp3->magnitude;
			delete aftemp1;
			delete aftemp2;
			delete aftemp3;

			PhiNRelativePrimeCK = gcm(PhiN, encoder_candidate);
			if (greater(PhiNRelativePrimeCK->gcm, one)==NULL)
			{
				ss1 = new struct signednumber;
				ss1->sign=1;
				ss1->magnitude=multiply(PhiNRelativePrimeCK->number1_multiple_gcm, PhiN);
				ss2 = new struct signednumber;
				ss2->sign=-1;
				ss2->magnitude=multiply(PhiNRelativePrimeCK->number2_multiple_gcm, encoder_candidate);
				ss3 = addsigned(ss1, ss2);
				if (ss3->sign==-1) // the sign of the sum
				{
					is_valid=1;
					decoder = PhiNRelativePrimeCK->number2_multiple_gcm;
				}
				ss1->magnitude=deletenumber(ss1->magnitude);
				ss2->magnitude=deletenumber(ss2->magnitude);
				ss3->magnitude=deletenumber(ss3->magnitude);
				delete ss1;
				delete ss2;
				delete ss3;
			}

			PhiNRelativePrimeCK->gcm=deletenumber(PhiNRelativePrimeCK->gcm);
			PhiNRelativePrimeCK->number1_multiple_gcm=deletenumber(PhiNRelativePrimeCK->number1_multiple_gcm);
			if(!is_valid) PhiNRelativePrimeCK->number2_multiple_gcm=deletenumber(PhiNRelativePrimeCK->number2_multiple_gcm);
			delete PhiNRelativePrimeCK;
		}

		the_pair = new struct pair;
		
		encoder_candidate=reverse(encoder_candidate);
		the_pair->encoder=copy(encoder_candidate);
		encoder_candidate=deletenumber(encoder_candidate);

		decoder=reverse(decoder);
		the_pair->decoder=copy(decoder);	
		decoder=deletenumber(decoder);
	}
	ftemp1=deletenumber(ftemp1);
	ftemp2=deletenumber(ftemp2);
	one = deletenumber(one);
	number_boardsl=deletenumber(number_boardsl);
	PhiN = deletenumber(PhiN);

	return(the_pair);
}

void listpairs(struct numbernode *N, struct numbernode* prime,
			   struct numbernode* another_prime, int number_boards)
{		
	numbernode *fcurrent;
	ofstream myfiled("decoderlist", ios::out);
	ofstream myfilee("encoderlist", ios::out);
	struct pair *apair;
	int f;
	for (f=0;f<number_boards;f++)
	{
		apair=board_pair(f, N, prime, another_prime, number_boards);

   		fcurrent=apair->decoder;
		while (fcurrent!=NULL)
		{
			myfiled<<fcurrent->digit;
			fcurrent=fcurrent->next;
		}
		myfiled << endl;

		fcurrent=apair->encoder;
		while (fcurrent!=NULL)
		{
			myfilee << fcurrent->digit;
			fcurrent=fcurrent->next;
		}
		myfilee << endl;

		cout << "done with listing pair " << f << endl;
		apair->decoder=deletenumber(apair->decoder);
		apair->encoder=deletenumber(apair->encoder);
		delete apair;
	}
	myfiled.close();
	myfilee.close();
	return;
}

void encryptmsg(struct numbernode* message, struct numbernode *N, int number_boards)
{
	// Use encoderlist to encrypt the message out to a file, for all the boards.
	ifstream encoderlist("encoderlist", ios::in);
	ofstream encryptedmsg("encryptedmsg", ios::out);

	char fnumber[16];  // all numbers are 15 digits or less.
	struct numbernode *fencoder, *fproduct1;
	char * charp;
	int j, k;

	// Encrypt the message with the encoders for all the boards.
	for (k=0; k<number_boards; k++)
	{
		j=0;
		while(encoderlist.get(fnumber[j])&&fnumber[j]!=10)
			j++;
		fnumber[j]='\0';
		fencoder=stringtonumber(fnumber);

		fproduct1 = apowerbmodc(message, fencoder, N);
		charp = stringnum(fproduct1);
		encryptedmsg << charp << endl;
		delete [] charp;
		fproduct1=deletenumber(fproduct1);

		fencoder=deletenumber(fencoder);
		cout << "done encrypting for board " << k << endl;
	}
	return;
}

struct numbernode* decrypt(struct numbernode *encrypted_message, struct numbernode*N,
						   int board_serial_number, struct numbernode* prime, 
								 struct numbernode* another_prime,int number_boards)
{
	struct numbernode* product;
	struct numbernode* decoder;
	struct pair* this_pair=
		board_pair( board_serial_number, N, prime, another_prime, number_boards);
	decoder = this_pair->decoder;
	this_pair->encoder=deletenumber(this_pair->encoder);
	delete this_pair;
	product = apowerbmodc(encrypted_message, decoder, N);
	return (product);
}

void verify_encrypted(struct numbernode* message, struct numbernode *N, int number_boards)
{
	// Verify that all messages in encryptedmsg file can be decoded to match message.

	
	ifstream encryptedmsg("encryptedmsg", ios::in);
	ifstream decoderlist("decoderlist", ios::in);
	
	struct numbernode *encodedmsg;
	struct numbernode *decoder;
	struct numbernode *product;

	char number[16];  // all numbers are 15 digits or less.
	int j,k;

	int failed=0;

	k=0;
	while (failed==0 && k<number_boards)
	{
		j=0;
		while(encryptedmsg.get(number[j])&&number[j]!=10)
			j++;
		number[j]='\0';
		encodedmsg=stringtonumber(number);

		j=0;
		while(decoderlist.get(number[j])&&number[j]!=10)
			j++;
		number[j]='\0';
		decoder=stringtonumber(number);

		product = apowerbmodc(encodedmsg, decoder, N);
		if (greater(product, message)!=NULL) failed=1;
		cout << "done decrypting msg " << k << endl;
		k++;
		
	}
	if (failed==1)
	{
		char * charp;
		cout << endl << "A message failed to be decoded." << endl;
		cout << "The decoder used was for board # " << (k-1) << endl;
		charp = stringnum(message);
		cout << "message was " << charp << endl;
		delete [] charp;
		charp = stringnum(product);
		cout << "remessage was " << charp << endl;
		delete [] charp;
	}
	return;
}
