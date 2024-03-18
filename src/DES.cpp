// DES (Data Encryption Standard) Block Cipher
// Key Generation, Encryption, Decryption
// Created by: Leo Martinez III in Spring 2024

// Background Information about DES:
// DES is a Symmetric Block Cipher (same key used for decryption as encryption).
// DES was eventually replaced by AES (Advanced Security Standard) in 2001.
// Core Components:
// Input: 64 bits
// Output: 64 bits
// Main Key: 64 bits
// Subkey: 56 bits
// Round Key: 48 bits
// Number of Rounds: 16 rounds

// necessary inclusions
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// subkeys needed for each round of the cipher (16 round) in array form
string keys[16];

// plaintext placeholder
string plaintext;

// convert decimal to binary
string DecimalToBinary(int decimal)
{
	string binary;
    while(decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary; 
		decimal = decimal/2;
	}
	while(binary.length() < 4){
		binary = "0" + binary;
	}
    return binary;
}

// convert binary to decimal
int BinaryToDecimal(string binary)
{
    int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for(int i = size-1; i >= 0; i--)
	{
    	if(binary[i] == '1'){
        	decimal += pow(2, counter);
    	}
    counter++;
	}
	return decimal;
}

// function perform circular left shifts on the given key partition, shift bits by one position
string shift_by_one_left(string key_partition){ 
    string shifted_val="";  
        for(int i = 1; i < 28; i++){ 
            shifted_val += key_partition[i]; 
        } 
        shifted_val += key_partition[0];   
    return shifted_val; 
} 
// function perform circular left shifts on the given key partition, shift bits by two positions
string shift_by_two_left(string key_partition){ 
    string shifted_val=""; 
    for(int i = 0; i < 2; i++){ 
        for(int j = 1; j < 28; j++){ 
            shifted_val += key_partition[j]; 
        } 
        shifted_val += key_partition[0]; 
        key_partition= shifted_val; 
        shifted_val =""; 
    } 
    return key_partition; 
}

// perform logical operation XOR between two strings
string XOR(string a, string b){ 
	string outcome = ""; 
	int size = b.size();
	for(int i = 0; i < size; i++){ 
		if(a[i] != b[i]){ 
			outcome += "1"; 
		}
		else{ 
			outcome += "0"; 
		} 
	} 
	return outcome; 
} 

void gen_keys(string key){
	// permutation table 0
	int perm0[56] = {
	57,49,41,33,25,17,9, 
	1,58,50,42,34,26,18, 
	10,2,59,51,43,35,27, 
	19,11,3,60,52,44,36,		 
	63,55,47,39,31,23,15, 
	7,62,54,46,38,30,22, 
	14,6,61,53,45,37,29, 
	21,13,5,28,20,12,4 
	};
	// permutation table 1
	int perm1[48] = { 
	14,17,11,24,1,5, 
	3,28,15,6,21,10, 
	23,19,12,4,26,8, 
	16,7,27,20,13,2, 
	41,52,31,37,47,55, 
	30,40,51,45,33,48, 
	44,49,39,56,34,53, 
	46,42,50,36,29,32 
	}; 
	
	// initial key is compressed using perm table 0
	string perm_key ="";  
	for(int i = 0; i < 56; i++){ 
		perm_key += key[perm0[i]-1]; 
	} 

	// compressed keys are then split into two halves 'left' and 'right'
	string left = perm_key.substr(0, 28); 
	string right = perm_key.substr(28, 28); 

	// generate 16 keys
	for(int i=0; i<16; i++){ 
		// shift by one for rounds 1, 2, 9, 16
		if(i == 0 || i == 1 || i==8 || i==15 ){
			left= shift_by_one_left(left); 
			right= shift_by_one_left(right);
		} 
		// shift by two for all other rounds
		else{
			left= shift_by_two_left(left); 
			right= shift_by_two_left(right);
		}

	// concatenate the left and right
	string combined_key = left + right;
	string round_key = ""; 

	// perm1 table is used for the final key bits
	for(int i = 0; i < 48; i++){ 
		round_key += combined_key[perm1[i]-1]; 
	}

	// generate a key for each round based on the DES Cipher methodology  
	keys[i] = round_key;
	//	cout<<"Key "<<i+1<<": "<<keys[i]<<endl;
    // Use the line above to print out each key generated for each round 
	} 
}

// Implementing the algorithm
string DES(){ 
	// initial permutation (IP) table
	int init_perm[64] = { 
	58,50,42,34,26,18,10,2, 
	60,52,44,36,28,20,12,4, 
	62,54,46,38,30,22,14,6, 
	64,56,48,40,32,24,16,8, 
	57,49,41,33,25,17,9,1, 
	59,51,43,35,27,19,11,3, 
	61,53,45,37,29,21,13,5, 
	63,55,47,39,31,23,15,7 
	}; 
	// table to perform expansion
	int expansion_table[48] = { 
	32,1,2,3,4,5,4,5, 
	6,7,8,9,8,9,10,11, 
	12,13,12,13,14,15,16,17, 
	16,17,18,19,20,21,20,21, 
	22,23,24,25,24,25,26,27, 
	28,29,28,29,30,31,32,1 
	}; 

	// substitution boxes
	int sub_box[8][4][16]=  
	{{ 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }, 
    { 
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 
    }, 
    { 
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 
    }, 
    { 
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 
    }, 
    { 
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 
    }, 
    { 
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 
    }, 
    { 
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 
    }, 
    { 
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
    }};

	// permutation table
	int perm_table[32] = { 
	16,7,20,21,29,12,28,17, 
	1,15,23,26,5,18,31,10, 
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25 
	};

	// inverse permutation (IP^-1) table
	int inv_perm[64]= { 
	40,8,48,16,56,24,64,32, 
	39,7,47,15,55,23,63,31, 
	38,6,46,14,54,22,62,30, 
	37,5,45,13,53,21,61,29, 
	36,4,44,12,52,20,60,28, 
	35,3,43,11,51,19,59,27, 
	34,2,42,10,50,18,58,26, 
	33,1,41,9,49,17,57,25 
	};

	// perform initial permutation (IP)
  	string perm = ""; 
	for(int i = 0; i < 64; i++){ 
		perm += plaintext[init_perm[i]-1]; 
	}  

	// split into two halves (left and right) 
	string left = perm.substr(0, 32); 
	string right = perm.substr(32, 32);

	// plain text is encrypted each round (16 rounds)
	for(int i=0; i<16; i++) { 
    	string right_expanded = ""; 
		// right half of the plaintext is expanded using the expansion table
    	for(int i = 0; i < 48; i++) { 
      		right_expanded += right[expansion_table[i]-1]; 
    };  // result XOR current key
		string XORed = XOR(keys[i], right_expanded);  
		string res = "";
 
		// divide result into 8 equal parts and pass through 8 sub boxes
		for(int i=0;i<8; i++){ 
			// substituion box, each box is reduces from 6 to 4 bits, use row + column pair to find sub values
      		string row1= XORed.substr(i*6,1) + XORed.substr(i*6 + 5,1);
      		int row = BinaryToDecimal(row1); // binary values converted to decimal
      		string col1 = XORed.substr(i*6 + 1,1) + XORed.substr(i*6 + 2,1) + XORed.substr(i*6 + 3,1) + XORed.substr(i*6 + 4,1);;
			int col = BinaryToDecimal(col1);
			int val = sub_box[i][row][col];
			res += DecimalToBinary(val);  // decimal values converted to binary 
		} 
		// another layer of permutation is used
		string perm2 =""; 
		for(int i = 0; i < 32; i++){ 
			perm2 += res[perm_table[i]-1]; 
		}

		// XOR result with the left half
		XORed = XOR(perm2, left);
		// swap left and right sections
		left = XORed; 
		if(i < 15){ 
			string temp = right;
			right = XORed;
			left = temp;
		} 
	} 

	// concatenate the left and right side
	string combined_text = left + right;   
	string outcome ="";

	// inverse permutation (IP^-1) is applied
	for(int i = 0; i < 64; i++){ 
		outcome += combined_text[inv_perm[i]-1]; 
	}
	// return the final string value (encryption for normal key order, decryption for reverse key order)
	return outcome; 
}
int main(){ 
	// 64 bit length key (can be changed as desired or for testing)
	string key = "1010101010111011000010010001100000100111001101101100110011011101";

	// 64 bit block length plaintext (can be changed as desired or for testing)
	plaintext = "1010101111001101111001101010101111001101000100110010010100110110";
    
    // save the value of plaintext for later use (checking if decryption worked properly)
	string pt = plaintext;

	// generate a key for each round of DES (16)
  	gen_keys(key);
    cout<<"\nPlaintext: "<<plaintext<<endl; // print out the value of plaintext

	// perform encryption on the plaintext
    string ciphertext = DES(); 
    cout<<"Ciphertext: "<<ciphertext<<endl; // print out the value of ciphertext

	// to perform decrpytion, the order of the keys must be reversed
	int i = 15;
	int j = 0;
	while(i > j)
	{
		string temp = keys[i];
		keys[i] = keys[j];
		keys[j] = temp;
		i--;
		j++;
	}

    // use the ciphertext as the input "plaintext" so it can be decrypted using reverse key order
	plaintext = ciphertext;
	string decrypted_ciphertext = DES();
	cout<<"Decrypted text:"<<decrypted_ciphertext<<endl; // print out the decrypted value

	// compare the original placeholder plaintext (pt) to see if it matches the decrpyted value (decrypted_ciphertext)
	if (decrypted_ciphertext == pt){
		cout<<"\nSuccessful!\nThe original plaintext value matches the final value after performing encryption and decryption."<<endl;}
    else {
        cout<<"\nError!\nAn error has occurred somewhere, the plaintext values do NOT."<<endl;
	}
} 
