🖼 LSB-Based Image Steganography for Data Encoding and Decoding

📌 Project Overview

This project is a command-line based Image Steganography application developed using Embedded C.

The application securely embeds secret text data inside a BMP image using the Least Significant Bit (LSB) technique without visibly altering the image quality.

It supports both:

- 🔐 Encoding (Hiding secret message)
- 🔓 Decoding (Extracting hidden message)

---

🎯 Objective

To implement a secure data hiding mechanism by manipulating the least significant bits of pixel bytes in a BMP image file.

---

🛠 Technologies Used

- Embedded C
- Bitwise Operations
- File Handling (fopen, fread, fwrite)
- Pointers
- Structures
- Command-line Arguments
- Makefile (Modular Compilation)

---

 ⚙ How It Works

 🔐 Encoding Process

1. Read source BMP image
2. Read secret text file
3. Verify image capacity
4. Encode:
   - Magic string
   - Size of secret file
   - Secret file extension
   - Secret data
5. Modify only LSB of pixel bytes
6. Generate encoded output image

✔ No visible change in image quality  
✔ Secure and systematic data embedding  

---

 🔓 Decoding Process

1. Read encoded BMP image
2. Extract magic string
3. Extract secret file size
4. Extract secret file extension
5. Reconstruct original secret message
6. Save decoded file

---
 
