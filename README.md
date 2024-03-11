# DES (Date Encryption Standard)

**Authors:** Leo Martinez III

**Contact:** [leo.martinez@students.tamuk.edu](mailto:leo.martinez@students.tamuk.edu)

**Created:** Spring 2024

---

This C++ program implements the DES Cipher, a type of block cipher, for encrypting and decrypting messages.

### Description:

- The DES_Encrypt.cpp/.exe script takes a plaintext and a 56-bit key as input. 
- It performs an initial permutation (IP) on the 64-bit plaintext. 
- The 64-bit block is divided into two 32-bit halves, left (L) and right (R). 
- The function performs 16 rounds of Feistel network operations using the subkeys derived from 
the key. 
- In each round, the right half is expanded, XORed with the subkey, and passed through S-boxes 
for substitution. 
- The results are then permuted and combined with the left half. 
- After 16 rounds, the left and right halves are swapped, and a final permutation (IP^(-1)) is 
applied. 
- The resulting 64-bit block is the ciphertext.
  
### Decryption:

- Use the same DES algorithm, but reverse the key order of the generated keys

- Example values for ciphertext and plaintext are provided for testing purposes.

### Note:

- Program was created in VS Code using C++


Here is a brief explanation of the items:
- **src:** folder that contains the source code python script: main.py (use this file to run the program)
- **README.md:** contains most basic information about the project
- **LICENSE:** Contains license information in regards to the Github repository

Additionally, ensure the "math" python is properly installed in your virtual environment as the .gcd() method is needed to ensure error checking to assist with decryption integrity.
