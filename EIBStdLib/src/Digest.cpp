#include "Digest.h"

CDigest::CDigest(DigestAlgorithm algorithm) : _algorithm(algorithm)
{
}

CDigest::~CDigest()
{
}

CString CDigest::HashFile(const CString& file_name)
{
	switch (_algorithm)
	{
	case ALGORITHM_MD5: return HashFile_MD5(file_name);
		break;
	default:
		break;
	}

	return EMPTY_STRING;
}

bool CDigest::Decode(const CString& cipher, CString& result)
{
	switch (_algorithm)
	{
	case ALGORITHM_MD5: throw CEIBException(GeneralError,"MD5 Cipher cannot be decoded");
		break;
	case ALGORITHM_BASE64: return CBase64::Decode(cipher, result);
		break;
	default:
		break;
	}
	throw CEIBException(GeneralError,"Unknown decoding algorithm");
	return false;
}

CString CDigest::HashFile_MD5(const CString& file_name)
{
	MD5* md5 = new MD5();
	FILE *file;
  	MD5_CTX context;

	int len;
  	unsigned char buffer[1024], digest[16];

	//open file
  	if ((file = fopen (file_name.GetBuffer(), "rb")) == NULL)
	{
		return EMPTY_STRING;
	}

	//init md5
 	md5->MD5Init (&context);

	//read the filecontent
	while ( (len = (int)fread (buffer, 1, 1024, file)) )
   	{
		md5->MD5Update (&context, buffer, len);
	}

	/*
	generate hash, close the file and return the
	hash as std::string
	*/
	md5->MD5Final (digest, &context);
 	fclose (file);
	return ConvertToString(digest);
}

CString CDigest::HashString(const CString& input)
{
	switch (_algorithm)
	{
	case ALGORITHM_MD5: return HashString_MD5(input);
		break;
	default:
		break;
	}

	return EMPTY_STRING;
}

CString CDigest::HashString_MD5(const CString& input)
{
	MD5* md5 = new MD5();

	MD5_CTX ctx;

	//init md5
	md5->MD5Init(&ctx);
	//update with our string
	md5->MD5Update(&ctx,(unsigned char*)input.GetBuffer(),input.GetLength());

	//create the hash
	unsigned char buff[16] = "";
	md5->MD5Final((unsigned char*)buff,&ctx);

	delete md5;

	//converte the hash to a string and return it
	return ConvertToString(buff);
}

CString CDigest::ConvertToString(unsigned char* buffer)
{
	char asciihash[33];

	int p = 0;
	for(int i=0; i<16; i++)
	{
		sprintf(&asciihash[p],"%02x",buffer[i]);
		p += 2;
	}
	asciihash[32] = '\0';
	CString ret(asciihash);
	return ret;
}
