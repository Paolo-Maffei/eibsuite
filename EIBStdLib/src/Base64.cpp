#include <Base64.h>

CString CBase64::Decode(const CString& cipher)
{
	int in_len = cipher.GetLength();
	int i = 0,j = 0, in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
    CString ret;

	while (in_len-- && ( cipher[in_] != '=') && is_base64(cipher[in_]))
	{
		char_array_4[i++] = cipher[in_]; in_++;
		if (i ==4)
		{
			for (i = 0; i <4; i++){
				char_array_4[i] = base64_chars.Find(char_array_4[i]);
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++){
				ret += (char)char_array_3[i];
			}
			i = 0;
		}
	}

	if (i != 0)
	{
		for (j = i; j <4; j++){
			char_array_4[j] = 0;
		}

		for (j = 0; j <4; j++){
			char_array_4[j] = base64_chars.Find(char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++){
			ret += char_array_3[j];
		}
	}

	return ret;
}
