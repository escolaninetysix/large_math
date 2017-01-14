#ifndef _LargeMath_
#define _LargeMath_ 1

/*LargeMath memory management procedures

Use of numbers.
Create a struct numbernode pointer.
	struct numbernode* mynumber1 = numberfromint(314);
	struct numbernode* mynumber2 = numberfromstring("3141592653589793238462643383279502884");
The pointer can receive a number from an integer, from a string.
The pointer can also receive a number from functions passed other numbernode linked lists.
	struct numbernode* myproduct = multiply(mynumber1, mynumber2);
	struct divout* myquotient = divide(mynumber2, mynumber1);
These functions generate the new number and then return it.  The intent is that a new struct numbernode* pointer
will receive the return numbernode struct.
Finally, the function is to be passed the pointer to the numbernode in order to delete all of the numbernodes in that
number's linked list.
	mynumber1 = deletenumber(mynumber1);
	mynumber2 = deletenumber(mynumber2);
Also, there is a struct called divout which contains two numbernode linked lists.
This also has a deletequotient function which deletes the two numbers inside it using deletenumber
 and then deletes its own divout.
Both deletenumber and deletequotient return null, so that you can assign the return value to the pointer to mark it empty.
	myquotient = deletequotient(myquotient);
*/
struct numbernode
{
	SInt32 digit;
	struct numbernode* next;
};

struct signednumber
{
	struct numbernode* magnitude;
	SInt32 sign;   // sign =-1 or 1
};

struct divout
{
	struct numbernode* quotient;
	struct numbernode* remainder;
};

struct leadingstruct
{
	SInt32 leadingdivisor;
	SInt32 leadingmet;
};

struct gcmout
{
	struct numbernode* number1_multiple_gcm;
	struct numbernode* number2_multiple_gcm;
	struct numbernode* gcm;
};

struct largepair
{
	struct numbernode *encoder;
	struct numbernode *decoder;
};

struct numbernode *stringtonumber(char *string);
struct numbernode* inttonumber (UInt32 integer);
char *stringnum(numbernode* number);
UInt32 intnum(numbernode* number);
void printrev (struct numbernode* number);
void printfwd (struct numbernode* number);
numbernode* reverse( struct numbernode* list);

struct numbernode* deletenumber(struct numbernode* number);
struct signednumber* deletesigned(struct signednumber* signednumber);
struct divout* deletequotient(struct divout* quotient);
struct numbernode* copy (struct numbernode* number);
struct numbernode* multiply(struct numbernode* number1, struct numbernode* number2);
struct divout* divide(struct numbernode* dividend, struct numbernode* divisor);
struct signednumber* addsigned(struct signednumber* number1, struct signednumber* number2);
struct numbernode* numnodegreater(struct numbernode* number1, struct numbernode* number2);

struct signednumber* opposite (struct signednumber* number);
struct numbernode* zeroclean (struct numbernode* number);
leadingstruct leading (struct numbernode* divisor, struct numbernode* met);

struct gcmout *gcm(numbernode* number1, numbernode* number2);
struct numbernode* apowerbmodc (struct numbernode* a, struct numbernode *b,
								struct numbernode *c);
struct largepair *cryptolargepair(struct numbernode *N, struct numbernode* prime, struct numbernode* another_prime, struct numbernode* searchpoint);
struct numbernode* encryptmessage(struct numbernode* message, struct numbernode* fencoder, struct numbernode *N);
struct numbernode* decryptmessage(struct numbernode* encodedmsg, struct numbernode* decoder, struct numbernode *N);

#endif //#define _LargeMath_ 1
