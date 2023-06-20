Compile comand:
1. Put EC_ElGamel.cpp and BigNumber.h under same folder
2. g++ EC_ElGamel.cpp -o EC_ElGamel

執行後首先是Encryption的部分，基本上就依序輸入即可。8個位元間可加或不加空格都可以。
每輸入完一個按enter即可輸入下一個，依序為Plaintext, Pa, nk。

第二部分是Decryption，與Encrytion相同。依序為Pk, Pb, na。

Encryption所需的時間比較久，尤其是在計算Pk和Pb的部分，大約需30秒。
Decryption所需的時間比較短，應該5秒左右即有結果。