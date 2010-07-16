/*! \file DiffieHellman.h
    \brief Diffie Hellman Class - Source file
    
	This is The header file for CDiffieHellman class. This Class handles the DiffeHellman protocol.

*/
#ifndef __DIFFIE_HELLMAN_HEADER__
#define __DIFFIE_HELLMAN_HEADER__

#include "EibStdLib.h"
#include "CString.h"

// CRYPTO LIBRARY FOR EXCHANGING KEYS
// USING THE DIFFIE-HELLMAN KEY EXCHANGE PROTOCOL

#define MAX_RANDOM_INTEGER 2147483648 //Should make these numbers massive to be more secure
#define MAX_PRIME_NUMBER   2147483648 //Bigger the number the slower the algorithm

//Linear Feedback Shift Registers
#define LFSR(n)    {if (n&1) n=((n^0x80000055)>>1)|0x80000000; else n>>=1;}
//Rotate32
#define ROT(x, y)  (x=(x<<y)|(x>>(32-y)))

#define CLEAN_ALL_MEMORY				1
#define CLEAN_ALL_MEMORY_EXCEPT_KEY		2


#define MODULUS							1
#define GENERATOR						2
#define PRIVATE_A						3
#define PRIVATE_B						4
#define PUBLIC_A						5
#define PUBLIC_B						6
#define KEY								7

/*! \class CDiffieHellman
	\brief DiffieHellman class. this class is responsible to implement all the neccessary action taken during
		   the diffe-hellman protocol. this class genrate random numbers acroding to the CPU time stamp. 
		   (EAX opcode in the x86 processors). to generate large random number we use integer in 64 bit size. in addition,
		   Miller-Rabin algorithm is used to determine whether a number is prime or not.
*/
class EIB_STD_EXPORT CDiffieHellman
{
public:
	/*!Default Constructor*/
	CDiffieHellman();
	/*!Destructor*/
	virtual ~CDiffieHellman();

	/*!
	\brief Method to generate the base numbers for the diffie hellman algorithm. (the mudulus factor and the generator)
	\fn void CreateKeys(int64 &Generator, int64 &Modulus);
	\param Generator will be filled with the generated value
	\param Modulus will be filled with the generated value
	*/
	void CreateKeys(int64 &Generator, int64 &Modulus);
	/*!
	\brief Method to generate the sender Public key
	\fn void CreateSenderInterKey(int64 &InterKey);
	\param InterKey will be filled with the generated value
	*/
	void CreateSenderInterKey(int64 &InterKey);
	/*!
	\brief Method to generate the Recipient Public key
	\fn void CreateRecipientInterKey(int64 &InterKey, int64 Generator, int64 Modulus)
	\param InterKey will be filled with the generated value
	\param Generator the generator value to use
	\param Modulus the modulus value to use
	*/
	void CreateRecipientInterKey(int64 &InterKey, int64 Generator, int64 Modulus);
	/*!
	\brief Method to generate the sender Private key
	\fn void CreateSenderEncryptionKey(int64 &EncryptionKey, int64 RecipientInterKey)
	\param EncryptionKey will be filled with the generated value
	\param RecipientInterKey the Recipient Public key
	*/
	void CreateSenderEncryptionKey(int64 &EncryptionKey, int64 RecipientInterKey);
	/*!
	\brief Method to generate the Recipient Private key
	\fn void CreateRecipientEncryptionKey(int64 &EncryptionKey, int64 SendersInterKey)
	\param EncryptionKey will be filled with the generated value
	\param SendersInterKey the sender Public key
	*/
	void CreateRecipientEncryptionKey(int64 &EncryptionKey, int64 SendersInterKey);
	/*!
	\brief Get Method
	\fn int GetValue(int64 &value, unsigned int flags = KEY)
	\param value will be filled according to the flags parameter
	\param flags the value to get
	*/
	int GetValue(int64 &value, unsigned int flags = KEY);
	/*!
	\brief Get Method
	\fn int64 GetPublicSender()
	\return Sender public key
	*/
	int64 GetPublicSender() { return _public_a;}
	/*!
	\brief Get Method
	\fn int64 GetPublicRecipient()
	\return Recipient public key
	*/
	int64 GetPublicRecipient() { return _public_b;}

	/*!
	\brief Get Method
	\fn const CString& GetSharedKey()
	\return The shared key
	*/
	const CString& GetSharedKey() { return _shared_key_str; }

private:
	int64 XpowYmodN(int64 x, int64 y, int64 N);
	uint64 GenerateRandomNumber( void );
	uint64 GeneratePrime();
	int64 GetRTSC( void );
	bool IsItPrime (int64 n, int64 a);
	bool MillerRabin (int64 n, int64 trials);

	int64	_generator;
	int64 _modulus;
	int64 _private_a;
	int64 _private_b;
	int64 _public_a;
	int64 _public_b;
	int64 _shared_key;
	CString _shared_key_str;


	void CleanMem(unsigned int flags = CLEAN_ALL_MEMORY);

};

#endif
