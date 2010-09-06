#include "DiffieHellman.h"

CDiffieHellman::CDiffieHellman(void): _generator(0),_modulus(0),_private_a(0),_private_b(0),_public_a(0),
_public_b(0),_shared_key(0)
{
}

CDiffieHellman::~CDiffieHellman(void)
{
	CleanMem();
}


// Checks the integer n for primality
bool CDiffieHellman::IsItPrime (int64 n, int64 a) 
{ 
	int64 d = XpowYmodN(a, n-1, n); 
	if (d==1) 
		return true; 
	else 
		return false; 
	 
} 

//Performs the miller-rabin primality test on a guessed prime n.
//trials is the number of attempts to verify this, because the function
//is not 100% accurate it may be a composite.  However setting the trial
//value to around 5 should guarantee success even with very large primes
bool CDiffieHellman::MillerRabin (int64 n, int64 trials) 
{ 
	int64 a = 0; 

	for (int64 i=0; i<trials; i++)
	{ 
		a = (rand() % (n-3))+2;// gets random value in [2..n-1] 
		
		if (IsItPrime (n,a)==false) 
		{ 
			return false; 
			//n composite, return false 
		} 
	} return true; // n probably prime 
} 


// Returns the Read Time Stamp Counter of the CPU
// The instruction returns in registers EDX:EAX the count of ticks from processor reset.
// Added in Pentium. Opcode: 0F 31.
int64 CDiffieHellman::GetRTSC( void )
{
#ifdef WIN32	
	int tmp1 = 0;
	int tmp2 = 0;
	
	__asm
	{
		RDTSC;			// Clock cycles since CPU started
		mov tmp1, eax;
		mov tmp2, edx;
	}

	return ((int64)tmp1 * (int64)tmp2);
#else
#ifdef PPC
	/*
	unsigned int tbl, tbu0, tbu1;
	do
	{
		__asm__ __volatile__ ("mftbu %0" : "=r"(tbu0));
	    __asm__ __volatile__ ("mftb %0"  : "=r"(tbl) );
	     __asm__ __volatile__ ("mftbu %0" : "=r"(tbu1));
	} while (tbu0 != tbu1);
	return (((int64)tbu0) << 32) | tbl;
	*/
	return rand()*rand();
#else
	int64 ll;
	__asm__ __volatile__ ("rdtsc" : "=A" (ll));
	return ll;
#endif
#endif
}

// Generates a random number by first getting the RTSC of the CPU, then 
// uses a Linear feedback shift register.
// The RTSC is then added to fill the 64-bits
uint64 CDiffieHellman::GenerateRandomNumber(void)
{
  static unsigned long rnd = 0x41594c49;
  static unsigned long x   = 0x94c49514;

  LFSR(x); 
  rnd^=GetRTSC()^x; 
  ROT(rnd,7);

  return (uint64)GetRTSC() + rnd;
} 



//Generates a large prime number by
//choosing a randomly large integer, and ensuring the value is odd
//then uses the miller-rabin primality test on it to see if it is prime
//if not the value gets increased until it is prime
uint64 CDiffieHellman::GeneratePrime()
{
	uint64  tmp = 0;

	tmp	=  GenerateRandomNumber() % MAX_PRIME_NUMBER;

	//ensure it is an odd number
	if ((tmp & 1)==0)
		tmp += 1;

	if (MillerRabin(tmp,5)==true) return tmp;
	
	do
	{
		tmp+=2;	
	} while (MillerRabin(tmp,5)==false);
		
	return tmp;
}
 
//Raises X to the power Y in modulus N
//the values of X, Y, and N can be massive, and this can be 
//acheived by first calculating X to the power of 2 then 
//using power chaining over modulus N
int64 CDiffieHellman::XpowYmodN(int64 x, int64 y, int64 N)
{
	int64 tmp = 0;
	if (y==1) return (x % N);

	if ((y&1)==0)
	{
		tmp = XpowYmodN(x,y/2,N);
		return ((tmp * tmp) % N);
	}
	else
	{
		tmp = XpowYmodN(x,(y-1)/2,N);
		tmp = ((tmp * tmp) % N);
		tmp = ((tmp * x) % N);
		return (tmp);
	}
}


void CDiffieHellman::CreateKeys(int64 &Generator, int64 &Modulus)
{
	int64 swap = 0;

	//Check if keys have already been established
	if ((_modulus!=0) && (_generator!=0))
	{
		Generator = _generator;
		Modulus   = _modulus;
	}
	else
	{
		_generator = GeneratePrime();
		_modulus = GeneratePrime();

		if (_generator>_modulus)
		{
			swap = _generator;
			_generator    = _modulus;
			_modulus    = swap;
		}
		Generator = _generator;
		Modulus   = _modulus;
	}
}

void CDiffieHellman::CreateSenderInterKey(int64 &InterKey)
{
	_private_a = (int64) (GenerateRandomNumber() % MAX_RANDOM_INTEGER);

	_public_a = XpowYmodN(_generator,_private_a,_modulus);
	
	InterKey = _public_a;
}

void CDiffieHellman::CreateRecipientInterKey(int64 &InterKey, int64 Generator, int64 Modulus)
{

	_private_b = (int64) (GenerateRandomNumber() % MAX_RANDOM_INTEGER);

	_generator = Generator;
	_modulus = Modulus;

	_public_b = XpowYmodN(_generator,_private_b,_modulus);
	
	InterKey = _public_b;
}

void CDiffieHellman::CreateSenderEncryptionKey(int64 &EncryptionKey, int64 RecipientInterKey)
{
	_public_b = RecipientInterKey;
	_shared_key = XpowYmodN(_public_b,_private_a,_modulus);
	EncryptionKey = _shared_key;
	_shared_key_str = EncryptionKey;
}


void CDiffieHellman::CreateRecipientEncryptionKey(int64 &EncryptionKey, int64 SendersInterKey)
{
	_public_a = SendersInterKey;
	_shared_key = XpowYmodN(_public_a,_private_b,_modulus);
	EncryptionKey = _shared_key;
	_shared_key_str = EncryptionKey;
}


void CDiffieHellman::CleanMem(unsigned int flags)
{
	_generator = 0;
	_modulus = 0;
	_private_a = 0;
	_private_b = 0;
	_public_a = 0;
	_public_b = 0;
	
	if (flags != CLEAN_ALL_MEMORY_EXCEPT_KEY)
	{
		_shared_key = 0;
	}
}

int CDiffieHellman::GetValue(int64 &value,unsigned int dwFlags)
{
	switch (dwFlags)
	{
		case MODULUS:
		{
			value = _modulus;
			break;
		}
		case GENERATOR:
		{
			value = _generator;
			break;
		}
		case PRIVATE_A:
		{
			value = _private_a;
			break;
		}
		case PRIVATE_B:
		{
			value = _private_b;
			break;
		}
		case PUBLIC_A:
		{
			value = _public_a;
			break;
		}
		case PUBLIC_B:
		{
			value = _public_b;
			break;
		}
		case KEY:
		{
			value = _shared_key ;
			break;
		}
	}
	return 0;			
}


